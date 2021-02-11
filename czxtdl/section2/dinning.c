#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <semaphore.h>

int p;
sem_t *forks[5];

void Din_Init(){
   
}

void think(){
  printf("think ..");
}

void eat(){
  printf("eat ..");
}

int left(int p){
  return p;
}

int right(int p){
  return (p + 1) % 5;
}

void getforks(){
  if(p == 4){
    sem_wait(forks[right(p)]);
    sem_wait(forks[left(p)]);
  }else{
    sem_wait(forks[left(p)]);
    sem_wait(forks[right(p)]);
  }
}

void putforks(){
  sem_post(forks[left(p)]);
  sem_post(forks[right(p)]);
}

int main(int argc, char *argv[]){
  while(1){
    think();
    getforks();
    eat();
    putforks();
  }
}



// gcc -o sem2condition sem2condition.c -Wall
