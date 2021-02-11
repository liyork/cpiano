#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "../src/csapp.c"

#define N 4

void *thread(void *vargp) {
  int myid = *((int *)vargp);
  free(vargp);
  printf("Hello from thread %d\n", myid);
  return NULL;
}

int main() {
  pthread_t tid[N];
  int i, *ptr;

  for (i = 0; i < N; i++) {
    ptr = malloc(sizeof(int));
    *ptr = i;
    pthread_create(&tid[i], NULL, thread, ptr);
  }
  for (i = 0; i < N; i++) {
    pthread_join(tid[i], NULL);
  }
  exit(0);
}

// gcc -o norace.o norace.c
