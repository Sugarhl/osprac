#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(int argc, char **argv)
{
    pid_t pid;
    int msqid;      // IPC descriptor for the message queue
    char pathname[]="11-task3-serv.c"; // The file name used to generate the key.
    // A file with this name must exist in the current directory.
    key_t  key;     // IPC key
    int i,len;      // Cycle counter and the length of the informative part of the message

    struct clientmsgbuf
    {
        long mtype;
        struct {
            int pid;
            int num;
        } info;
    } clientbuf;

    struct servermsgbuf{
        int mtype;
        struct {
            int num;
        } info;
    } serverbuf;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    pid  = getpid();

    clientbuf.mtype = 1;
    clientbuf.info.pid = pid;
    printf("Type a number: ");
    if (scanf("%d", &clientbuf.info.num) != 1) {
        printf("Input error\n");
        exit(-1);
    }

    if (msgsnd(msqid,  (struct msgbuf *)&clientbuf, sizeof(clientbuf.info), 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    printf("Client %d send message to server: '%d'\n",clientbuf.info.pid, clientbuf.info.num);
    if (( len = msgrcv(msqid, (struct msgbuf *) &serverbuf, sizeof(serverbuf.info), pid, 0)) < 0) {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }

    printf("Client %d got message: '%d'\n", pid, serverbuf.info.num);

    return 0;
}

