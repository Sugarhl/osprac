#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(void)
{
    int msqid;      // IPC descriptor for the message queue
    char pathname[]="11-task3-serv.c"; // The file name used to generate the key.
    // A file with this name must exist in the current directory.
    key_t  key;     // IPC key
    int i,len;      // Cycle counter and the length of the informative part of the message

    struct killermsgbuf
    {
        long mtype;
    } sndbuf;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid \n");
        exit(-1);
    }


    sndbuf.mtype = 2;
    if (msgsnd(msqid, (struct msgbuf *)&sndbuf, 0, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Send stop message");


    return 0;
}
