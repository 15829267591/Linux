/*
 *这是一个命名管道的demo，实现一个通信功能
 *
 */
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
int main()
{
    int fd = open("troye",O_WRONLY);
    if(fd < 0)
    {
	perror("open error:");
	return -1;
    }
    printf("open fifo successful!\n");
   while(1){ 
    char buf[1024] = {0};
    printf("input:");
    fflush(stdout);
    scanf("%s",buf);
    write(fd,buf,1024);
    }
    close(fd);
    return 0;
}
