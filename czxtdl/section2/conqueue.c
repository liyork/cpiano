#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __node_t{
  int value;
  struct __node_t *next;
} __node_t;

typedef struct queue_t{
  __node_t *head;
  __node_t *tail;
  pthread_mutex_t headLock;
  pthread_mutex_t tailLock;
} queue_t;

void Queue_Init(queue_t *q){
  __node_t *tmp = malloc(sizeof(__node_t));
  tmp->next = NULL;
  q->head = q->tail = tmp;
  pthread_mutex_init(&q->headLock, NULL);
  pthread_mutex_init(&q->tailLock, NULL);
}

void Queue_Enqueue(queue_t *q, int value){
  __node_t *tmp = malloc(sizeof(__node_t));
  assert(tmp != NULL);
  tmp->value = value;
  tmp->next = NULL;

  pthread_mutex_lock(&q->tailLock);
  q->tail->next = tmp;
  q->tail = tmp;
  pthread_mutex_unlock(&q->tailLock);
}

int Queue_Dequeue(queue_t *q, int *value){
  pthread_mutex_lock(&q->headLock);
  __node_t *tmp = q->head;
  __node_t *newHead = tmp->next;
  if (newHead == NULL){
    pthread_mutex_unlock(&q->headLock);
    return -1;
  }
  *value = newHead->value;
  q->head = newHead;
  pthread_mutex_unlock(&q->headLock);

  free(tmp);
  return 0;
}

int main(int argc, char *argv[]){
  queue_t *q = malloc(sizeof(queue_t));
  Queue_Init(q);
  Queue_Enqueue(q, 1);
  Queue_Enqueue(q, 2);
  int a;
  int b;
  b = Queue_Dequeue(q, &a);
  printf("%d, %d\n", a, b);
  b = Queue_Dequeue(q, &a);
  printf("%d, %d\n", a, b);
  int c;
  b = Queue_Dequeue(q, &c);
  printf("%d, %d\n", c, b);
}
