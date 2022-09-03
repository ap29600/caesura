# Caesura

An array programming language with a focus on tacit programming and expressivity

## Basic syntax and semantics

a caesura program is composed of tokens.
a token may be:
- a literal scalar value (`floating`, `integer`, `boolean`, `character`)
- an identifier: a string matching `[^.,'():space:]+`.
- a special operator: one of `.`, `,`, `'`, `(`, `)`, `:=`.

### expression

a literal value, identifier or function application

### function application

in Caesura, functions may be monadic or dyadic, meaning they take either one or two arguments.
Scalars and arrays can be seen as special cases of nonadic functions (zero-argument functions).

Caesura uses infix notation, and function application is done by adjoining the function and the value(s).
- a monadic function has its only argument on the right: `f x` (equivalent to lisp's `(f x)`)
- a dyadic function has a left argument and a right argument: `y f x` (equivalent to lisp's `(f x y)`)

Note that it does not matter whether `a` or `b` are functions or data: the result of `a f b` is equivalent to: 
`(lambda (x y) (f (a x y) (b x y)))` which is usually called a train in array languages.
If `a` or `b` only take one argument (or take none) then the left argument is discarded first, then the right.

#### forcing monadic or dyadic application

While writing complex expressions, monadic or dyadic application can be enforced by the following operators: `'`, `.`

- the operator `'` enforces dyadic application of its right argument on its left argument and an additional argument on the right,
    and it has a precedence of 2. The expression `y'f x` is similar to lisp's `(f x y)`. This syntax will enforce dyadic
    application even when the right argument is not present: `y f` usually denotes applying the function `y` monadically
    to the argument `f`, but `y'f` denotes partial application of the function `f` to the left argument `y` and evaluates
    to the monadic function `(lambda (x) (f x y))`

- the operator `.` enforces monadic application of its left argument to its right argument, and it has a precedence of 1.
    the expression `f.x` is similar to lisp's `(f x)`. crucially, if `x` is itself a function, `.` can be interpreted as
    denoting function composition.

for reference, regular function application by adjacency has a precedence of 0.

### assignment

a sequence of 1, 2 or 3 identifiers at the start of a line, followed by the operator `:=`, followed by an expression that spans the rest of the line.
the following cases are possible:
- `a := (...)`: `(...)` is evaluated and the result is bound to the identifier `a`
- `f g := (...) ` where `(...)` is an expression (possibly making use of `g`): binds to `f` the monadic function whose value is `(...)` when applied to the formal parameter `g`.
- `h f g := (...) ` where `(...)` is an expression (possibly making use of `g` and `h`): binds to `f` the dyadic function whose value is `(...)` when applied to the formal parameters `g` and `h`.

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


### examples:

|      name       |  type   |   explicit definition    | tacit definition (explicit) | tacit definition (operators) |
|-----------------|---------|--------------------------|-----------------------------|------------------------------|
|    increment    | monadic |     `inc x := 1 + x`     |       `inc := 1 + ->`       |         `inc := 1'+`         |
| successive diff | monadic | `diff x := (1 \| x) - x` |  `diff := (1 \| ->) - ->`   |      `diff := (1'\|)'-`      |

