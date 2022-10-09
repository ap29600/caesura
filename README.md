# Caesura

An array programming language with a focus on tacit programming and expressivity

## Grammar

```
Definition = Identifier '::' Expression
           | Identifier Identifier '::' Expression
           | Identifier Identifier Identifier '::' Expression

Expression = Monadic
           | Dyadic
           | Value

Value      = Atom
           | MonadComp
           | DyadComp

MonadComp  = ( Atom | DyadComp ) '.' ( Atom | DyadComp | MonadComp )

DyadComp   = ( Atom | DyadComp ) ':' Atom

Atom       = Identifier
           | Literal
           | Immediate
           | '(' Expression ')'

Immediate  = '!' Atom

Literal    = Number ( ',' Number )*

Monadic    = Value ( Value | Dyadic )

Dyadic     = Value Value ( Value | Dyadic )

Number     = /[-+]?[0-9]+(.[0-9]+)?(e[-+]?[0-9]+)?/

Identifier = /[^.:,()!]+/
```

Note: the `DyadComp` rule is left-recursive. This might be an issue while
generating parsers for the grammar.

For example the string `- 1 (foo : bar . baz) 3, 4, 5` would parse to:

```
Expression
    Monadic
        Value Identifier '-'
        Dyadic
            Value Atom Literal 1
            Value
                '('
                MonadComp
                    DyadComp
                        Atom Identifier 'foo'
                        ':'
                        Atom Identifier 'bar'
                    '.'
                    Atom Identifier 'baz'
                ')'
            Value Atom Literal '3, 4, 5'
```


## Semantic Meaning of language constructs

- `Literal`s and `Identifier`s are scalar values, arrays or variable names.
variables may evaluate to scalar values, arrays, or functions.

- `Monadic` expressions indicate function composition, where any non-function
object is treated as a function of no arguments;

For example if `baz` $: (A \times B) \to C$ and `foo` $: C \to D$ then
`(foo bar)` $: (A \times B) \to C$.

Any scalar of type $T$ is equivalent for the purpose of composition to a
function of type $\\{\emptyset\\} \to T$.

- `Dyadic` expressions indicate composition of a two-argument function, with
the same rules as before for composition with scalars.

For example if
`foo` $: \Omega \le (A \times B) \to C$,
`bar` $: \Theta \le (A \times B) \to D$,
`baz` $:(C \times D) \to E$ then `(foo baz bar)` $: (A \times B) \to D$
provided that $\le$ is the relation of being a quotient of the product set,
and that $(A \times B)$ is chosen minimally.

a concrete example would be:
`foo` $: (\mathbb N \times \mathbb Q) \to \mathbb R$
`bar` $: (\\{\emptyset\\} \times \mathbb Q) \to \mathbb R$
`baz` $: (\mathbb R \times \mathbb R) \to \mathbb C$
`(foo baz bar)` $: (\mathbb N \times \mathbb Q) \to \mathbb C$

where we can see that `foo` takes an argument in the whole set
$(\mathbb N \times \mathbb Q)$, while `bar` only takes the component in
$\mathbb Q$, and the minimal product space containing the domains of both 
`foo` and `bar` is $(\mathbb N \times \mathbb Q)$.

- `MonadComp` is syntactic sugar for `Monadic`:
`foo . bar` is equivalent to `(foo bar)` and its associativity can be used to
avoid using lots of parentheses: `foo . bar . baz . fee 10` is equivalent to
`foo (bar (baz (fee 10)))`

- `DyadComp` is syntactic sugar for `Dyadic`:
`foo : bar` is equivalent to `(foo bar ->)` where `->` is the identity
function.

- `Immediate` turns the contained `Atom` into a non-function value.
This is useful for higher order functions: For example `(foo !bar) baz`
indicates applying to `baz` the function returned from `foo` applied to `bar`.

- `Definition`s bind the `Expression` on the right to the `Identifier`s on the
left, pattern-matching them as a `Monadic` or `Dyadic` expression.

For example if there are three identifiers, the middle one is used as a bind name,
while the left and right are the formal left and right arguments and are available
in the definition body.

## Operator precedence

| operator |    name     | precedence | associativity |
|----------|-------------|------------|---------------|
|   `::`   |  `define`   |     0      |      ---      |
|   ` `    |   `apply`   |     1      |     right     |
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
