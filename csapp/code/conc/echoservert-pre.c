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
#include "sbuf.c"
#define NTHREADS 4
#define SBUFSIZE 16

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


sbuf_t sbuf;

static int byte_cnt;
static sem_t *mutex;

static void init_echo_cnt(void) {
  //sem_init(&mutex, 0, 1);
  mutex = sem_open("/mysem",O_CREAT, S_IRUSR | S_IWUSR, 1);
  byte_cnt = 0;
}

void echo_cnt(int connfd) {
  int n;
  char buf[MAXLINE];
  rio_t rio;
  static pthread_once_t once = PTHREAD_ONCE_INIT;
  pthread_once(&once, init_echo_cnt);
  rio_readinitb(&rio,connfd);
  while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    P(mutex);
    byte_cnt += n;
    printf("server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
    V(mutex);
    rio_writen(connfd, buf, n);
  }
}

void *thread(void *vargp) {
  pthread_detach(pthread_self());
  while (1) {
    int connfd = sbuf_remove(&sbuf);
    echo_cnt(connfd);
    close(connfd);
  }
}

typedef struct sockaddr SA;
int main(int argc, char **argv) {
  int i, listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }

  listenfd = open_listenfd(argv[1]);
  sbuf_init(&sbuf, SBUFSIZE);
  for (i = 0; i < NTHREADS; i++) {
    pthread_create(&tid, NULL, thread, NULL);
  }

  while (1) {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
    sbuf_insert(&sbuf, connfd);
  }
}

// telnet localhost 2222
