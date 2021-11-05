#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

// global vars to track status of last process and child prcesses.
int status = 0;
pid_t proc_arr[200] = {0};
int num_proc = 0;

// global var to use wtih SIGTSTP
int foreground_only = 0;

#define TRASH "/dev/null"
#define PROMPT ": "

#include "smallsh.h"

int main(int argc, char const *argv[]) {
  // signal handlers fof parent
  struct sigaction SIGINT_action = {0};
  SIGINT_action.sa_handler = SIG_IGN;
  sigfillset(&SIGINT_action.sa_mask);
  SIGINT_action.sa_flags = SA_RESTART;
  sigaction(SIGINT, &SIGINT_action, NULL);


  struct sigaction SIGTSTP_action = {0};
  SIGTSTP_action.sa_handler = handle_SIGTSTP;
  sigfillset(&SIGTSTP_action.sa_mask);
  SIGTSTP_action.sa_flags = SA_RESTART;
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);


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
        if (num_proc != 0) {
          for (int i = 0; i < num_proc; i++) {
            kill(proc_arr[i], SIGKILL);
          }
          // clean mem allocd
          //...
        }
        return 0;
      } else {
        char *args[512] = {0};
        tokenize_commands(args, token);
      }
    }
  }
}
