char* expand(char* str) {
  char pid[16] = {0};
  sprintf(pid, "%d", getpid());

  char *front = calloc(strlen(str) + 1, sizeof(char));
  char *back = calloc(strlen(str) + 1, sizeof(char));
  int i, j;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] == '$') {
      j = i + 2;
      break;
    }
  }
  strncpy(front, str, i);
  strncpy(back, str + j, strlen(str) - j);
  char *final = calloc(strlen(front) + strlen(pid) + strlen(back), sizeof(char));
  sprintf(final, "%s%s%s", front, pid, back);
  return final;
}

void change_dir(char **args) {
  if (args[1] == NULL) args[1] = getenv("HOME");
  if (chdir(args[1]) == -1) printf("Could not change directory");
}

void external_fg(char **args, char *in_file, char *out_file) {
  pid_t pid = fork();
  if (pid == 0) {
    // only runs in child proccess

    // redirect input / output using dup2

    execvp(args[0], args);
    perror("execv");   /* execve() returns only on error */
	  exit(1);
  }
  else {
    waitpid(pid, &status, 0);
  }
}

void external_bg(char **args, char *in_file, char *out_file) {
  pid_t child_pid = fork();
  if (child_pid == -1) {
    perror("fork() failed!");
    exit(1);
  }
  else if (child_pid == 0) {
    // only runis in child
    // redirect i/o using dup(2), if not specified, redirect to dev/null

    execvp(args[0], args);
    perror("execvp failed!");
    exit(1);
  }
  else {
    int *child_status;
    waitpid(child_pid, child_status, WNOHANG);
    proc_arr[num_proc] = child_pid;
    num_proc++;
    printf("background process pid: %d\n", child_pid);
  }
}


void handle_command(char** args, int num_args, char* in_file, char* out_file) {
  if (strcmp(args[0], "\n") == 0 || args[0][0] == '#') return;
  if (strcmp(args[0], "cd") == 0) change_dir(args);
  else if (strcmp(args[0], "status") == 0) {
    if (WIFEXITED(status)) printf("Last foreground process exited normally with status %d\n", WEXITSTATUS(status));
    else printf("Last foreground process exited abnormally due to signal %d\n", WTERMSIG(status));
  }
  else if (strcmp(args[num_args - 1], "&") == 0) {
    free(args[num_args-1]);
    args[num_args-1] = NULL;
    external_bg(args, in_file, out_file);
  }
  else {
    external_fg(args, in_file, out_file);
  }
}

void tokenize_commands(char** arg_array, char* line) {
  char* in_file = NULL;
  char* out_file = NULL;
  char *token = strtok(line, " ");

  int i = 0;
  while (token != NULL) {
    //provies var $$ expansion
    if (strstr(token, "$$") != NULL) {
      token = expand(token);
    }
    if (strcmp(token, "<") == 0) {
      token = strtok(NULL, " ");
      in_file = token;
      token = strtok(NULL, " ");
      continue;
    }
    if (strcmp(token, ">") == 0) {
      token = strtok(NULL, " ");
      out_file = token;
      token = strtok(NULL, " ");
      continue;
    }

    arg_array[i] = calloc(strlen(token) + 1, sizeof(char));
    strcpy(arg_array[i], token);
    token = strtok(NULL, " ");
    i++;
  }
  handle_command(arg_array, i, in_file, out_file);
}

void check_children_status(pid_t *child_array, int *num_children) {
  for (int i = 0; i < *num_children; i++) {
    int *status;
    if (waitpid(child_array[i], status, WNOHANG) != 0) {
      if (WIFEXITED(status)) {
        printf("background process %d exited normally with status %d\n", child_array[i], WEXITSTATUS(status));
      }
      else {
        printf("Background process %d exited abnormally due to signal %d\n", child_array[i], WTERMSIG(status));
      }
      for (int j = i; j < *num_children; j++) {
        child_array[j] = child_array[j+1];
      }
      (*num_children)--;
    }
  }
}
