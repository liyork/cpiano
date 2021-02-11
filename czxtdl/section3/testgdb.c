#include <stdio.h>

struct Data {
  int x;
};

int main(int argc, char *argv[]){
  struct Data *p = NULL;
  printf("%d\n", p->x);
  return 0;
}

// gcc -o testgdb testgdb.c -Wall -g
// gdb testgdb
// run
// print p
//
// break main
// run
// next
// next error occur
