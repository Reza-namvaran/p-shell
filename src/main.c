#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void shell_loop() {
    char *line = NULL;
    size_t buff_size = 0;

    while (1) {
        printf("\u03c0> ");

        if (getline(&line, &buff_size, stdin) == -1) {
            if (feof(stdin)) {
                printf("Exiting shell.\n");
                break;
            } else {
                perror("getline fail in loop\n");
                exit(EXIT_FAILURE);
            }
        }

        printf("User typed: %s", line);

        free(line);
        line = NULL;
    }
}

int main(void) {
    shell_loop();
    return EXIT_SUCCESS;
}