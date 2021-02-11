#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  int x = 3;
  printf("x: %d\n", x);
  int *x1 = (int *) malloc(sizeof(int));
  printf("x1 add: %p\n", x1);
  printf("x1 size: %ld\n", sizeof(x1));
  double *d = (double *) malloc(sizeof(double));
  printf("double size: %ld\n", sizeof(d));
  int *y = malloc(10 * sizeof(int));
  printf("10*int size: %ld\n", sizeof(y));// 结果不是想要的实际大小
  int z[10];
  printf("int arr size: %ld\n", sizeof(z));

  char *src = "hello";
  char *dst = (char *) malloc(strlen(src)+1);
  strcpy(dst, src);
  printf("dst: %s\n", dst);

  free(x1);
}

// gcc -o memapitest memapitest.c -Wall
// ./memapitest
