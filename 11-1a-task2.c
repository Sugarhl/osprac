#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
  int msqid;      // IPC descriptor for the message queue
  char pathname[]="11-1a-task2.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
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

  /* Send information */

  for (i = 1; i <= 5; i++) {

    mybuf.mtype = 1;
    strcpy(mybuf.info.mtext, "This is text message from A");
    mybuf.info.num = i;
    mybuf.info.weight = i * 237.34;
    len =sizeof(mybuf.info)+1;

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
    printf("A send message\n");
    int rcvlen = -1;
  while (rcvlen < 0){

    int maxlen =sizeof(mybuf.info)+10;

        if (( rcvlen = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0)) < 0) {
        printf("Can\'t receive message from queue\n");
        exit(-1);
        }
        else{
            printf("A recive message:\nmessage type = %ld, info:\n\t number = %d,\n\t text = %s \n\t weight = %f\n",
                    mybuf.mtype, mybuf.info.num,mybuf.info.mtext,mybuf.info.weight);
            rcvlen++;
     }
    }
  }

  return 0;
}
