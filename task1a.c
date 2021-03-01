#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
const int bufsize = 30000;
int main()
{
  char* buf;
  char* path = "task1a.c"; 
  int     shmid;
  char    pathname[] = "task1a.c";
  key_t   key;

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, bufsize*sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    }
  }

  if ((buf = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }
  FILE* input = fopen(path, "rb");

  if(input == NULL){
      printf("Can't open file");
      exit(-1);
  }

  fseek(input, 0, SEEK_END);
  size_t filesize = ftell(input); 
  rewind(input);

  size_t read = fread(buf, sizeof(char), filesize, input);
  if(read != filesize){
    fclose(input);
    printf("Cant read file");
    exit(-1);
  }

  printf("Program prints its source text");

  if (shmdt(buf) < 0) {
    printf("Can't detach shared memory\n");
    fclose(input);
    exit(-1);
  }

  fclose(input);
  return 0;
}
