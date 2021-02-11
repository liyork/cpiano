#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(void){
  char c;

  while(read(STDIN_FILENO, &c, 1) != 0){
    write(STDOUT_FILENO, &c, 1);
  }
  exit(0);
}

// gcc -o cpstdin.o cpstdin.c
