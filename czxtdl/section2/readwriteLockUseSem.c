#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct _rwlock_t{
  sem_t *lock;
  sem_t *writelock;
  int readers;
} rwlock_t;

void rwlock_init(rwlock_t *rw){
  rw->readers = 0;
  rw->lock = sem_open("lock", O_CREAT, 0644, 1);
  rw->writelock = sem_open("writelock", O_CREAT, 0644, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw){
  sem_wait(rw->lock);
  rw->readers++;
  if (rw->readers == 1){
    sem_wait(rw->writelock);
  }
  sem_post(rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw){
  sem_wait(rw->lock);
  rw->readers--;
  if (rw->readers == 0){
    sem_post(rw->writelock);
  }
  sem_post(rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw){
  sem_wait(rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw){
  sem_post(rw->writelock);
}

int main(int argc, char *argv[]){
}

// gcc -o sem2condition sem2condition.c -Wall
