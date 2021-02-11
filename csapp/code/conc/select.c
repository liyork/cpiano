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

#define LISTENQ 1024

int open_listenfd(char *port) {
  struct addrinfo hints, *listp, *p;
  int listenfd, optval=1;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
  hints.ai_flags |= AI_NUMERICSERV;
  getaddrinfo(NULL, port, &hints, &listp);
  
  for (p = listp; p; p = p->ai_next) {
    if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      continue;
    }
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0){
      break;
    }
    close(listenfd);
  }

  freeaddrinfo(listp);
  if (!p) {
    return -1;
  }
  if (listen(listenfd, LISTENQ) < 0) {
    close(listenfd);
    return -1;
  }
  return listenfd;
}

void echo(int connfd){
  size_t n;
  char buf[MAXLINE];
  rio_t rio;
  
  rio_readinitb(&rio, connfd);
  while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    printf("server received %d bytes\n", (int)n);
    rio_writen(connfd, buf, n);
  }
}

void command(void) {
  char buf[MAXLINE];
  if (!fgets(buf, MAXLINE, stdin)) {
    exit(0);
  }
  printf("%s", buf);
}

typedef struct sockaddr SA;

int main(int argc, char **argv){
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  fd_set read_set, ready_set;
  
  if (argc != 2) {
    fprintf(stderr, "usage:%s <port>\n", argv[0]);
    exit(0);
  }

  listenfd = open_listenfd(argv[1]);

  FD_ZERO(&read_set);
  FD_SET(STDIN_FILENO, &read_set);
  FD_SET(listenfd, &read_set);

  while (1) {
    ready_set = read_set;
    select(listenfd+1, &ready_set, NULL, NULL, NULL);
    if (FD_ISSET(STDIN_FILENO, &ready_set)) {
      command();
    }
    if (FD_ISSET(listenfd, &ready_set)) {
      clientlen = sizeof(struct sockaddr_storage);
      connfd = accept(listenfd, (SA *) &clientaddr, &clientlen);
      echo(connfd);
      close(connfd);
    }
  }
  exit(0);
}

// gcc -o select.o select.c
// 控制台输入数字，可有回应
// telnet localhost 1234
// 控制台输入数字，没反应了，因为已被echo占用
