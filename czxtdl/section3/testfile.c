#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

// lseek
#include <sys/types.h>
#include <unistd.h>
// dir
#include <dirent.h>

int testopen(){
  int fd = open("foo.txt", O_CREAT | O_WRONLY | O_TRUNC);
  char buffer[10];
  buffer[0]=1;
  write(fd, buffer, 10);
  printf("fd: %d\n", fd);
  return fd;
}

void atomicUpdate(){
  int fd = open("foo.txt.tmp", O_WRONLY|O_CREAT|O_TRUNC);
  char buffer[1];
  buffer[0]=69;
  write(fd, buffer, 1);
  fsync(fd);
  close(fd);
  rename("foo.txt.tmp", "foo.txt");
}

void testlseek(int fd){
  //long a = lseek(fd, 0L, SEEK_END);
  long a = lseek(fd, 2, SEEK_SET);
  char buffer[1];
  buffer[0]=3;
  int rc = write(fd, buffer, 1);
  rc = fsync(fd);
  printf("xxx12 %ld\n",a);
  printf("rc %d\n",rc);
}

void readDir(){
  printf("readDir begin...");
  DIR *dp = opendir(".");
  struct dirent *d;
  while ((d = readdir(dp)) != NULL){
    printf("%d %s\n", (int)d->d_ino, d->d_name);
  }
  closedir(dp);
}  

int main(int argc, char *argv[]){
  printf("xx%d\n", 11);
  int fd = testopen();
  testlseek(fd);
  atomicUpdate();
  readDir();
  return 0;
}
