#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <lib.h>
#include <minix/type.h>
int main(int argc, char* argv[]) 
{
  message mes;
  pid_t  pid = getpid();
  int i, j;
  int prio = atoi(argv[1]);
  int id = pid;
  printf("inicjalizacja procesu o pid: %d z argumentem: %d\n", id, prio);

  mes.m1_i1 =  pid;
  mes.m1_i2=  prio;
  _syscall(MM, SETPRI , &mes);
  for(j=0; j<40; ++j) 
  {
    for(i=1; i<9999999; ++i);
  }
  printf("skonczyl sie proces o pid: %d z argumentem: %d \n", getpid(), prio);
  return 0;
}
