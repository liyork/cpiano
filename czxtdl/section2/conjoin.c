#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

int done = 0;
pthread_mutex_t m ;
pthread_cond_t c ;

void Lock_Init(){
  pthread_mutex_init(&m, NULL);
  pthread_cond_init(&c, NULL);
}

void thr_exit(){
  pthread_mutex_lock(&m);
  done = 1;
  pthread_cond_signal(&c);
  pthread_mutex_unlock(&m);
}

void *child(void *arg){
  printf("child\n");
  thr_exit();
  return NULL;
}

void thr_join(){
  pthread_mutex_lock(&m);
  while (done == 0){
    pthread_cond_wait(&c, &m);
  }
  pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[]){
  printf("parent:begin\n");
  pthread_t p;
  pthread_create(&p, NULL, child, NULL);
  thr_join();
  printf("parent:end\n");
  return 0;
}

// gcc -o conhash conhash.c -Wall
