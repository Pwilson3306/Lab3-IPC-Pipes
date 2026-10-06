#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep <argument> | sort".
 * In this quick-and-dirty implementation the parent doesn't wait
 * for the child to finish.
 *
 */

int main(int argc, char **argv)
{
  int pipefd1[2];
  int pipefd2[2];

  int pid1;
  int pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  if (argc != 2)
  {
    printf("Usage: %s <grep argument>\n", argv[0]);
    return 1;
  }

  // make a pipe
  pipe(pipefd1);

  pid1 = fork();

  if (pid1 == 0)
  {
    // child handles grep

    pipe(pipefd2);

    pid2 = fork();

    if (pid2 == 0)
    {
      // child's child handles sort

      dup2(pipefd2[0], 0);

      close(pipefd2[1]);
      close(pipefd1[0]);
      close(pipefd1[1]);

      execvp("sort", sort_args);
    }
    else
    {
      // child handles grep

      dup2(pipefd1[0], 0);

      dup2(pipefd2[1], 1);

      close(pipefd1[1]);
      close(pipefd2[0]);

      execvp("grep", grep_args);
    }
  }
  else
  {
    // parent gets here and handles "cat scores"

    // replace standard output with output part of pipe
    dup2(pipefd1[1], 1);

    // close unused input half of pipe
    close(pipefd1[0]);

    // execute cat
    execvp("cat", cat_args);
  }

  return 0;
}
