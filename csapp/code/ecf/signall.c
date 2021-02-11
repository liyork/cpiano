#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#define MAXBUF 100

void handler2(int sig){
  int olderrno = errno;

  while(waitpid(-1, NULL, 0) > 0){
    puts("handler reaped child\n");
  }
  if (errno != ECHILD) {
    printf("waitpid error");
    exit(0);
  }
  sleep(1);
  errno = olderrno;
}

int main(){
  int i, n;
  char buf[MAXBUF];
  
  if (signal(SIGCHLD, handler2) == SIG_ERR) {
    printf("read error");
    exit(0);
  }

  for (i = 0; i < 3; i++) {
    if (fork() == 0){
      printf("Hello from child %d\n", (int)getpid());
      exit(0);
    }
  }

  if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
    printf("read error");
    exit(0);
  }

  printf("Parent processing input\n");
  while(1){
    ;
  }
  exit(0);
}

// gcc -o signall.o signall.c
// ctrl +c 
