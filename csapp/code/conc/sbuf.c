#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "sbuf.h"

int P(sem_t *s) {
  return sem_wait(s);
}

int V(sem_t *s) {
  return sem_post(s);
}

void sbuf_init(sbuf_t *sp, int n) {
  sp->buf = calloc(n, sizeof(int));
  sp->n = n;
  sp->front = sp->rear = 0;
  //sem_init(&sp->mutex, 0, 1);
  sp->mutex = sem_open("semmu", O_CREAT|O_EXCL, S_IRWXU, 1);
//sem_destory(&sem)改成sem_unlink("sem");
  //sem_init(&sp->slots, 0, n);
  sp->slots = sem_open("sems", O_CREAT|O_EXCL, S_IRWXU, n);
  //sem_init(&sp->items, 0, 0);
  sp->items = sem_open("semi", O_CREAT|O_EXCL, S_IRWXU, 0);
}

void sbuf_deinit(sbuf_t *sp) {
  free(sp->buf);
}

void sbuf_insert(sbuf_t *sp, int item) {
  P(sp->slots);
  P(sp->mutex);
  sp->buf[(++sp->rear)%(sp->n)] = item;
  V(sp->mutex);
  V(sp->items);
}

int sbuf_remove(sbuf_t *sp) {
  int item;
  P(sp->items);
  P(sp->mutex);
  item = sp->buf[(++sp->front)%(sp->n)];
  V(sp->mutex);
  V(sp->slots);
  return item;
}


