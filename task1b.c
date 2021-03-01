#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

const int bufsize = 30000;
int main()
{
  char *buf;
  int     shmid; 
  int     new = 1;
  char    pathname[] = "task1a.c";
                                
  key_t   key;       

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, bufsize*sizeof(char), 0)) < 0) {
    printf("Can\'t find shared memory\n");
    exit(-1);
  }

  if ((buf = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  printf("Source text of %s: \n%s", pathname, buf);

  if (shmdt(buf) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  if(shmctl(shmid, IPC_RMID, NULL)){
      printf("Cant delete shared memory");
      exit(-1); 
  }

  return 0;
}
