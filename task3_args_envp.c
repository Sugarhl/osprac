/* Студент: Сахаров Никита 
 * Группа: БПи196
 * Условие: Напишите программу, распечатывающую значения аргументов
 * командной строки и параметров окружающей среды для текущего процесса.
 */ 
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[])
{
    printf("Command line agruments:\n(number:\t agrument:)\n");
    for(int i = 0;i < argc; ++i){
        printf("%d\t%s\n", i, argv[i]);
    }
    printf("\n");

    int i = 0;
    printf("Enviroment variables:\n(number:\t variable:)\n");
    while(envp[i]!=NULL){
        printf("%d\t%s\n", i, envp[i]);
        ++i;
    }

    return 0;  
}
