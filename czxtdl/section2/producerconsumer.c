#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

#define MAX (10)
int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

void put(int value){
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int get(){
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void Lock_Init(){
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&empty, NULL);
  pthread_cond_init(&fill, NULL);
}

void *producer(void *arg){
  int i;
  int loops = (int) arg;
  for (i = 0; i < loops; i++){
    pthread_mutex_lock(&mutex);
    while(count == MAX){
      pthread_cond_wait(&empty, &mutex);
    }
    put(i);
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void *consumer(void *arg){
  int i;
  int loops = (int) arg;
  for (i = 0; i < loops; i++){
    pthread_mutex_lock(&mutex);
    while(count == 0){
      pthread_cond_wait(&fill, &mutex);
    }
    int tmp = get();
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
    printf("%d\n", tmp);
  }
  return NULL;
}

int main(int argc, char *argv[]){
  Lock_Init();
  producer((int *)3);
  consumer((int *)3);
  return 0;
}

// gcc -o conhash conhash.c -Wall
