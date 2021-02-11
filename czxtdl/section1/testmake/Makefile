
#一个 Makefile 中通常包含如下内容:
#1)需要由 make 工具创建的目标体(target),通常是目标文件或可执行文件;
#2)要创建的目标体所依赖的文件(dependency_file);
#3)创建每个目标体时需要运行的命令(command)。
#格式为:
#target: dependency_files
#	command //command 前必须有“Tab”符

#例如,有两个文件分别为 hello.c 和 hello.h,创建的目标体为 hello.o
#The simplest example
hello.o: hello.c hello.h
	gcc -c hello.c -o hello.o

#make 的格式为 make target
#执行make hello.o
