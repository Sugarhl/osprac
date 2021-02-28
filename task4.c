#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
  int     fd, result;

  size_t  size;
  char    resstring[14];
  char    name[] = "aaa.fifo";

  (void)umask(0);

  if (mknod(name, S_IFIFO | 0666, 0) < 0) {
    printf("Can\'t create FIFO\n");
    exit(-1);
  }

  if ((result = fork()) < 0) {
    printf("Can\t fork child\n");
    exit(-1);
  } else if (result > 0) {
    execle("write.out", "write.out", (char)0, envp);
  } else { 
    execle("read.out", "read.out", (char)0, envp);
  }
  return 0;
}
