#include <stdio.h>
#include <stdlib.h>
#include <sys/tim.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
  while(1){
    fd_set readFDs;
    FD_ZERO(&readFDs);

    int fd;
    for (fd = minFD; fd < maxFD; fd++){
      FD_SET(FD, &readFDs);
    }
  
    int rc = select(maxFD+1, &readFDs, NULL, NULL, NULL);

    int fd;
    for (fd = minFD; fd < maxFD; fd++){
      if (FD_ISSET(fd, &readFDs)){
        processFD(fd);
      }
    }
  }
}
