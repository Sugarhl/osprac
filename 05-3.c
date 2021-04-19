#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

/* В родителе бежим по циклу и после прочтения провводим над семафором A(S,2) 
 * далее перед чтением Z(S) и читаем 
 * В сыне же перед перед чтением D(S,1) читаем пишем и снова D(S,1) и эти дейстивя тоже повторяем в циклике.
 *
 * Пока родитель чего-то не запишет, у сына будет недостаточно большое значение чтобы прочесть 
 * и пока сын не прочтет и не ответит, у отца будет значение больше нуля и он не сможет прочитать.
*/
int main()
{
  int     fd[2], result;
  int     n, key;
  size_t size;
  char  pathname[] = "05-3.c"; 
  char mes[7];
  struct sembuf mybuf;
  int    semid;

  printf("Ведите количество сообщений между родителем и сыном :");
  scanf("%d", &n);

  if ((key = ftok(pathname,0)) < 0) {
      printf("Can\'t generate key\n");
      exit(-1);
  }

  if (pipe(fd) < 0) {
      printf("Can\'t open pipe\n");
      exit(-1);
  }

  result = fork();

  if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
      if (errno != EEXIST) {
          printf("Can\'t create semaphore set\n");
          exit(-1);
      } else {
          if ((semid = semget(key, 1, 0)) < 0) {
              printf("Can\'t find semaphore set\n");
              exit(-1);
          }
      }
  } 
 
  if (result < 0) {
      printf("Can\'t fork child\n");
      exit(-1);
  } else if (result > 0) {

   /* Parent process */
    int k = 0;
    while( --n>=0 ){
        
        size = write(fd[1], &k, sizeof(k));
        if (size < sizeof(k)) {
            printf("Can\'t write all string to pipe\n");
            exit(-1);
        }

        mybuf.sem_num = 0;
        mybuf.sem_op  = 2;
        mybuf.sem_flg = 0;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Can\'t add 1 to semaphore\n");
            exit(-1);
        }

        mybuf.sem_num = 0;
        mybuf.sem_op  = 0;
        mybuf.sem_flg = 0;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        size = read(fd[0], &k, sizeof(k));

        if (size < 0) {
          printf("Can\'t read string from pipe\n");
          exit(-1);
        }

        printf("Message: %d received to parent.\n", k++);

    }

    if (close(fd[1]) < 0) {
        printf("parent: Can\'t close writing side of pipe\n"); exit(-1);
    }

    if (close(fd[0]) < 0) {
        printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
    }
    printf("Parent exit\n");

  } else {

    /* Child process */
    int k = 0;
    while(--n>=0){

        mybuf.sem_num = 0;
        mybuf.sem_op  = -1;
        mybuf.sem_flg = 0;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        size = read(fd[0], &k, sizeof(k));

        if (size < 0) {
          printf("Can\'t read string from pipe\n");
          exit(-1);
        }

        printf("Message: %d received to child.\n", k++);

        size = write(fd[1], &k, sizeof(k));

        if (size < sizeof(k)) {
            printf("Can\'t write all string to pipe\n");
            exit(-1);
        }

        mybuf.sem_num = 0;
        mybuf.sem_op  = -1;
        mybuf.sem_flg = 0;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Can\'t add 1 to semaphore\n");
            exit(-1);
        }
    }

    printf("Child exit\n");
    if (close(fd[0]) < 0) {
        printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }

    if (close(fd[1]) < 0) {
        printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }
  }

  return 0;
}
