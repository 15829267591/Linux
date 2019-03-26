#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
int main()
{
    while(1)
    {
	printf("minishell: ");
	char cmd[1024] ={0};
	if(scanf("%[^\n]%*c",cmd)!=1)
	{
	    getchar();
	}
	//解析获取到的命令
	char* ptr = cmd;
	char* argv[32] = {NULL};
	int argc = 0;
	argv[argc++] = ptr;
	while(*ptr != '\0')
	{
	    if(isspace(*ptr)){
	    while(isspace(*ptr)&&*ptr != '\0'){
	    *ptr++ = '\0';
	    }
	    
	    argv[argc++] = ptr;
	   }
         	ptr++;
        }
	if(0 == fork())
	{
	    execvp(argv[0],argv);
	}
	wait(NULL);
    }
    return 0;
}
