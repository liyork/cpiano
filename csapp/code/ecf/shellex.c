#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define MAXLINE 1000
#define MAXARGS 128

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int buildin_command(char **argv);

int main(){
  char cmdline[MAXLINE];

  while(1){
    printf("> ");
    fgets(cmdline, MAXLINE, stdin);
    if (feof(stdin)) {
      exit(0);
    }
  
    eval(cmdline);
  }
}

void eval(char *cmdline){
  char *argv[MAXARGS];
  char buf[MAXLINE];
  int bg;
  pid_t pid;
  char *environ[ ]={"PATH=/bin", NULL};

  strcpy(buf, cmdline);
  bg = parseline(buf, argv);
  if (argv[0] == NULL) {
    return;
  }

  if (!buildin_command(argv)) {
    if ((pid = fork() == 0)){
      if (execve(argv[0], argv, environ) <0) {
        printf("%s: Command not found.\n", argv[0]);
        exit(0);
      }
    }

    if (!bg) {
      printf("bg=0,%d %s ", pid, cmdline);
      int status;
      if (waitpid(pid, &status, 0) < 0){
        //unix_error("waitfg: waitpid error");
        printf("waitfg: waitpid error");
      }
    }else{
      printf("bg=1,%d %s ", pid, cmdline);
    }
  }
  
  return;
}

int buildin_command(char **argv){
  if (!strcmp(argv[0], "quit")){
    exit(0);
  }
  if (!strcmp(argv[0], "&")){
    return 1;
  }
  return 0;
}

int parseline(char *buf, char **argv){
  char *delim;
  int argc;
  int  bg;
  
  // 去除空格最后空格
  buf[strlen(buf)-1] = ' ';
  // 去除相邻空格
  while (*buf && (*buf == ' ')){
    buf++;
  }
  
  argc = 0;
  // 定位字符' '，获取每个参数
  while ((delim = strchr(buf, ' '))) {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim +1;
    // 去除相邻空格
    while(*buf && (*buf == ' ')){
      buf++;
    }
  }

  // 设置结尾NULL
  argv[argc] = NULL;
  if (argc == 0){
   return 1;
  }

  if ((bg = (*argv[argc-1] == '&')) != 0){
    argv[--argc] = NULL;
  }

  return bg;
}

// gcc -o shellex.o shellex.c
// /bin/ls
// /bin/pwdi &
// ls
// quit
