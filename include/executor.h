#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Runs command (builtin or external)
int psh_execute(char **args);

// Handles external execution + redirection
int psh_execute_and_redirect(char **args);

// Spawns a process
int psh_lunch(char **args, char *infile, char *outfile, int append);

#endif
