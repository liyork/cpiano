#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#define MAXBUF 100

void initjobs(){

}

void addjob(int pid){

}

void deletejob(int pid){

}

void handler(int sig){
  int olderrno = errno;
  sigset_t mask_all, prev_all;
  pid_t pid;
 
  sigfillset(&mask_all);
  while((pid = waitpid(-1, NULL, 0)) > 0){
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
    deletejob(pid);
    sigprocmask(SIG_SETMASK, &prev_all, NULL);
  }
  if (errno != ECHILD){
    printf("waitpid error");
    exit(0);
  }
  errno = olderrno;
}

int main(int argc, char **argv){
  int pid;
  sigset_t mask_all, mask_one, prev_one;
  
  sigfillset(&mask_all);
  sigemptyset(&mask_one);
  sigaddset(&mask_one, SIGCHLD);
  signal(SIGCHLD, handler);
  initjobs();

  while(1){
    sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
    if ((pid = fork()) == 0){
      sigprocmask(SIG_SETMASK, &prev_one, NULL);
      execve("/bin/date", argv, NULL);
    }
    sigprocmask(SIG_BLOCK, &mask_all, NULL);
    addjob(pid);
    sigprocmask(SIG_SETMASK, &prev_one, NULL);
  }
  
  exit(0);
}

// gcc -o procmask2.o procmask2.c
// ctrl +c 
