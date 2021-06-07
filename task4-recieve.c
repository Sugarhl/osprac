#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

char send = 1;
char curr_send = 1;
uint number = 0000000000;
uint mask = 0x80000000;
int cnt = 0;
pid_t snd_pid;

void my_handler(int nsig) {
    int bit=0;
    if (nsig == SIGUSR1) {
        bit=0;
    } else if (nsig == SIGUSR2) {
        bit=1;
    }
    if(cnt==0)
        number = bit;
    number = (number << 1) + bit;
    cnt++;
    if (cnt > 32) {
        kill(snd_pid, SIGUSR2);
    } else {
        kill(snd_pid, SIGUSR1);
    }
}

int main(void) {

    (void)signal(SIGUSR1, my_handler);
    (void)signal(SIGUSR2, my_handler);

    pid_t pid;
    pid = getpid();
    printf("This is reciever. pid: %d\n", pid);

    printf("Enter reciever pid: ");
    scanf("%d", &snd_pid);

    kill(snd_pid, SIGUSR1);
    while (cnt<32) {
    }

    printf("Recieved number: %d\n", number);
    return 0;
}
