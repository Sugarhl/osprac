#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fdp[2], fds[2], result;

  size_t ssize, psize;
  char  presstring[16], sresstring[16];

  if (pipe(fdp) < 0) {
    printf("Can\'t open parent pipe\n");
    exit(-1);
  }

  if (pipe(fds) < 0) {
    printf("Can\'t open child pipe\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

    if (close(fdp[0]) < 0) {
      printf("parent: Can\'t close reading side of parent pipe\n"); exit(-1);
    }
    if (close(fds[1]) < 0) {
      printf("parent: Can\'t close writing side of son pipe\n"); exit(-1);
    }

    psize = write(fdp[1], "Hello from dad!", 16);

    if (psize != 16) {
      printf("Can\'t write all string to parent pipe\n"); exit(-1);
    }

    sleep(1);

    psize = read(fds[0], presstring, 16);

    if (psize < 0) {
      printf("Can\'t read string from pipe\n"); exit(-1);
    }

    printf("Parent exit, resstring:%s\n", presstring);

    if (close(fdp[1]) < 0) {
      printf("parent: Can\'t close writing side of parent pipe\n"); exit(-1);
    }

    if (close(fds[0]) < 0) {
      printf("child: Can\'t close reading side of son pipe\n"); exit(-1);
    }

  } else {

    /* Child process */

    if (close(fdp[1]) < 0) {
      printf("child: Can\'t close writing side of parent pipe\n"); exit(-1);
    }
    if (close(fds[0]) < 0) {
      printf("child: Can\'t close reading side of son pipe\n"); exit(-1);
    }

    ssize = write(fds[1], "Hello from son!", 16);

    if (ssize != 16) {
      printf("Can\'t write all string to son pipe\n"); exit(-1);
    }
    
    sleep(1);

    ssize = read(fdp[0], sresstring, 16);

    if (ssize < 0) {
      printf("Can\'t read string from pipe\n"); exit(-1);
    }

    printf("Child exit, resstring:%s\n", sresstring);
    
    if (close(fds[1]) < 0) {
      printf("parent: Can\'t close writing side of son pipe\n"); exit(-1);
    }

    if (close(fdp[0]) < 0) {
      printf("child: Can\'t close reading side of parent pipe\n"); exit(-1);
    }
  }

  return 0;
}
