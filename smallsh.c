#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "smallsh.h"

#define PROMPT ": "


int main(int argc, char const *argv[]) {
  char pwd[2049] = {0};

  char input[2049] = {0};


  while (1) {
    printf("%s %s", pwd, PROMPT);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "exit\n") == 0) return 0;

    handle_command(input);

  }

}
