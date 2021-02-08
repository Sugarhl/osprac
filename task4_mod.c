/* Студент: Сахаров Никита
 * Группа: БПи196
 * Условие: Модифицируйте программу, созданную при выполнении задачи 2 так, чтобы порождённый 
 * процесс запускал на исполнение новую (любую) программу.
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
  pid_t pid, ppid;
  int a = 1001;
  int sum = 0;

  pid = fork();

  for(int i = 0; i < 100; ++i){
    if ((int)pid < -1) {
        printf("Error!!!");
        /* ошибка */
    } else if ((int)pid != 0) {
      // родитель
      if(i%3 != 0){
         sum = sum + i;
      }
    }
  }
  if(pid == 0){
      // ребенок
      execle("/bin/cat", "/bin/cat", "task3_args_envp.c", 0, envp);
  }

  printf("My pid = %d, my ppid = %d, sum = %d\n", (int)pid, (int)ppid, (int)sum);

  return 0;
}

