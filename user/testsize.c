#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid = getpid();
  int size = getprocsize(pid);

  printf("My PID is: %d\n", pid);
  printf("Memory size: %d bytes\n", size);

  exit(0);
}