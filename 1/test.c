#include </usr/include/lib.h>
#include </usr/include/stdlib.h>
#include </usr/include/stdio.h>

int getprocnr(int id)
{
 message msg;
 msg.m1_i1 = id;

 return (_syscall(0,78,&msg));
}

int main(int argc, char* argv[])
{
 int id;
 int i;
 int result;

 if(argc == 1)
 {
  printf("Potrzebny argument\n");
  return 0;
 }
 else
 {
  id = atoi( argv[1] );
 }

 for(i = 10; i >= 0; i--)
 {
  result = getprocnr( id - i );

  if(result != -1) 
  {
   printf("Proces %d ma indeks %d w tablicy\n", id - i, result);
  }
  else
  {
   printf("Procesu %d nie ma w tablicy. Kod bledu %d\n", id - i, errno);
  }
 }

 return 0;
}

