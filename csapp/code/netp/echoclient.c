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


int open_clientfd(char *hostname, char *port) {
  int clientfd;
  struct addrinfo hints, *listp, *p;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICSERV;
  hints.ai_flags |= AI_ADDRCONFIG;
  getaddrinfo(hostname, port, &hints, &listp);

  for (p = listp; p; p = p->ai_next) {
    if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      continue;
    }
    if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
      break;
    }
    close(clientfd);
  }
  
  freeaddrinfo(listp);
  if (!p) {
    return -1;
  }else {
    return clientfd;
  }
}

int main(int argc, char **argv){
  int clientfd;
  char *host, *port, buf[MAXLINE];
  rio_t rio;
  if (argc != 3) {
    fprintf(stderr, "usage:%s <host> <port>\n", argv[0]);
    exit(0);
  }
  host = argv[1];
  port = argv[2];

  clientfd = open_clientfd(host,port);
  rio_readinitb(&rio, clientfd);

  while (fgets(buf, MAXLINE, stdin) != NULL) {
    rio_writen(clientfd, buf, strlen(buf));
    rio_readlineb(&rio, buf, MAXLINE);
    fputs(buf, stdout);
  }  
  close(clientfd);
  exit(0);
}

// ./echoclient.o localhost 15333
