#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* exit */
#include <unistd.h>    /* _exit, fork */
#include <string.h>
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

int builtin(char *cmd){
  return 0;
}

void fork_failed() {
  perror("fork failed");
  exit(EXIT_FAILURE);
}

int main() {
  printf("starting shell\n");

  do {
    char *cmd = read_cmd();

    pid_t pid;
    pid = fork();

    if (pid == -1) {
      fork_failed();
    } else if (pid == 0) {
      char *tok = strtok(cmd, " ");
      char *toks[100];
      char *argv0;
      int i = 0;
      int status;

      while (tok != NULL) {
        if(i == 0){
          argv0 = tok;
          toks[i] = tok;
        } else {
          toks[i] = tok;
        }
        i++;
        tok = strtok(NULL, " ");
      }
      toks[i + 1] = NULL;
      // printf("argv0: %s\n", argv0);
      // printf("toks: %s\n", toks[0]);
      if (builtin(cmd) == 1) {
        printf("built in %s\n", cmd);
      } else {
        status = execvp(argv0, toks);
      }

      _exit(status);
    } else {
      /*
       * When fork() returns a positive number, we are in the parent process
       * and the return value is the PID of the newly created child process.
       */
      int status;
      (void)waitpid(pid, &status, 0);
    }

  } while (line != NULL);

  exit(EXIT_SUCCESS);
}
