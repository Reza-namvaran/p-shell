#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define TOKEN_DELIMITERS " \t\r\n\a"
#define PROMPT "\u03c0> "

// Main shell loop
void shell_loop(void);

#endif
