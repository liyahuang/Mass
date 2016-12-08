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
if(-1 == sem_init(&sem,0,1))
{
	fprintf(stderr,"semaphore init error: %s\n",strerror(errno));
	exit(EXIT_FAILURE);
}
struct msgbuf
{
	long mtype;
	char mtext[BUFFER+1];
}
void send()
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
	msg.mtype = SENDER_FLAG; 
	do
	{
		scanf("%s",message);
		strncpy(msg.mtext,message,BUFFER);
		if(-1 == msgsnd(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),0))
		{
			fprintf(stderr,"msg send error: %s",strerror(errno));
		}
	}
	while(strcmp(message,"exit") != 0);
	strcpy(msg.mtext,"end");
	msgsnd(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),0);//send "end" message
	
	//接收
	do
	{
		memset(&msg,'\0',sizeof(struct msgbuf));
		msgrcv(msqid,&msg,sizeof(struct msgbuf)-sizeof(long),RECEIVER_FLAG,0);
	}while( 0 != strcmp(msg.mtext,"over"));
	//结束程序运行
	//exit(0);
	
}
void receive()
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
		msgrcv(msqid,&msg,BUFFER+1,SENDER_FLAG,0);
		if()
		{
			
		}
	}while( 0 != strcmp(msg.mtext,"end"));
	msg.mtype = RECEIVER_FLAG;
	strcpy(msg.mtext,"over");
	msgsnd(msqid,&msg,BUFFER+1,0);
}
int main()
{
	
	
	
}
