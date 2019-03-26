/*
 *这是一个实现管道符的demo
 */

#include <stdio.h>
#include<unistd.h>
int main()
{
    pid_t pid;
    int fd[2] = {0};
    if(pipe(fd) < 0)
    {
	perror("pipe error:");
	return -1;
    }
    if((pid = fork()) < 0)
    {
	perror("fork error:");
	return -1;
    }
    else if(pid == 0){
	//子进程，将 ps -ef 的结果作为grep ssh 命令的输入
	close(0);
	dup2(fd[0],0);
	close(fd[1]);
	execl("/bin/grep", "grep", "ssh",NULL);
    }
    else{
	//父进程，ps -ef的输出重定向到管道的输入（写）fd[1] 
	close(1);
	dup2(fd[1],1);
	close(fd[0]);
	execl("/bin/ps","ps","-ef", NULL);
    }
    return 0;
}

