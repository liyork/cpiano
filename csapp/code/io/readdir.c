#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

int main (int argc, char **argv) {
  DIR *streamp;
  struct dirent *dep;
  streamp = opendir(argv[1]);
  errno = 0;
  while ((dep = readdir(streamp)) != NULL) {
    printf("Found file: %s\n", dep->d_name);
  }
  if (errno != 0) {
    //unix_error("readdir error");
    printf("readdir error");
    exit(0);
  }
  closedir(streamp);
  exit(0);
}
