#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle(int arg){
  printf("stop wakin' me up...\n");
}

int main(int argc, char *argv[]){
  signal(SIGHUP,  handle);
  while(1){
   printf("while xxx\n");
   sleep(2);
  }
  return 0;
}

// ./testsignal &
// kill -HUP [pid]
