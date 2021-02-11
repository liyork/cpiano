#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <setjmp.h>

sigjmp_buf buf;

void handler(int sig){
  siglongjmp(buf, 1);
}

int main(int argc, char **argv){
  if (!sigsetjmp(buf, 1)){
    signal(SIGINT, handler);
    puts("starting\n");
  }else{
    puts("restarting\n");
  }

  while(1){
    sleep(1);
    puts("processing...\n");
  }
  exit(0);
}

// gcc -o restart.o restart.c
// ctrl + c
// kill -9 pid
