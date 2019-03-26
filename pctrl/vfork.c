#include <stdio.h>
#include <stdlib.h>
int main()
{
    pid_t pid = vfork();
    if(pid < 0)
    {
	perror("vfork");
    }
    else if(pid == 0)
    {
	sleep(5);
	printf("this is child!\n");
	exit(0);
    }
    else
    {
	printf("this is parent!!\n");
	sleep(1);
	
    }
    return 0;
}
