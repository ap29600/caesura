# Caesura

An array programming language with a focus on tacit programming and expressivity

## Basic syntax and semantics

a caesura program is composed of tokens.
a token may be:
- a literal scalar value (`floating`, `integer`, `boolean`, `character`)
- an identifier: a string matching `[^,.():space:]+`.
- a special operator: one of `.`, `,`, `(`, `)`, `:=`.

### expression

a literal value, identifier or function application

### function application

caesura by default uses infix notation, and function application is done by adjunction of the function and the value(s).
- caesura: `a f b`
- lisp:    `(f a b)`

note that it does not matter whether `a` or `b` are functions or data, as functions in caesura are entirely first-class:
the result of `a f b` is then equivalent to: `(lambda (x y) (f (a x y) (b x y)))` which is usually called a train in array languages.
If `a` or `b` only take one argument (or take none) then the left argument is discarded first, then the right.
In this sense, scalars and arrays can be seen as special cases of nonadic functions that evaluate to their content.

#### special form: function composition

often, the programmer will want to apply a function on the result of a previous function application, as in lisp: `(f (g 10))`;
however trying to do so by adjunction (caesura: `f g 10`) does not indicate composition in the usual way.
The obvious way to achieve this is to use parentheses (caesura: `f (g 10)`) however if many functions are composed the syntax may be uncomfortable.
the special operator `.` signifies function composition, and the expression `f.g` does not evaluate to `(lambda (x y) ('.' (f x y) (g x y)))`,
but rather to `(lambda (x y) (f (g x y)))`. The `.` operator has higher precedence than function application.

### assignment

a sequence of 1, 2 or 3 identifiers at the start of a line, followed by the operator `:=`, followed by an expression that spans the rest of the line.
the following cases are possible:
- `a := (...)`: `(...)` is evaluated and the result is bound to the identifier `a`
- `f g := (...) ` where `(...)` is an expression (possibly making use of `g`): binds to `f` the monadic function whose value is `(...)` when applied to the formal parameter `g`.
- `h f g := (...) ` where `(...)` is an expression (possibly making use of `g` and `h`): binds to `f` the dyadic function whose value is `(...)` when applied to the formal parameters `g` and `h`.
