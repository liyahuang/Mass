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
#define MSG_NUM 1024
#define BUFFER 255
#define SENDER_FLAG 1
#define RECEIVER_FLAG 2
#define PERM S_IRUSR|S_IWUSR
sem_t sem;

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
	char *myask = "I'm receiver";
	int ret_value;
	char message[BUFFER+1];
	
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
	printf("snd key: %d\n",key);
	msg.mtype = SENDER_FLAG; 
	while (1)
	{
		puts("input message:\n");
		scanf("%s",message);
		strncpy(msg.mtext,message,BUFFER);
		printf("msg send:%s\n",msg.mtext);
		//sem_wait(&sem);

		if(strcmp(message,"exit") == 0)
		{
			//发送"end"
			strcpy(msg.mtext,"end");
			if(-1 == msgsnd(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),0))
			{
				fprintf(stderr,"msg send error: %s",strerror(errno));
			}
			break;
		}
		if(-1 == msgsnd(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),0))
		{
			fprintf(stderr,"msg send error: %s",strerror(errno));
		}
		//sem_post(&sem);
	}
	
	//sem_wait(&sem);
	msgsnd(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),0);//send "end" message
	//sem_post(&sem);
	//接收
	do
	{
		memset(&msg,'\0',sizeof(struct msgbuf));
		msgrcv(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),RECEIVER_FLAG,0);
	}while( 0 != strcmp(msg.mtext,"over"));
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
	printf("rcv key: %d\n",msqid);
	do
	{
		//sem_wait(&sem);
		msgrcv(msqid,&msg,BUFFER+1,SENDER_FLAG,0);
		printf("receive message: %s\n",msg.mtext);
		//sem_post(&sem);
	}while( 0 != strcmp(msg.mtext,"end"));
	msg.mtype = RECEIVER_FLAG;
	strcpy(msg.mtext,"over");
	//sem_wait(&sem);
	msgsnd(msqid,&msg,BUFFER+1,0);
	//sem_post(&sem);
	pthread_exit(NULL);
}
int main()
{
	if(-1 == sem_init(&sem,0,1))
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
	if(msgctl(0,IPC_RMID,NULL)<0)
	{
		perror("msgctl");
		exit(1);
	}
	
	sem_destroy(&sem);
}
