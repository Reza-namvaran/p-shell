#include "shell.h"
#include "parser.h"
#include "executor.h"

void shell_loop(void) {
    char *line = NULL;
    size_t buff_size = 0;
    char **args;
    int status = 1;

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

        status = psh_execute(args);

        if (status == 0) {
            free(line);
            free(args);
            line = NULL;
            args = NULL;
        }
    }
}
