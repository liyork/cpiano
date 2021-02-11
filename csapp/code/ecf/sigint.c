#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

void sigint_handler(int sig){
  printf("Caught SIGINT!\n");
  exit(0);
}

int main(){
  if (signal(SIGINT, sigint_handler) == SIG_ERR){
    //unix_error("signal error");
    printf("signal error");
    exit(0);
  }
  pause();
  return 0;
}

// gcc -o sigint.o sigint.c
// ctrl +c 
