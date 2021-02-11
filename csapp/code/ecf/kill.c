#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(){
  pid_t pid;

  if ((pid = fork()) == 0){
    printf("wait for a signal to arrive");
    pause();
    printf("control should never reach here!\n");
    exit(0);
  }

  printf("chid pid :%d", pid);

  sleep(2);
  kill(pid, SIGKILL);
  exit(0);
}

// gcc -o kill.o kill.c
