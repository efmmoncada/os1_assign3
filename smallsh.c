#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int status = 0;
pid_t proc_arr[200] = {0};
int num_proc = 0;

#include "smallsh.h"

#define PROMPT ":"

// still have to do signal handleres

int main(int argc, char const *argv[]) {

  char input[2049] = {0};


  while (1) {

    check_children_status(proc_arr, &num_proc);
    printf("%s", PROMPT);
    fflush(stdout);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
      char *token = strtok(input, "\n");
      if (strcmp(token, "exit") == 0) {
        // clean up and kill processes created, then exit;
        return 0;
      } else {
        char *args[512] = {0};
        tokenize_commands(args, token);
      }
    }
  }

}
