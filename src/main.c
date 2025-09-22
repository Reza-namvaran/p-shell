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

void shell_loop(void) {
    char *line = NULL;
    size_t buff_size = 0;
    char **args;

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

        printf("Tokens:\n");
        for (int i = 0; args[i] != NULL; i++) {
            printf(" - %s\n", args[i]);
        }

        free(line);
        free(args);
        line = NULL;
        args = NULL;
    }
}

int main(void) {
    shell_loop();
    return EXIT_SUCCESS;
}