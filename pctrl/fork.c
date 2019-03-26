#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    pid_t pid ;
    printf("before: pid is %d\n",getpid());
    pid = fork();
    if(pid < 0)
    {
	perror("fork()");
	exit(1);
    }
    else if(pid == 0)
    {
	printf("After: pid is %d, 返回值是%d\n",getpid(),pid);

    }
    else
    {
       printf("After: pid is %d,返回值是%d\n",getpid(),pid);
       sleep(1);
    }

	
    return 0;
}
