//pipeDemo.c
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#define BUFNUM 256
#define PIPECAPACITY (1024*64)
int main(void)
{
	sem_t mutex_0,mutex_1,mutex_2,mutex_3;
    int n;
    int fd[2];
    pid_t pid[3];
    char *buf[3] = {"1234","12345","123456"};
    char read_buf[3*BUFNUM+1]="";
    // int sem_init(sem_t *sem, int pshared用于相关进程间的同步, unsigned int value);
    sem_init(&mutex_0,1,1);
    sem_init(&mutex_1,1,0);
    sem_init(&mutex_2,1,0);
    sem_init(&mutex_3,1,0);
    
    //创建管道
    if (pipe(fd) < 0)
    {
        printf("Creat Pipe Error：%s \n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    while(-1 == (pid[0] = fork()));//创建第一个子进程
    if (0 == pid[0])//第一个子进程
    {
    	sleep(1);
    	sem_wait(&mutex_0);
		close(fd[0]);
    	n = write(fd[1],buf[0],4);
		printf("write0,%d\n",n);
		fflush(stdout);
		sem_post(&mutex_0);
		sem_post(&mutex_1);	
    }
    else//父进程
    {
    	
    	while(-1 == (pid[1] = fork()));//创建第二个进程
    	if(0 == pid[1])//第二个子进程
    	{
    		//sem_wait(&mutex_1);
    		sleep(1);
    		sem_wait(&mutex_0);
    		close(fd[0]);
    		n = write(fd[1],buf[1],5);
			printf("write1,%d\n",n);
			
				fflush(stdout);
			sem_post(&mutex_0);
		//	sem_post(&mutex_1);
			sem_post(&mutex_2);
    	}
    	else//父进程
    	{
    		//创建第三个进程;
    		while(-1 == (pid[2] = fork()));
    		if(0 == pid[2] )
    		{
    			//sem_wait(&mutex_2);
    			//sem_wait(&mutex_1);
    			sleep(1);
    			sem_wait(&mutex_0);
    			close(fd[0]);
    			n = write(fd[1],buf[2],6);
				printf("write2,%d\n",n);	
				fflush(stdout);
				sem_post(&mutex_0);
				//sem_post(&mutex_1);
				//sem_post(&mutex_2);
				sem_post(&mutex_3);
		}
    		else
    		{
    			/*sem_wait(&mutex_3);
    			sem_wait(&mutex_2);
    			sem_wait(&mutex_1);*/
    			
	  			sem_wait(&mutex_0);
    			close(fd[1]);
				n = read(fd[0],read_buf,100);
				read_buf[BUFNUM]='\0';
				printf("read:%s,length = %d\n",read_buf,n);
				fflush(stdout);
				n = read(fd[0],read_buf,50);
				printf("read:%s,length = %d\n",read_buf,n);
				fflush(stdout);
				n = read(fd[0],read_buf,60);
				printf("read:%s,length = %d\n",read_buf,n);
				fflush(stdout);
				sem_post(&mutex_0);
				/*sem_post(&mutex_1);
				sem_post(&mutex_2);
				sem_post(&mutex_3);*/
    		}
    		
    	}  	
    	
    }
    return 0;
}
