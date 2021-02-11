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

#define MAXTHREADS 32

long gsum = 0;
long nelems_per_thread;
//sem_t mutex;
long psum[MAXTHREADS];

void *sum_local(void *vargp) {
  long myid = *((long *)vargp);
  long start = myid * nelems_per_thread;
  long end = start + nelems_per_thread;
  long i, sum = 0;

  for (i = start; i < end; i++) {
    sum += i;
  }
  psum[myid] = sum;
  printf("myid:%ld,sum=%ld\n", myid, sum);
  return NULL;
}

int main(int argc, char **argv) {
  long i, nelems, log_nelems, nthreads, myid[MAXTHREADS];
  pthread_t tid[MAXTHREADS];
  
  if (argc != 3) {
    printf("Usage: %s <nthreads> <log_nelems>\n", argv[0]);
    exit(0);
  }
  nthreads = atoi(argv[1]);
  log_nelems = atoi(argv[2]);
  nelems = (1L << log_nelems);
  nelems_per_thread = nelems / nthreads;
  //sem_init(&mutex, 0, 1);
 
  for (i = 0; i < nthreads; i++) {
    myid[i] = i;
    pthread_create(&tid[i], NULL, sum_local, &myid[i]);
  }
  for (i = 0; i < nthreads; i++) {
    pthread_join(tid[i], NULL);
  }
  
  for (i = 0; i < nthreads; i++) {
    gsum += psum[i];
  }
  
  printf("gsum:%ld", gsum);
  if (gsum != (nelems * (nelems-1))/2) {
    printf("Error: result=%ld\n", gsum);
  }
  exit(0);
}

// ./psum-mutex.o 2 4
