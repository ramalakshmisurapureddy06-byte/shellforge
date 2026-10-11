# Shellforge

A Unix-style command-line shell written in C.

Shellforge is a systems programming project that explores how a basic Unix shell works, including command parsing, built-in commands, process execution, job control, and command history.

## Features

- Command-line interface
- External command execution
- Built-in commands:
  - `cd` — Change the current directory
  - `pwd` — Display the current directory
  - `echo` — Print text to the terminal
  - `exit` — Exit the shell
  - `jobs` — List background and suspended jobs
  - `fg` — Bring a job to the foreground
  - `bg` — Resume a job in the background
  - `mydate` — Display the current date and time
  - `myuser` — Display the current username
  - `mydir` — Display the current directory
  - `mysystem` — Display operating system and kernel information
- Command history
- GNU Readline support
- Process and job control
- Modular C source structure

## Technologies

- C
- Linux
- GCC
- Make
- GNU Readline
- Git

## Build and Run

Compile the project:

```bash
make -f makefile
```

Run Shellforge:

```bash
./shellforge
```

## Example Usage

```text
shellforge$ mydate
shellforge$ myuser
shellforge$ mydir
shellforge$ mysystem
shellforge$ echo Hello
```

## Project Structure

```text
shellforge/
├── include/
├── src/
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
```

## About

Shellforge is a hands-on operating systems project focused on understanding shell implementation, built-in commands, process execution, job control, and command history.
