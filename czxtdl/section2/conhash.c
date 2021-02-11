#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include "conlink.c"

#define BUCKETS (101)

typedef struct __hash_t{
  list_t lists[BUCKETS];
} hash_t;

void Hash_Init(hash_t *H){
  int i;
  for (i = 0; i < BUCKETS; i++){
    List_Init(&H->lists[i]);
  }
}

int Hash_Insert(hash_t *H, int key){
  int bucket = key % BUCKETS;
  List_Insert(&H->lists[bucket], key);
  return 1;
}

int Hash_Lookup(hash_t *H, int key){
  int bucket = key % BUCKETS;
  return List_Lookup(&H->lists[bucket], key);
}

int main(int argc, char *argv[]){
  hash_t *h = malloc(sizeof(hash_t));
  Hash_Init(h);
  Hash_Insert(h, 1);
  Hash_Insert(h, 2);
  Hash_Insert(h, 3);
  int a = Hash_Lookup(h, 2);
  printf("%d\n", a);
  a = Hash_Lookup(h, 3);
  printf("%d\n", a);
}

// gcc -o conhash conhash.c -Wall
