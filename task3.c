#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
const int buf_size = 100000;

int main(int argc, char* argv[], char* envp[])
{

    int result, size_write, size_read;
    int fd[2];
    int res = pipe(fd);
    char buf[buf_size];
    if(res){
        printf("Can't open pipe");
        exit(-1);
    }
    if ((result = fork()) < 0) {
        printf("Can\t fork child\n");
        exit(-1);
    } else if (result > 0) {
        int size_write = write(fd[1], buf, buf_size);
        printf("Write size:%d\n", size_write);
    } else {
        int size_read = read(fd[0], buf, buf_size);
        printf("Pipe size (get from read):%d\n", size_read);
    }

    close(fd[0]);
    close(fd[1]);
    
    return 0;
}
