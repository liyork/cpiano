#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct node_t{
  int key;
  struct node_t *next;
} node_t;

typedef struct list_t{
  node_t *head;
  pthread_mutex_t lock;
} list_t;

void List_Init(list_t *L){
  L->head = NULL;
  pthread_mutex_init(&L->lock, NULL);
}

void List_Insert(list_t *L, int key){
  node_t *new = malloc(sizeof(node_t));
  if (new == NULL) {
    perror("malloc");
    return;
  }
  new->key = key;

  pthread_mutex_lock(&L->lock);
  new->next = L->head;
  L->head = new;
  pthread_mutex_unlock(&L->lock);
}

int List_Lookup(list_t *L, int key){
  int rv = -1;
  pthread_mutex_lock(&L->lock);
  node_t *curr = L->head;
  while(curr){
    if (curr->key == key){
      rv = 0;
      break;
    }
    curr = curr->next;
  }
  pthread_mutex_unlock(&L->lock);
  return rv;
}

//int main(int argc, char *argv[]){
//  list_t *l = malloc(sizeof(list_t));
//  List_Init(l);
//  List_Insert(l, 1);
//  List_Insert(l, 2);
//  List_Insert(l, 3);
//  int a = List_Lookup(l, 1);
//  int b = List_Lookup(l, 4);
//  printf("%d, %d\n", a, b);
//}

// gcc -o conlink conlink.c -Wall
