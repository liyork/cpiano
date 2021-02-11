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

void sigchld_handler(int sig) {
  while (waitpid(-1, 0, WNOHANG) > 0) {
    ;
  }
  return;
}

typedef struct sockaddr SA;

int main(int argc, char **argv){
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  
  if (argc != 2) {
    fprintf(stderr, "usage:%s <port>\n", argv[0]);
    exit(0);
  }

  signal(SIGCHLD, sigchld_handler);
  listenfd = open_listenfd(argv[1]);
  while (1) {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = accept(listenfd, (SA *) &clientaddr, &clientlen);
    if (fork() == 0) {
      close(listenfd);
      echo(connfd);
      close(connfd);
      exit(0);
    }
    close(connfd);
  }
  exit(0);
}

// telnet localhost 1234
