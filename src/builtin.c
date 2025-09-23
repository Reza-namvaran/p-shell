#include "builtin.h"

char *builtin_str[]  = { "cd", "help", "exit"};

int (*builtin_funcs[]) (char **) = { &psh_cd, &psh_help, &psh_exit };

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int psh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "psh: Missing argument. Cannot switch dir");
    } else {
        if (chdir(args[1]) != 1) {
            perror("psh: cd status");
        }
    }

    return 1;
}

int psh_help(__attribute__((unused)) char **args) {
  printf("Welcome to P-Shell (psh)!\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built-in commands:\n");
  int num_builtins = sizeof(builtin_str) / sizeof(char *);
  for (int i = 0; i < num_builtins; i++) {
    printf("  - %s\n", builtin_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}


int psh_exit(__attribute__((unused)) char **args) {
    return 0;
}
