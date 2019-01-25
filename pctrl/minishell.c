#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024
#define NUM 32
int main()
{
    char cmd[MAX];
    char* myargv[NUM];
    for(;;){
    printf("[chuu@localhost pctrl]#" );
    fflush(stdout);
    fgets(cmd,MAX, stdin);
    cmd[strlen(cmd)-1] = 0;

    int i = 0;
    myargv[i] = strtok(cmd, " ");
    i++;
    while(1)
    {
    char* p = strtok(NULL, " ");
    if(p == NULL)
    {
	myargv[i] = NULL;
	break;
    }
    myargv[i] = p;
    i++;

    }
    //for(i=0; myargv[i] != NULL; i++)
   // printf("%d :%s\n",i, myargv[i]);
   pid_t id = fork();
   if(id < 0)
   {
	printf("error");
	exit(1);
   }
   else if(id == 0)
   {
	
	execvp(myargv[0],myargv);
   }else{
       waitpid(id , NULL ,0);
   }
}
    return 0;
}
