#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define N 2

int main(){
  int status, i;
  pid_t pid[N], retpid;
  
  for (i = 0; i < N; i++) {
    if ((pid[i] = fork()) == 0){
      exit(100+i);
    }
  }

  i = 0;
  while ((retpid = waitpid(pid[i++], &status, 0)) > 0) {
    if (WIFEXITED(status)){
      printf("child %d terminated normally with exit status=%d\n",
             retpid, WEXITSTATUS(status));
    }else{
      printf("child %d terminated abnormally\n", retpid);
    }
  }
 
  if (errno != ECHILD){
    //暂时执行不了
    //unix_error("waitpid error");
    printf("waitpid error");
  }
  
  exit(0);
}

// gcc -o waitpid2.o waitpid2.c
