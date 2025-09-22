#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TOKEN_DELIMITERS " \t\r\n\a"
#define PROMPT "\u03c0> "

char **split_lines(char *line) {
    int buff_size = 64;
    int position = 0;

    char **tokens = malloc(buff_size * sizeof(char*)); 
    char *token;

    while (!tokens) {
        perror("allocation for tokens fail\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMITERS);

    while (token != NULL) {
        tokens[position] = token;
        position++;

        // If exceeded the buffer size, reallocate the array
        if (position >= buff_size) {
            buff_size += 64;
            tokens = realloc(tokens, buff_size * sizeof(char*));

            if (!tokens) {
                perror("reallocation error in tokenizing");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    tokens[position] = NULL;
    return tokens;
}

int execute(char **args) {
    pid_t pid;
    int status;

    if (args[0] == NULL) {
        return 1;
    }
    if (strcmp(args[0], "exit") == 0) {
        return 0; // Signal the main loop to exit
    }

    // Create child process
    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Error occurred in forking");
        }

        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("Error forking");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void shell_loop(void) {
    char *line = NULL;
    size_t buff_size = 0;
    char **args;
    int status;

    while (1) {
        printf(PROMPT);

        if (getline(&line, &buff_size, stdin) == -1) {
            if (feof(stdin)) {
                printf("Exiting shell.\n");
                break;
            } else {
                perror("getline fail in loop\n");
                exit(EXIT_FAILURE);
            }
        }

        args = split_lines(line);

        if (args[0] == NULL) {
            free(line);
            free(args);
            line = NULL;
            args = NULL;
            continue;
        }

        status = execute(args);

        if (status = 0) {
            free(line);
            free(args);
            line = NULL;
            args = NULL;
        }
    }
}

int main(void) {
    shell_loop();
    return EXIT_SUCCESS;
}