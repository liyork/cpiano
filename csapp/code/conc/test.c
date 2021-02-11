#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "../src/csapp.c"
#include <sys/select.h>
#define MAXLINE 1024

typedef struct sockaddr SA;

int main(int argc, char **argv){
  printf("argc:%d\n", argc);
  if (argc != 2) {
    fprintf(stderr, "usage:%s <port>\n", argv[0]);
    exit(0);
  }

  printf("argv[0]:%s\n", argv[0]);
  printf("argv[1]:%s\n", argv[1]);
}

// gcc -o echoservers.o echoservers.c
