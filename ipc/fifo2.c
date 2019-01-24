#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
int main()
{
   //扮演通信中的服务器  
    umask(0);
    if(mkfifo("troye",0664) < 0)
    {
	if(errno == EEXIST)
	{
	    printf("fifo exit.\n");	    
	}
	perror("fifo:");
	return -1;
    }
    int fd = open("troye",O_RDONLY);
    if(fd < 0)
    {
	perror("open error:");
	return -1;
    }
	printf("open fifo successful!\n");
    while(1){
	char buf[1024];
	memset(buf, 0x00, 1024);
	int ret = read(fd,buf,1024);
	if(ret > 0)
	{
	    printf("client say:%s\n",buf);
	}
    }
    close(fd);
    return 0;
}

