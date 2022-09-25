# Caesura Style Guide

## Code Formatting

- All `.c` and `.h` files use tabs for indentation.
- Line comments (`// ...`) can only be preceded by whitespace on the same line.
- Block comments (`/* ... */`) can only be preceded or followed by whitespace on the same line.
- A line that opens or closes a block comment either contains the whole comment or is composed
    of exactly one of `/*` or `*/`.
- A comment refers to the set of contiguous code lines that directly follow it.

```c
/// allowed:

// comment
// comment
statement();

/* comment */
statement1();
statement2();

/*
    comment
    comment
*/
statement();

/// disallowed:

statement(); // comment
statement(); /* comment */

statement();
/* comment */

/* comment
   comment */
statement();
```

- Any block statement may close on the same line is opened on only if it contains exactly one
    statement.
- When an `if` or `for` keyword introduces a block that spans multiple lines, the opening `{`
    must be the last token of the line, and the closing `}` must be the first token on a new line.
- `{` and `}` should be separated from any other token by at least one whitespace character.

```c
/// allowed:
{ statement(); }
{
    statement1();
    statement2();
}

/// disallowed:
{statement();}
{ statement1(); statement2(); }
{ statement1();
  statement2(); }
```

- All `if`, `else`, and `for` keywords must be followed by a block opened on the same line as the
    closing parenthesis of the condition for `if` and `for`, or on the same line as the keyword
    itself for `else`.
- `else` should directly follow the closing `}` of the respective `if` block when this spans
    multiple lines.

```c
/// allowed:

if (condition) { statement1(); }
else {statement2()}

if (condition) {
    statement1();
    statement2();
}

if (condition) {
    statement1();
    statement2();
} else {
    statement3();
}

/// disallowed:

if (condition) statement1();
else statement2();

if (condition) { statement1(); statement2(); }

if (condition)
{
    statement1();
    statement2();
}
else
{
    statement3();
}
```

## Loops and iteration
- `while` loops are prohibited. Use `for(; condition;)` in their place.
- Do not use pointers as iterators when iterating arrays, prefer indices.
- When iterating over positive indices, prefer `u64` for the iteration variable.
- Use pre-increment over post-increment when the previous value is discarded.
- No raw loops: prefer functions that contain exactly one loop or no loops at all.

## Switch statements
- When enumerating cases, use the `break;case ... :` pattern to prevent unwanted fallthrough.
- When fallthrough is the desired behaviour, indicate so with the `[[fallthrough]];case ... :`
    pattern (allowed since the C23 standard)

## Naming
- Function names, local variable names and struct field names are in `snake_case`.
- Type names and enum variants are in `Pascal_Snake_Case`.
- enum variants are prefixed with the name of the type they belong to.
