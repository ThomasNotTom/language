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

The language uses a `C`-style variable declaration. Namely the form `{type} {identifier} = {value};`.
For example:

```
uint8 a = 10;
uint8 b = 10;
```

## Returning

Values can be returned with the `return` keyword.
For example:

```
uint8 a = 1;
return a;
```

## Examples

Examples can be found in the `/examples` folder.

# Compilation Structure
