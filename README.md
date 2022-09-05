# Caesura

An array programming language with a focus on tacit programming and expressivity

## Basic syntax and semantics

a caesura program is composed of tokens.
a token may be:
- a literal scalar value (`floating`, `integer`, `boolean`, `character`)
- an identifier: a string matching `[^.,'():space:]+`.
- a special operator: one of `.`, `:`, `,`, `!`, `(`, `)`, `::`.

### Expression

a literal value, identifier or function application

### Function application

in Caesura, functions may be monadic or dyadic, meaning they take either one or two arguments.
Scalars and arrays can be seen as special cases of nonadic functions (zero-argument functions).

Caesura uses infix notation, and function application is done by adjoining the function and the value(s).
- a monadic function has its only argument on the right: `f x` (equivalent to lisp's `(f x)`)
- a dyadic function has a left argument and a right argument: `y f x` (equivalent to lisp's `(f x y)`)

Note that it does not matter whether `a` or `b` are functions or data: the result of `a f b` is equivalent to: 
`(lambda (x y) (f (a x y) (b x y)))` which is usually called a train in array languages.
If `a` or `b` only take one argument (or take none) then the left argument is discarded first, then the right.

#### Forcing monadic or dyadic application

While writing complex expressions, monadic or dyadic application can be enforced by the following operators: `:`, `.`

- the operator `:` (`dyadic`) enforces dyadic application of its right argument on its left argument and an additional argument on the right.
    The expression `y:f x` is similar to lisp's `(f x y)`.
    The right argument of `f` can be omitted and `(y:f)` is equivalent to `(y f ->)`; if `y` takes no arguments this is equivalent to partial application.

Example: given that the dyadic meaning of `*` is multiplication, the dyadic meaning of `+` is sum, and the `:` operator is left-associative, `(1:+:*) x` expands to `((1 + x) * x)`.

- the operator `.` (`monadic`) enforces monadic application of its left argument to its right argument.
    The expression `f.x` denotes monadic application of the function `f` to the parameter `x`.
    The right argument can be omitted and `(f.)` is equivalent to `(f ->)`.

Example: given that the monadic meaning of `*` is squaring and the monadic meaning of `-` is negation, `(-.*.) 2` evaluates to `-4`.

#### Function objects

given a higher order function `f` expecting a function as the right argument, in order to provide said argument the programmer must inhibit the
composition semantics of `f g`.

In order to do this, the operator `!` (`immediate`) transforms an expression into the corresponding function object, preventing composition
(that is, `!` returns the nonadic function whose value is equal to its argument).
`f!g` is then equal to substituting the formal right argument with `g` in the body of `f`.
This also works with a dyadic application: `!f g !h` applies `g` to the function objects `!f` and `!h`.
If `g` is not a function, it remains unchanged.

Example: given `flip f :: (<- f ->)` the expression `1 (flip !-) 0` evaluates to `-1`

### Building arrays

The `,` (`enlist`) operator can be used in order to construct a one-dimensional array from multiple values. The operators `,,`, `,,,`, ... are reserved for building
higher dimensional arrays, but the exact behaviour is not yet defined.

Example: `1,2,3 + 4,5,6` evaluates to `5,7,9`

### Definition

a sequence of 1, 2 or 3 identifiers at the start of a line, followed by the operator `::` (`define`), followed by an expression that spans the rest of the line.
the following cases are possible:
- `a :: (...)`: `(...)` is evaluated and the result is bound to the identifier `a`
- `f g :: (...) ` where `(...)` is an expression (possibly making use of `g`): binds to `f` the monadic function whose value is `(...)` when applied to the formal parameter `g`.
- `h f g :: (...) ` where `(...)` is an expression (possibly making use of `g` and `h`): binds to `f` the dyadic function whose value is `(...)` when applied to the formal parameters `g` and `h`.

## Operator precedence

| operator |    name     | precedence | associativity |
|----------|-------------|------------|---------------|
|   `::`   |  `define`   |     0      |      ---      |
|   ` `    |   `apply`   |     1      |     left      |
|   `.`    |  `monadic`  |     2      |     left      |
|   `:`    |  `dyadic`   |     3      |     left      |
|   `,`    |  `enlist`   |     4      |     left      |
|   `!`    | `immediate` |     5      |      ---      |

### primitives:

| symbol |       monadic        |        dyadic        |
|--------|----------------------|----------------------|
|  `*`   |        square        | multiplication (AND) |
|  `+`   |       identity       |    addition  (OR)    |
|  `-`   | arithmetic negation  |     subtraction      |
|  `~`   | 1's complement (NOT) |    mismatch (XOR)    |
|  `\|`  |         ---          |        rotate        |
|  `->`  |       identity       |    right argument    |
|  `<-`  |         ---          |    left argument     |
|  `/`   |         ---          |       reduce         |
|  `\`   |         ---          |       scan           |


### examples:

|      name       |  type   |   explicit definition    | tacit definition (explicit) | tacit definition (operators) |
|-----------------|---------|--------------------------|-----------------------------|------------------------------|
|    increment    | monadic |     `inc x :: 1 + x`     |       `inc :: 1 + ->`       |         `inc :: 1:+`         |
| successive diff | monadic | `diff x :: (1 \| x) - x` |  `diff :: (1 \| ->) - ->`   |      `diff :: (1:\|):-`      |
