# Shellforge

A Unix-style command-line shell written in C.

Shellforge is a systems programming project that explores how a basic Unix shell works, including command parsing, built-in commands, command execution, and command history.

## Features

- Command-line interface
- External command execution
- Built-in commands
- `cd` support
- Command history
- GNU Readline support
- Modular C source structure

## Technologies

- C
- Linux
- GCC
- Make
- GNU Readline
- Git

## Project Structure

```text
shellforge/
├── include/
│   └── Header files
├── src/
│   └── Source files
├── builtin.c
├── builtin.h
├── executor.c
├── executor.h
├── expand.c
├── expand.h
├── lexer.c
├── lexer.h
├── main.c
├── makefile
└── README.md
