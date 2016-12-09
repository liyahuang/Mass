#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#define MSG_FILE "queue.c"
#define MSG_NUM 10
#define BUFFER 255
#define SENDER_FLAG 1
#define RECEIVER_FLAG 2
#define PERM S_IRUSR|S_IWUSR
sem_t sem_full_1,sem_empty_1;
sem_t sem_full_2,sem_empty_2;

typedef struct msgbuf
{
	long mtype;
	char mtext[BUFFER+1];
}msgbuf;
void *send()
{
	struct msgbuf msg;
	key_t key;
	int msqid;
	char message[BUFFER+1];
	int res = 0;
	if (-1 == (key = ftok(MSG_FILE,MSG_NUM)))
	{
		fprintf(stderr,"creat key error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(-1 == (msqid= msgget(key,PERM|IPC_CREAT)))
	{
		fprintf(stderr,"creat msg queue error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	msg.mtype = SENDER_FLAG; 
	while (1)
	{
		printf("input message:\n");
		scanf("%s",message);
		strncpy(msg.mtext,message,BUFFER);
		printf("sender - send msg :%s\n",msg.mtext);


		if(strcmp(message,"exit") == 0)//"exit" msg
		{
			//发送"end"
			strcpy(msg.mtext,"end");
			sem_wait(&sem_empty_1);
			res = msgsnd(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),0);
			sem_post(&sem_full_1);
			if(-1 == res)
			{
				fprintf(stderr,"msg send error: %s",strerror(errno));
			}
			break;
		}
		//非"exit" msg
		sem_wait(&sem_empty_1);
		res = msgsnd(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),0);
		sem_post(&sem_full_1);
		if(-1 == res)
		{
			fprintf(stderr,"msg send error: %s",strerror(errno));
		}
	}
	
	//接收
	do
	{
		sem_wait(&sem_full_2);
		msgrcv(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),RECEIVER_FLAG,0);
		sem_post(&sem_empty_2);
	}while( 0 != strcmp(msg.mtext,"over"));
	printf("sender - get msg:%s",msg.mtext);
	if(msgctl(msqid,IPC_RMID,NULL)<0)
	{
		perror("msgctl");
		exit(1);
	}
	//结束程序运行
	pthread_exit(NULL);
	
}
void *receive()
{
	key_t key;
	int msqid;
	msgbuf msg;
	if (-1 == (key = ftok(MSG_FILE,MSG_NUM)))
	{
		fprintf(stderr,"creat key error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
    if(-1 == (msqid=msgget(key,PERM|IPC_CREAT)))
    {
        fprintf(stderr,"Creat MessageQueue Error：%s \n",strerror(errno));
        exit(EXIT_FAILURE);
    }

	do
	{
		sem_wait(&sem_full_1);
		msgrcv(msqid,&msg,BUFFER+1,SENDER_FLAG,0);
		sem_post(&sem_empty_1);
		printf("receiver - get msg: %s\n",msg.mtext);
	}while( 0 != strcmp(msg.mtext,"end"));
	msg.mtype = RECEIVER_FLAG;
	strcpy(msg.mtext,"over");
	sem_wait(&sem_empty_2);
	msgsnd(msqid,&msg,BUFFER+1,0);
	sem_post(&sem_full_2);
	pthread_exit(NULL);

	sem_wait(&sem_full_1);
	msgrcv(msqid,&msg,BUFFER+1,SENDER_FLAG,0);
	sem_post(&sem_empty_2);
	printf("receiver - get msg: %s\n",msg.mtext);
}
int main()
{
	if(-1 == sem_init(&sem_full_1,0,0))//已有的mtype = 1消息的数目
	{
		fprintf(stderr,"semaphore init error: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(-1 == sem_init(&sem_empty_1,0,MSG_NUM))//消息队列中的空闲数
	{
		fprintf(stderr,"semaphore init error: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(-1 == sem_init(&sem_full_2,0,0))//已有的mtype = 2消息的数目
	{
		fprintf(stderr,"semaphore init error: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(-1 == sem_init(&sem_empty_2,0,1))//消息队列中的空闲数
	{
		fprintf(stderr,"semaphore init error: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	pthread_t s_thread,r_thread;
	if(-1 == pthread_create(&s_thread,NULL,send,NULL))
	{
		fprintf(stdout,"create send thread error: %s",strerror(errno));
	}
	if(-1 == pthread_create(&r_thread,NULL,receive,NULL))
	{
		fprintf(stdout,"create receive thread error: %s",strerror(errno));
	}  
	if(0 != pthread_join(s_thread, NULL))  
    {  
        perror("pthread_join failed\n");  
        exit(EXIT_FAILURE);  
    }  
    if(0 != pthread_join(r_thread, NULL))  
    {  
        perror("pthread_join failed\n");  
        exit(EXIT_FAILURE);  
    }  
	
	
	sem_destroy(&sem_full_1);
}
