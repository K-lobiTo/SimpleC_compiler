# SimpleC Compiler

A simplified C compiler that demonstrates basic compiler construction techniques.

## Features

- **Preprocessing**: Handles `#include` and `#define` directives
- **Lexical Analysis**: Tokenizes input using lexer
- **Syntax Analysis**: Parses grammar using yacc
- **Scope Management**: Uses stack of tries for variable scoping
- **Code Generation**: Produces assembly output from AST

## How It Works

1. **Preprocess**: Expand includes and macros
2. **Tokenize**: Break code into tokens
3. **Parse**: Build AST using grammar rules
4. **Manage Scopes**: Handle variables with stack of tries
5. **Generate Code**: Produce assembly output

## Build

```bash
cd Klompiler
make
```

## Quick Start

```bash
# Compile your C code
./compiler your_c_code.c

# This generates the preprocessed file and starts the compilation process
# If compilation is successful, it generates assembly code
```

## Limitations

- **Libraries**: Does not support external libraries
- **Functions**: Does not support function declarations or calls
- **Preprocessor**: Supports only `#include` and `#define` directives

## References

Fischer, C. N., & LeBlanc, R. J. (2010). Crafting a Compiler with C. Addison-Wesley.

---

**Note**: This is an educational project demonstrating compiler design concepts and supports a limited subset of C syntax.
