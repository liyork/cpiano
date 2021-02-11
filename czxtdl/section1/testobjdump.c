#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]){
  int *p = malloc(sizeof(int));
  *p = 0;
  *p = *p + 1;
  return 0;
}

// gcc -o testobjdump testobjdump.c -Wall
// ./testobjdump
// objdump -d testobjdump
