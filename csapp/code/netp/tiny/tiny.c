#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include "../../src/csapp.c"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#define MAXLINE 1024
#define MAXBUF 1024


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

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) {
  char buf[MAXLINE], body[MAXBUF];
  sprintf(body, "<html><title>Tiny Error</title>");
  sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
  rio_writen(fd, buf, strlen(buf));
  rio_writen(fd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp) {
  char buf[MAXLINE];
  rio_readlineb(rp, buf, MAXLINE);
  while(strcmp(buf, "\r\n")) {
    rio_readlineb(rp, buf, MAXLINE);
    printf("xxxx->%s", buf);
  }
  return;
}

int parse_uri(char *uri, char *filename, char *cgiargs) {
  char *ptr;
  if (!strstr(uri, "cgi-bin")) {
    strcpy(cgiargs, "");
    strcpy(filename, "./");
    strcat(filename, uri);
    if (uri[strlen(uri)-1] == '/') {
      strcat(filename, "home.html");
    }
    return 1;
  }else {
    ptr = index(uri, '?');
    if (ptr) {
      strcpy(cgiargs, ptr+1);
      *ptr = '\0';
    }else {
      strcpy(cgiargs, "");
    }
    strcpy(filename, ".");
    strcat(filename, uri);
    return 0;
  }
}

void get_filetype(char *filename, char *filetype) {
  if (strstr(filename, ".html")) {
    strcpy(filetype, "text/html");
  }else if (strstr(filename, ".gif")) {
    strcpy(filetype, "image/gif");
  }else if (strstr(filename, ".png")) {
    strcpy(filetype, "image/png");
  }else if (strstr(filename, ".jpg")) {
    strcpy(filetype, "image/jpeg");
  }else {
    strcpy(filetype, "text/plain");
  }
}

void serve_static(int fd, char *filename, int filesize) {
  int srcfd;
  char *srcp, filetype[MAXLINE], buf[MAXBUF];
  get_filetype(filename, filetype);
  sprintf(buf, "HTTP/1.0 200 OK\r\n");
  sprintf(buf, "%sServer: TIny Web Server\r\n", buf);
  sprintf(buf, "%sConnection: close\r\n", buf);
  sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
  sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
  rio_writen(fd, buf, strlen(buf));
  printf("response headers:\n");
  printf("%s", buf);

  srcfd = open(filename, O_RDONLY, 0);
  srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
  close(srcfd);
  rio_writen(fd, srcp, filesize);
  munmap(srcp, filesize);
}


char *environ[ ]={"PATH=/bin", NULL};
void serve_dynamic(int fd, char *filename, char *cgiargs) {
  char buf[MAXLINE], *emptylist[] = {NULL};
  sprintf(buf, "HTTP/1.0 200 OK\r\n");
  rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Server: Tiny Web Server\r\n");
  rio_writen(fd, buf, strlen(buf));
  //rio_writen(fd, cgiargs, 20);

  if (fork() == 0){
    //printf("cgiargs:%c", *cgiargs);
    setenv("QUERY_STRING", cgiargs, 1);
    dup2(fd, STDOUT_FILENO);
    execve(filename, emptylist, environ);
  }
  wait(NULL);
}

void doit(int fd) {
  int is_static;
  struct stat sbuf;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char filename[MAXLINE], cgiargs[MAXLINE];
  rio_t rio;
  
  rio_readinitb(&rio, fd);
  rio_readlineb(&rio, buf, MAXLINE);
  printf("Request headers:\n");
  //printf("xxxx1111->%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);
  //printf("yyy->%d %d", 1, 2);
  //printf("yyy2->%lu", strlen(method));
  if (strcasecmp(method, "GET")) {
    clienterror(fd, method, "501", "Not implemented", "Tiny does not implement this method");
    return;
  }
  read_requesthdrs(&rio);

  is_static = parse_uri(uri, filename, cgiargs);
  if (stat(filename, &sbuf) < 0) {
    clienterror(fd, filename, "404", "Not found", "Tiny couldn't find this file");
    return;
  }
  if (is_static) {
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
      clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
      return;
    }
    serve_static(fd, filename, sbuf.st_size);
  }else {
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
      clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
      return;
    }
    serve_dynamic(fd, filename, cgiargs);
  }
}

typedef struct sockaddr SA;
int main(int argc, char **argv){
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  listenfd = open_listenfd(argv[1]);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
    getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd);
    close(connfd);
  }
  
  exit(0);
}

// GET / HTTP/1.0 + 回车
// GET a.txt HTTP/1.0
// GET /cgi-bin/adder?15000&213 HTTP/1.0

