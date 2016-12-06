//pipeDemo.c
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#define BUFNUM 128
#define PIPECAPACITY (1024*64)
int main(void)
{
	sem_t mutex_0;
    int n;
    int fd[2];
    pid_t pid[3];
    char buf[3][BUFNUM] = {"context0","context1","context2"};
    char read_buf[3*BUFNUM+1]="";
//    char sem_name[][]={"R","W0","W1","W2"};
    
    //创建管道
    if (pipe(fd) < 0)
    {
        printf("Creat Pipe Error：%s \n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    while(-1 == (pid[0] = fork()));//创建第一个子进程
    if (0 == pid[0])//第一个子进程
    {
		close(fd[0]);
    	n = write(pid[1],buf[0],BUFNUM);
		printf("write0,%d\n",n);	
    }
    else//父进程
    {
    	
    	while(-1 == (pid[1] = fork()));//创建第二个进程
    	if(0 == pid[1])//第二个子进程
    	{
    		close(fd[0]);
    		n = write(pid[1],buf[1],BUFNUM);
			printf("write1,%d\n",n);
    	}
    	else//父进程
    	{
    		//创建第三个进程;
    		while(-1 == (pid[2] = fork()));
    		if(0 == pid[2] )
    		{
    			close(fd[0]);
    			n = write(pid[1],buf[2],BUFNUM);
				printf("write2,%d\n",n);	
		}
    		else
    		{
    			close(fd[1]);
				n = read(pid[0],read_buf,3*BUFNUM+1);
				read_buf[3*BUFNUM]='\0';
				printf("read:%s,length = %d\n",read_buf,n);
    		}
    		
    	}  	
    	
    }
    return 0;
}
