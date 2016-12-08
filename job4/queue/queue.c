#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#define MSG_FILE "queue.c"
#define MSG_NUM 1024
#define BUFFER 255
#define PERM S_IRUSR|S_IWUSR
struct msgbuf
{
	long mtype;
	char mtext[BUFFER+1];
}
void send()
{
	struct msgbuf msg;
	key_t key;
	int msgid;
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
	msg.mtype = 1; 
	while(1)
	{
		scanf("%s",s_buf);
		strcpy(msg.mtext,message);
		if(strcmp(s_buf,"exit");
		{
			strcpy(msg.mtext,"end");
			msgsnd(msgid,&msg,sizeof(struct msgbuf),0);
			//发送内容为"end"的消息
			break;
		}
		msgsnd(msgid,&msg,sizeof(struct msgbuf),0);
		//发送消息
	}

}
void receive()
{
	
}
int main()
{
	
	
	
}
