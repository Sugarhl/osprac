#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[]="11-1a-task2.c";
  key_t  key;
  int len, maxlen;

  struct mymsgbuf
  {
    long mtype;
    struct {
        int num;
        char mtext[81];
        float weight;
    } info;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }
  maxlen = sizeof(mybuf.info) + 10; 

  for (int i = 1; i <= 5; i++) {
    int rcvlen = -1;
    while (rcvlen < 0){

        if (( rcvlen = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
        }
        else{
            printf("B recive message:\nmessage type = %ld, info:\n\t number = %d,\n\t text = %s \n\t weight = %f\n",
                    mybuf.mtype, mybuf.info.num,mybuf.info.mtext,mybuf.info.weight);
            rcvlen++;
        }
    }

    mybuf.mtype = 2;
    strcpy(mybuf.info.mtext, "This is text message from B");
    mybuf.info.num = i;
    mybuf.info.weight = i * 237.34;
    len =sizeof(mybuf.info)+1;

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
    printf("B send message\n");
  }

  return 0;
}
