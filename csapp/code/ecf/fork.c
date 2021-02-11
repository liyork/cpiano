#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  pid_t pid;
  int x = 1;

  pid = fork();
  if (pid == 0) {
    printf("child : x=%d\n", ++x);
    // 子进程退出
    exit(0);
  }

  printf("parent: x=%d\n", --x);
  // 主进程退出
  exit(0);
}

// gcc -o fork.o fork.c
