#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

const int NUMCPUS = 2;

typedef struct counter_t {
  int global;
  pthread_mutex_t glock;
  
  int local[NUMCPUS];
  pthread_mutex_t llock[NUMCPUS];
  int threshold;
} counter_t;

void init(counter_t *c, int threshold){
  c->threshold = threshold;
  
  c->global = 0;
  pthread_mutex_init(&c->glock, NULL);

  int i;
  for (i = 0; i < NUMCPUS; i++) {
    c->local[i] = 0;
    pthread_mutex_init(&c->llock[i], NULL);
  }
}

void update(counter_t *c, int threadID, int amt) {
  pthread_mutex_lock(&c->llock[threadID]);
  c->local[threadID] += amt;
  if (c->local[threadID] >= c->threshold) {
    pthread_mutex_lock(&c->glock);
    c->global += c->local[threadID];
    pthread_mutex_unlock(&c->glock);
    c->local[threadID] = 0;
  }
  pthread_mutex_unlock(&c->llock[threadID]);
}

int get(counter_t *c){
  pthread_mutex_lock(&c->glock);
  int val = c->global;
  pthread_mutex_unlock(&c->glock);
  return val;
}

int main(int argc, char *argv[]){
  counter_t *ct = malloc(sizeof(counter_t));
  init(ct, 2);
  update(ct, 1, 1);
  update(ct, 1, 1);
  update(ct, 1, 1);
  int a = get(ct);
  printf("%d\n", a); 
}


// gcc -o concounter concounter.c -Wall -lpthread
