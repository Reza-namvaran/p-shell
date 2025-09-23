#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdio.h>
#include <unistd.h>

// Built-in commands
int psh_cd(char **args);
int psh_help(char **args);
int psh_exit(char **args);

// Array of builtin names and funcs
extern char *builtin_str[];
extern int (*builtin_funcs[])(char **);

int num_builtins(void);

#endif
