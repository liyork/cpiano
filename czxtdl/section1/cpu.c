#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include "common.h"
#include <unistd.h>

int main(int argc, char *argv[]){
  if (argc != 2) {
    fprintf(stderr, "usage: cpu <string>\n");
    exit(1);
  }
  char *str = argv[1];
  while (1){
    Spin(1);
    //sleep(1);
    printf("%s\n",str);
  }
  return 0;
}

// gcc -o cpu cpu.c -Wall
// ./cpu "A"
// ./cpu A & ; ./cpu B & ; ./cpu C & ; ./cpu D &
