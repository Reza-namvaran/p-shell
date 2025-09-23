#include "parser.h"

#define TOKEN_DELIMITERS " \t\r\n\a"

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
