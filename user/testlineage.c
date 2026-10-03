#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid = getpid();

  printf("My PID is: %d\n", pid);

  int count = lineage(pid);

  printf("Count of ancestors: %d\n", count);

  exit(0);
}