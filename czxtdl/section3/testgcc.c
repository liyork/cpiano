#include <math.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  printf("hello,world\n");
  printf("argc:%d, argv0:%s, argv1:%s\n", argc, argv[0], argv[1]);
  double a = 0.8;
  printf("tan:%lf\n", tan(a)); 
  return 0;
}

// gcc -o testgcc testgcc.c -Wall -lm
