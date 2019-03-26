/*
 * 这是一个匿名管道的读写demo
 * 父子进程
 */
#include<stdio.h>
#include<unistd.h>
#include <errno.h>
#include <string.h>

//函数原型：
//int pipe(int fd[2])
int main()
{
    pid_t pid;
    int fd[2] = {0};
    if(pipe(fd) < 0)
    {
	perror("create fail:");
	return -1;
    }
    //创建子进程 fork()
    if((pid = fork()) < 0)
    {
	perror("fork fail:");
	return -1;
    }
    //父进程 写
    else if(pid > 0)
    {
	char* str = "troye sivan";
	close(fd[0]);
	write(fd[1],str,strlen(str));
	close(fd[1]);
    }
    //子进程 读
    else
    {
	close(fd[1]);
	char buff[256] = {0};
	read(fd[0],buff,11);
	printf("%s",buff);
	close(fd[0]);

    }
    return 0;
}
