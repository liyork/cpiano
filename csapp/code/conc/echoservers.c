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


typedef struct {
  int maxfd;
  fd_set read_set;
  fd_set ready_set;
  int nready;
  int maxi;
  int clientfd[FD_SETSIZE];
  rio_t clientrio[FD_SETSIZE];
} pool;

int byte_cnt = 0;

void init_pool(int listenfd, pool *p) {
  //printf("init_pool");
  int i;
  p->maxi = -1;
  for (i=0; i < FD_SETSIZE; i++) {
    p->clientfd[i] = -1;
  }
  p->maxfd = listenfd;
  FD_ZERO(&p->read_set);
  FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool *p) {
  //printf("add_client");
  int i;
  p->nready--;
  for (i = 0; i < FD_SETSIZE; i++) {
    if (p->clientfd[i] < 0) {
      p->clientfd[i] = connfd;
      //printf("1111");
      rio_readinitb(&p->clientrio[i], connfd);
      //printf("222");
      FD_SET(connfd, &p->read_set);
      if (connfd > p->maxfd) {
        p->maxfd = connfd;
      }
      printf("maxfd:%d", p->maxfd);
      if (i > p->maxi) {
        p->maxi = i;
      }
      printf("maxi:%d", p->maxi);
      break;
    }
  }
  if (i == FD_SETSIZE) {
    //app_error("add_client error: Too many clients");
    printf("add_client error: Too many clients");
    exit(0);
  }
  //printf("finish add_client");
}

void check_clients(pool *p) {
  //printf("check_clients");
  int i, connfd, n;
  char buf[MAXLINE];
  rio_t rio;
  
  //printf("p->maxi:%d",p->maxi);
  //for (i=0; i< 10;i++){
  //  printf("i:%d", i);
  //}
  i = 0;
  for (; (i <= p->maxi) && (p->nready > 0); i++) {
    //printf("i:%d\n", i);
    //p->nready--;
    connfd = p->clientfd[i];
    rio = p->clientrio[i];
    if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) {
      p->nready--;
      if ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        byte_cnt += n;
        printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
        rio_writen(connfd, buf, n);
      }else {
        close(connfd);
        FD_CLR(connfd, &p->read_set);
        p->clientfd[i] = -1;
      }
      //sleep(1);
    }
  //printf("finish check_client");
  }
}
typedef struct sockaddr SA;

int main(int argc, char **argv){
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  static pool pool1;
  //printf("main...");
  
  //printf("argvc:%d", argc);
  if (argc != 2) {
    fprintf(stderr, "usage:%s <port>\n", argv[0]);
    exit(0);
  }

  //printf("argv[1]:%s", argv[1]);
  listenfd = open_listenfd(argv[1]);
  //printf("prepare initpool");
  init_pool(listenfd, &pool1);

  while (1) {
    pool1.ready_set = pool1.read_set;
    //printf("xxx->%d\n",FD_ISSET(listenfd, &pool1.read_set));
    //printf("qq->%d",(pool1.maxfd)+1);
    pool1.nready = select((pool1.maxfd)+1, &pool1.ready_set, NULL, NULL, NULL);
    if (FD_ISSET(listenfd, &pool1.ready_set)) {
      //printf("FD_ISSET");
      clientlen = sizeof(struct sockaddr_storage);
      connfd = accept(listenfd, (SA *) &clientaddr, &clientlen);
      add_client(connfd, &pool1);
    }
    
    //printf("aaaaa");
    check_clients(&pool1);
  }
}

// gcc -o echoservers.o echoservers.c
