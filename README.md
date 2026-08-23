# Language (`.lang`)

# Background

## Summary

The goal of this langauge is to allow for the build up highly nested types, and to enforce relationships between. This is particularly useful for serialising data, handling complicated `HTTP` response data, and highly interconnected data.

## Constraints

The langauge uses `constraint`s to add a layer above standard type checking. A constraint acts like the domain of a variable.

The syntax for an unknown value of a variable will be using a `?`.

```
uint8 a = ?;
a; # constraints(a == ?)
```

A constraint can be applied by passing the variable through an `if` statement.

```

uint8 a = ?;
a; # constraints(a == ?)

if (a >= 5) {
  break;
}

a; # constraints(a < 5)

```

Unnecessary constraint checks are treated as a compile-time error.

```

uint8 a = ?;
a; # constraints(a == ?)

if (a >= 5) {
  break;
}

if (a >= 5) { # Error: validating preexisting constraint
  break;
}

if (a >= 4) { # Error: validating constraint implied by preexisting constraint
  break;
}

```

Constraints on variables will be adjusted as they are manipulated.

```

uint8 a = ?;
a; # constraints(a == ?)

if (a >= 5) {
  break;
}
a; # constrains(a < 4)

a += 1;

a; # constrains(a < 5)

```

Constraints become slightly more difficult for loops as the exact constraints may vary for each iteration. During the loop the value `N` is used to adjust the constraint of values as the loop goes on.

The value of some variables can be inferred by the collapsed constraints of another variable. For example for each iteration `i` may be incremented by `1` and `j` may be incremented by `2`. If `i` were to be measured as `3` then the value of `j` could be implied to be `6`.

# Primitives

The primitves aim to be informative to ensure that there is no ambiguity on how the primitive will function.

| Keyword  | Full Name                | Size (bytes) |
| -------- | ------------------------ | ------------ |
| `uint8`  | Unsigned Integer 8-bits  | `1`          |
| `uint16` | Unsigned Integer 16-bits | `2`          |
| `uint32` | Unsigned Integer 32-bits | `4`          |
| `uint64` | Unsigned Integer 64-bits | `8`          |

# Syntax

## Variable Declaration

### Basic Assignment

The language uses a `C`-style variable declaration. Namely the form `{type} {identifier} = {value};`.
For example:

```

uint8 a = 10;

```

### Assignment From Variable

Variables can also be assigned a value from another variable.

```

uint8 a = 10;
uint8 b = a;

```

## Arithmetic

Values can be defined as a series of arithmetic statements, combining identifiers and values:

```

uint8 a = 1;
uint8 b = a + 1;
uint8 c = a + b + 1;

uint8 d = c - b;
uint8 e = d - 1;

```

## Printing

Values can be printed to standard out using the print function, proceeded by the value wanted to print.

```

print(1);

uint8 a = 2;
print(a);

```

## Returning

Values can be returned with the `return` function.
For example:

```

uint8 a = 1;
return(a);

```

## Examples

Examples can be found in the `/examples` folder.

# Compilation Structure

## File Reader

The file is opened and read in it's entirity to an `std::string`.

## Lexer

String segments are converted to a list of tokens. Representing the smallest segment of a statement. Tokens can be inherited by sub-tokens, to add more specificity.

| Token Name           | Token Type Enum        | Description                          |
| -------------------- | ---------------------- | ------------------------------------ |
| End of line          | `END_OF_LINE`          | Eend of a statement                  |
| Operator             | `OPERATOR`             | Parent class of an operator type     |
| Open Normal Bracket  | `BRACKET_NORMAL_CLOSE` | A `(` bracket                        |
| Close Normal Bracket | `BRACKET_NORMAL_OPEN`  | A `)` bracket                        |
| Other                | `OTHER`                | Stores a string for any unknown type |

## Syntax Analyser

Combines tokens into statements.

| Statement Name | Statement Type Enum | Description                                            |
| -------------- | ------------------- | ------------------------------------------------------ |
| Initialisation | `INITIALISATION`    | Initialises a variable with the given primitive type   |
| Assignment     | `ASSIGNMENT`        | Sets a variable to a value                             |
| Addition       | `ADDITION`          | Adds two values and assigns them to an identifier      |
| Subtraction    | `SUBTRACTION`       | Subtracts two values and assigns them to an identifier |
| Function Call  | `FUNCTION_CALL`     | Calls a function with any number of parameters         |

## `LLVM` Intermediate Representation

The statements are then converted into `LLVM` Intermediate Representation (`LLVM IR`). Each statement translates almost-directly into a line in the `LLVM IR`, and so the primary technical content of this stage is keeping track of identifiers and using them where needed.

## Compilation

The `LLVM IR` is compiled into an `output.o` file, which is then compiled using `clang` to create a finished `main.out` executeable.

```

```

```

```
