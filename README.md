# Language (`.lang`)

# Background

The goal of this langauge is to allow for the build up highly nested types, and to enforce relationships between. This is particularly useful for serialising data, handling complicated `HTTP` response data, and highly interconnected data.

# Primitives

The primitves aim to be informative to ensure that there is no ambiguity on how the primitive will function.

| Keyword   | Full Name                | Size (bytes) | Range                                                                               |
| --------- | ------------------------ | ------------ | ----------------------------------------------------------------------------------- |
| `uint8`   | Unsigned Integer 8-bits  | `1`          | $0$ to $255$                                                                        |
| `uint16`  | Unsigned Integer 16-bits | `2`          | $0$ to $65535$                                                                      |
| `uint32`  | Unsigned Integer 32-bits | `4`          | $0$ to $2 ^ {32} - 1$                                                               |
| `uint64`  | Unsigned Integer 64-bits | `8`          | $0$ to $2 ^ {64} - 1$                                                               |
| `float16` | Signed Float 16-bits     | `2`          | $-65504$ to $65504$                                                                 |
| `float32` | Signed Float 32-bits     | `4`          | $\approx -3.4028234664 \times 10^{38}$ to $\approx 3.4028234664 × 10^{38}$          |
| `float64` | Signed Float 64-bits     | `8`          | $\approx  -1.7976931348 \times 10^{308}$ to $\approx  1.7976931348 \times 10^{308}$ |

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

| Token Name    | Token Type Enum | Description                          |
| ------------- | --------------- | ------------------------------------ |
| End of line   | `END_OF_LINE`   | Eend of a statement                  |
| Operator      | `OPERATOR`      | Parent class of an operator type     |
| Open Bracket  | `BRACKET_CLOSE` | A `(` bracket                        |
| Close Bracket | `BRACKET_OPEN`  | A `)` bracket                        |
| Other         | `OTHER`         | Stores a string for any unknown type |

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
