#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

char send = 1;
char curr_send = 1;

void my_handler(int nsig) {
    if (nsig == SIGUSR1) {
        curr_send = 1;
    } else if (nsig == SIGUSR2) {
        send = 0;
    }
}

int main(void) {

    (void)signal(SIGUSR1, my_handler);
    (void)signal(SIGUSR2, my_handler);

    pid_t pid, rec_pid;
    pid = getpid();
    printf("This is sender. pid: %d\n", pid);

    printf("Enter reciever pid: ");
    scanf("%d", &rec_pid);

    printf("Enter your number: ");
    uint number;
    scanf("%d", &number);
    int cnt=0;

    uint mask = 0x80000000;
    while (send) {
        if (curr_send) {

            int bit = (number & mask) ? 1 : 0;

            mask >>= 1;
            if (bit) {
                kill(rec_pid, SIGUSR1);
            } else {
                kill(rec_pid, SIGUSR2);
            }
            curr_send = 0;
        }
    }

    return 0;
}
