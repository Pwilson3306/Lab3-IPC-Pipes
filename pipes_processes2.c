#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
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

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd1);

  pid1 = fork();

  if (pid1 == 0)
    {
      pipe(pipefd2);

      pid2 =fork();
      if (pid2 == 0)
      {
        dup2(pipefd2[0], 0);

        close(pipefd2[1]);
        close(pipefd1[0]);
        close(pipefd1[1]);

        execvp("sort", sort_args);

      
      // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe

      

      // close unused hald of pipe


      // execute grep

      
    }
  else
    {
      dup2(pipefd1[0], 0);

      dup2(pipefd2[1], 1);

      close(pipefd1[1]);
      close(pipefd2[0]);

      execvp("grep", grep_args);
    }
  }
  else
  {
    dup2(pipefd1[1], 1);

    close(pipefd1[0]);

    execvp("cat", cat_args);
  }

  return 0;
}
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

    

      // close unused unput half of pipe

    

      // execute cat

      