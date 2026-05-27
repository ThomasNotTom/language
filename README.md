# Language (`.lang`)

# Background

The goal of this langauge is to allow for the build up highly nested types, and to enforce relationships between. This is particularly useful for serialising data, handling complicated `HTTP` response data, and highly interconnected data.

# Primitives

The primitves aim to be informative to ensure that there is no ambiguity on how the primitive will function.

| Keyword | Full Name               | Size     |
| ------- | ----------------------- | -------- |
| `uint8` | Unsigned Integer 8-bits | `1 byte` |

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

## Returning

Values can be returned with the `return` keyword.
For example:

```
uint8 a = 1;
return a;
```

## Arithmetic

Values can be defined as a series of arithemtic statements, combining identifiers and values:

```
uint8 a = 1;
uint8 b = a + 1;
uint8 c = a + b + 1;
```

## Examples

Examples can be found in the `/examples` folder.

# Compilation Structure

## File Reader

The file is opened and read in it's entirity to an `std::string`.

## Lexer

String segments are converted to a list of tokens. Representing the smallest segment of a statement. Tokens can be inherited by sub-tokens, to add more specificity.

| Token Name  | Token Type Enum | Description                      |
| ----------- | --------------- | -------------------------------- |
| End of line | `END_OF_LINE`   | Eend of a statement              |
| Primitive   | `PRIMITIVE`     | Parent class of a primitive type |
| Operator.   | `OPERATOR`      | Parent class of an operator type |
| Identifier  | `IDENTIFIER`    | Variable identifier              |
| Number      | `NUMBER`        | Integer                          |
| Return      | `RETURN`        | Returns a value from a function  |

## Syntax Analyser

Combines tokens into statements.

| Statement Name | Statement Type Enum | Description                                          |
| -------------- | ------------------- | ---------------------------------------------------- |
| Initialisation | `INITIALISATION`    | Initialises a variable with the given primitive type |
| Assignment     | `ASSIGNMENT`        | Sets a variable to a value                           |
| Return         | `RETURN`            | Returns a value from a function                      |
| Addition       | `ADDITION`          | Adds two values and assigns them to an identifier    |

## `LLVM` Intermediate Representation

The statements are then converted into `LLVM` intermediate representation (IR). Each statement translates almost-directly into a line in the `LLVM IR`, and so the primary technical content of this stage is keeping track of identifiers and using them where needed.

## Compilation

The `LLVM IR` is compiled into a `.o` file, which is then compiled using `clang` to create a finished `main.out` file.
