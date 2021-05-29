#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
    char check = 1;
    if (argc == 2 && (strcmp(argv[1], "-f") == 0 || strcmp(argv[1],"--force") == 0)) {
        check = 0;
    } else if (argc != 1) {
        printf("Got incorrect arguments\n");
        exit(-1);
    }

    int msqid;
    char pathname[]="11-task3-serv.c";
    key_t  key;
    int len, maxlen;

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
        }info;
    } serverbuf;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Can\'t get msqid\n");
            exit(-1);
        } else if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get msqid\n");
            exit(-1);
        }
        if ((len = msgrcv(msqid, (struct serverbuf *) &serverbuf, 0, 3, IPC_NOWAIT)) < 0) {
            if (errno == ENOMSG) {
                if (check) {
                    printf("Can't run new server, because another already running or an error was made at startup");
                    exit(-1);
                }
            } else {
                printf("Can\'t receive message from queue\n");
                exit(-1);
            }
        }
    }

    while(1){
        if (( len = msgrcv(msqid, (struct msgbuf *) &clientbuf, sizeof(clientbuf.info), -2, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        printf("Server got message: msgtype=%ld\n", clientbuf.mtype);

        if (clientbuf.mtype == 1) {

            serverbuf.mtype = clientbuf.info.pid; 
            serverbuf.info.num = clientbuf.info.num * clientbuf.info.num;
            sleep(2);
            if (msgsnd(msqid,  (struct msgbuf *)&serverbuf, sizeof(serverbuf.info), 0) < 0) {
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
            printf("Server answered to client %d : %d\n", serverbuf.mtype, serverbuf.info.num);
        } else if (clientbuf.mtype == 2) {
            printf("Stopping server\n");
            break;
        }
    }

    serverbuf.mtype = 3;
    if (msgsnd(msqid, (struct msgbuf *) &serverbuf, 0, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Server stopped.");
}
