#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;
  int a = 1001;
  int sum = 0;
  
  for(int i = 0; i < 100; ++i){
      sum = sum + i;
  }

  pid  = getpid();
  ppid = getppid();

  printf("My pid = %d, my ppid = %d, sum = %d\n", (int)pid, (int)ppid, (int)sum);
  /* PID меняется т.к. ОС выдает новый айдишник для только рожденного процесса 
   * PPID не меняется потому что процесс наследуется от какого то процесса запушенного при старте ОС
   * (PID растет потому что в unix есть счетчик для процессов, который инкрементируется до маскимума,
   * а дальше смотрит на занятость id начиная отсчет заново. ) */

  return 0;
}
