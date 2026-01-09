#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
work(int n)
{
  int i;
  int j;
  for(i = 0; i < n; i++){
    for(j = 0; j < 1000000; j++){
      asm("nop");
    }
  }
}

int
main(int argc, char *argv[])
{
  int pid;
  int i;
  int start, end;

  if (argc < 2) {
    printf("usage: schedtest <N processes>\n");
    exit(1);
  }
  
  int N = atoi(argv[1]);

  printf("Starting %d processes...\n", N);
  start = uptime();
  
  for(i = 0; i < N; i++){
    pid = fork();
    if(pid == 0){
      // child
      // set priority based on i (just for PBS demonstration)
      // i=0 -> priority 60
      // i=1 -> priority 50
      // i=2 -> priority 40
      // Lower value = higher priority. So later children have higher priority here?
      // Let's set varied priorities.
      int prio = 60 - i*2;
      set_priority(getpid(), prio);
      
      uint64 my_start = uptime();
      work(50); // do some work
      uint64 my_end = uptime();
      
      printf("Child %d (pid %d) (prio %d): start %d, end %d, duration %d\n", i, getpid(), prio, (int)my_start, (int)my_end, (int)(my_end - my_start));
      exit(0);
    }
  }

  for(i = 0; i < N; i++){
    wait(0);
  }
  end = uptime();
  
  printf("Total time: %d\n", end - start);
  exit(0);
}
