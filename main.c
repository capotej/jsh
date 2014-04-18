#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* exit */
#include <unistd.h>    /* _exit, fork */
#include <string.h>
#include <readline/readline.h>

static char *line = (char *)NULL;

char *read_cmd()
{
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

void fork_failed()
{
  perror("fork failed");
  exit(EXIT_FAILURE);
}

void tokenize_args(char *cmd, char **toks)
{
  int i = 0;
  char *tok = strtok(cmd, " ");

  while (tok != NULL) {
    if(i == 0){
      toks[i] = tok;
    } else {
      toks[i] = tok;
    }
    i++;
    tok = strtok(NULL, " ");
  }
  toks[i + 1] = NULL;
}


void fork_and_execute_cmd(char **argv){
  pid_t pid;
  pid = fork();
  int status;

  if (pid == -1) {
    fork_failed();
  } else if (pid == 0) {
    status = execvp(argv[0], argv);
    _exit(status);
  } else {
    int status;
    (void)waitpid(pid, &status, 0);
  }

}

void handle_cmd(char *cmd)
{
  char *argv[256];
  tokenize_args(cmd, argv);
  fork_and_execute_cmd(argv);
}

int main() {
  printf("starting jsh\n");

  do {
    char *cmd = read_cmd();
    handle_cmd(cmd);
  } while (line != NULL);

  exit(EXIT_SUCCESS);
}
