#include "executor.h"
#include "builtin.h"

int psh_lunch(char **args, char *infile, char *outfile, int append) {
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
        // child process

        if (infile) {
            int fd_in = open(infile, O_RDONLY);
            if (fd_in < 0) {
                perror("psh in-file");
                exit(EXIT_FAILURE);
            }
        
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        if (outfile) {
            int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
            int fd_out = open(outfile, flags, 0644);
            if (fd_out < 0) {
                perror("psh: fd_out");
                exit(EXIT_FAILURE);
            }

            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }



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

int psh_execute_and_redirect(char **args) {
    char *infile = NULL;
    char *outfile = NULL;
    int append = 0;

    // Create a new array for command and arguments only
    char **exec_args = malloc(sizeof(char*) * 64);
    if (!exec_args) {
        perror("allocation error");
        exit(EXIT_FAILURE);
    }
    int exec_pos = 0;

    /// TODO: Make this faster
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            if (args[i + 1] == NULL) {
                fprintf(stderr, "psh: syntax error: missing filename for redirection\n");
                free(exec_args);
                return 1;
            }
            outfile = args[i + 1];
            i++;
            continue;
        }
    
        if (strcmp(args[i], "<") == 0) {
            if (args[i+1] == NULL) {
                fprintf(stderr, "psh: syntax error: missing filename for redirection\n");
                free(exec_args);
                return 1;
            }

            infile = args[i + 1];
            i++;
            continue;
        }

        exec_args[exec_pos++] = args[i];
    }
    exec_args[exec_pos] = NULL;

    // If there are no arguments for the command, return
    if (exec_args[0] == NULL) {
      free(exec_args);
      return 1;
    }

    // Launch the command with the parsed redirection options
    int status = psh_lunch(exec_args, infile, outfile, append);

    free(exec_args);
    return status;
}

int psh_execute(char **args) {
    if (args[0] == NULL) {
        return 1; // Empty command
    }

    /// TODO: Change to fast lookup  
    // Loop through the built-in command names
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_funcs[i])(args);
        }
    }

    // If not builtin lunch in a separate process
    return psh_execute_and_redirect(args);
}