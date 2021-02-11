#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

void *thread(void *vargp);

volatile long cnt = 0;
sem_t *mutex;

int main(int argc, char **argv) {
  //sem_init(&mutex, 0, 1);
  sem_t* psemaphore = sem_open("/mysem",O_CREAT, S_IRUSR | S_IWUSR, 1);
  mutex = psemaphore;
  long niters;
  pthread_t tid1, tid2;
  if (argc != 2) {
    printf("usage: %s <niters>\n", argv[0]);
    exit(0);
  }
  niters = atoi(argv[1]);
  pthread_create(&tid1, NULL, thread, &niters);
  pthread_create(&tid2, NULL, thread, &niters);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
 
  if (cnt != (2 * niters)) {
    printf("BOOM! cnt=%ld\n", cnt);
  }else {
    printf("OK cnt=%ld\n", cnt);
  }
  exit(0);
}

void *thread(void *vargp) {
  long i, niters = *((long *)vargp);
  for (i = 0; i < niters; i++) {
    sem_wait(mutex);
    cnt++; 
    sem_post(mutex);
  }
  return NULL;
}

// ./badcnt.o 100000
