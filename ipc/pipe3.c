/*
 * 这是一个测试demo
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE PIPE_BUF
void err_quit(char* msg)
{
    printf(msg);
    exit(1);
}
int main()
{
    int fd[2];
    pid_t pid;
    int len;
    char buff[BUFSIZE] = "hello world\n";
    if(pipe(fd) < 0)
    {
	err_quit("pipe falied\n");
    }
    if((pid = fork()) < 0)
    {
	err_quit("fork failed\n");
    }
    else if(pid > 0)
    {
	close(fd[0]);
	write(fd[1],buff,strlen(buff));
	exit(0);
    }
    else
    {
	close(fd[1]);
	len = read(fd[0],buff,BUFSIZE);
	if(len < 0)
	{
	    err_quit("process failed when read a pipe\n");

	}
	else
	{
	    write(STDOUT_FILENO,buff ,len);
	}
	exit(0);
    }
    
}
