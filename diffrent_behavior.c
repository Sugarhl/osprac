#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;
  int a = 1001;
  int sum = 0;

  pid = fork();

  for(int i = 0; i < 100; ++i){
    if (pid == -1) {
        printf("Error!!!");
        /* ошибка */
    } else if (pid == 0) {

      if(i%3 == 0){
         sum = sum + i;
      }
        /* ребенок */
    } else {

      if(i%3 != 0){
         sum = sum + i;
      }
        /* родитель */
    }
      
  }

  printf("My pid = %d, my ppid = %d, sum = %d\n", (int)pid, (int)ppid, (int)sum);

  return 0;
}

