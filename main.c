#include <stdio.h>
#include <readline/readline.h>

static char *line = (char *)NULL;

char *read_cmd(){
  if (line) {
    free(line);
    line = (char *)NULL;
  }

  line = readline("$ ");

  if (line && *line) {
    add_history(line);
  }

  return (line);
}

int main() {
  printf("starting shell\n");

  do {
    char *cmd = read_cmd();
    printf("cmd: %s\n", cmd);
  } while (line != NULL);

}
