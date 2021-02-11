#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <semaphore.h>

#define MAX (10)

int buffer1[MAX];
int fill = 0;
int use = 0;

void put(int value){
  printf("put i:%d, value:%d\n", fill, value);
  buffer1[fill] = value;
  fill = (fill + 1) % MAX;
  printf("put fill:%d, buffer1[value]:%d\n", fill-1, buffer1[fill-1]);
}

int get(){
  printf("get buffer[1]:%d, use:%d\n", buffer1[1], use);
  int tmp = buffer1[use];
  use = (use + 1) % MAX;
  return tmp;
}

sem_t *empty;
sem_t *full;
sem_t *mutex;

void Sem_Innit(){
  empty = sem_open("empty", O_CREAT, 0644, MAX);
  full = sem_open("full", O_CREAT, 0644, 0);
  mutex = sem_open("mutex", O_CREAT, 0644, 1);
}

void *producer(void *arg){
  int i;
  int loops = (int) arg;
  for (i = 0; i < loops; i++){
    sem_wait(empty);
    sem_wait(mutex);
    put(i);
    printf("put i:%d\n", i);
    sem_post(mutex);
    sem_post(full);
  }
  return NULL;
}

void *consumer(void *arg){
  int i;
  int loops = (int) arg;
  for (i = 0; i < loops; i++){
    sem_wait(full);
    sem_wait(mutex);
    int tmp = get();
    sem_post(mutex);
    sem_post(empty);
    printf("%d\n", tmp);
  }
  return NULL;
}

int main(int argc, char *argv[]){
  pthread_t p;
  pthread_create(&p, NULL, producer, (int *)5);
  pthread_t c;
  pthread_create(&c, NULL, consumer, (int *)5);
  pthread_join(c, NULL);
  pthread_join(p, NULL);
  printf("finish\n");
  printf("%d\n", buffer1[1]);
  int xx[2];
  xx[0] = 1;
  printf("%d\n", xx[0]);
  return 0;
}

// gcc -o sem2condition sem2condition.c -Wall
