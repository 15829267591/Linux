/*
 *这是一个匿名管道创建的demo
 * */
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    char buff[256] = {0};
    int ret = pipe(fd);
    if(ret < 0)
    {
	perror("pipe error!");
	return -1;
    }
    //xie
    
    write(fd[1],"hello troye!",13);
    read(fd[0],buff,sizeof(buff));
    printf("%s",buff);
    printf("pipe file descripe are %d %d\n",fd[0],fd[1]);
    close(fd[0]);
    close(fd[1]);
    return 0;
}
