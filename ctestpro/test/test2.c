#include <stdio.h>
//fork()为liunx的内置函数，在mac中需要倒入<unistd.h>，Linux中可以省略
#include <unistd.h>
int main(){
	int pid;
	pid=fork();
	if(pid == 0)
		printf("I am the child process!\n");
	else
		printf("I am the parent process!\n");
}
