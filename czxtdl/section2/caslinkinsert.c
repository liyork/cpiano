#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

// assume
int CompareAndSwap(int *address, int expected, int new){
  if (*address == expected) {
    *address = new;
    return 1;
  }
  return 0;
}

void AtomicIncrement(int *value, int amount){
  do {
    int old = *value;
  } while (CmopareAndSwap(value, old, old + amount) == 0);
}

// use lock
void insert(int value){
  node_t *n = malloc(sizeof(node_t));
  assert(n != NULL);
  n->value = value;
  lock(listlock);
  n->next = head;
  head = n;
  unlock(listlock);
}

// use cas
void insert(int value){
  node_t *n = malloc(sizeof(node_t));
  assert(n != NULL);
  n->value = value;
  do {
    n->next = head;
  } while (CompareAndSwap(&head, n->next, n) == 0);
}

int main(int argc, char *argv[]){
}

// gcc -o conlink conlink.c -Wall
