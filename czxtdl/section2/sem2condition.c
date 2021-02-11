#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t *s;

void Sem_Innit(){
  s = sem_open("xx", O_CREAT, 0644, 1);
}

void *child(void *arg){
  printf("child\n");
  sem_post(s);
  return NULL;  
}

int main(int argc, char *argv[]){
  printf("parent:begin\n");
  pthread_t c;
  pthread_create(&c, NULL, child, NULL);
  sem_wait(s);
  printf("parent:end\n");
  return 0;
}

// gcc -o sem2condition sem2condition.c -Wall
