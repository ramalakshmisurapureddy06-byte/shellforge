#ifndef BUILTIN_H
#define BUILTIN_H

#include "parser.h"


/*
 * Check whether a command is a built-in command.
 *
 * Built-in commands:
 *      cd
 *      pwd
 *      echo
 *      clear
 *      help
 *      exit
 *
 * Returns:
 *      1 -> built-in command
 *      0 -> external command
 */
int is_builtin(const command_t *cmd);


/*
 * Execute a built-in command.
 *
 * Supported commands:
 *      cd     -> Change directory
 *      pwd    -> Show current directory
 *      echo   -> Display text
 *      clear  -> Clear terminal screen
 *      help   -> Show available commands
 *      exit   -> Exit the shell
 *
 * Returns:
 *      0  -> command executed successfully
 *      1  -> shell should exit
 *     -1  -> error
 */
int execute_builtin(command_t *cmd);

#endif
