#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <setjmp.h>

jmp_buf buf;

int error1 = 1;
int error2 = 0;

void foo(void), bar(void);

int main(int argc, char **argv){
  switch(setjmp(buf)){
  case 0:
    foo();
    break;
  case 1:
    printf("Detected an error1 condition in foo\n");
    break;
  case 2:
    printf("Detected an error2 condition in foo\n");
    break;
  default:
    printf("Unknown error condition in foo\n");
  }
  exit(0);
}

void foo(void){
  if (error1){
    longjmp(buf, 1);
  }
  bar();
}

void bar(void){
  if (error2){
    longjmp(buf, 2);
  }
}

// gcc -o setjmp.o setjmp.c
// 修改error1/error2
