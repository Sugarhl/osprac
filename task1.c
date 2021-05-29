#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int     fd;
    char    fileName[] = "file";
    char    symlinkName[] = "tmp/file";
    (void) umask(0);

    // создаём директорию под файлы
    if (mkdir("tmp", 0777) < 0) {
        printf("Unable to create dir 'tmp'.\n");
        exit(-1);
    }

    // создаём начальный файл
    if ((fd = open("tmp/file", O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }
    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }

    int maxRecLvl = 0;
    while (1) {
        //меняем имя ссылки
        symlinkName[7]++;

        if (symlink(fileName, symlinkName) != 0) {
            printf("Unable to create symlink\n");
            exit(-1);
        }
        // считаем глубину рекурсии через попытку открытия.
        if ((fd = open(symlinkName, O_WRONLY, 0666)) < 0) {
            break;
        } else {
            maxRecLvl++;
        }

        if (close(fd) < 0) {
            printf("Can\'t close file\n");
        }
        // теперь хотим ссылаться на новый файлик.
        fileName[3]++;
    }

    printf("Max recursion level: %d\n", maxRecLvl);


    return 0;
}
