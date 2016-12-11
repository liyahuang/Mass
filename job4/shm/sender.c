#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>// 定义了oflag
#include <string.h>
#include <stdlib.h>
#define SHMSZ 32
char sem_name[]="read";
char sem_name1[] = "write";
int main()
{
    key_t key = 1000;
    char ch ,buff[32];
    int shmid;
    char *shm;
    int i ;
    sem_t *mutex_read,*mutex_write;
    mutex_read = sem_open(sem_name,O_CREAT,0644,0);
    if(mutex_read == SEM_FAILED)
    {
        perror("unable to create semaphore");
        sem_unlink(sem_name);
        exit(EXIT_FAILURE);
    }
    mutex_write = sem_open(sem_name1,O_CREAT,0644,1);
    if(mutex_write == SEM_FAILED)
    {
        perror("unable to create semaphore");
        sem_unlink(sem_name1);
        exit(EXIT_FAILURE);
    }
    shmid = shmget(key,SHMSZ,IPC_CREAT|0666);
    if(shmid < 0)
    {
        perror("failure in shmget");
        exit(EXIT_FAILURE);
    }
    shm = shmat(shmid,NULL,0);
    while(1)
    {
        scanf("%s",buff);
        sem_wait(mutex_write);
        strcpy(shm,buff);
        sem_post(mutex_read);
        if(strcmp(buff,"exit") == 0)
            break;
        printf("sender - send msg: %s\n",buff);
    }
    sem_wait(mutex_write);
    strcpy(shm,"end");
    sem_post(mutex_read);
    printf("sender - send msg: %s\n","end");


    while(1)
    {
        sem_wait(mutex_read);
        strcpy(buff,shm);
        sem_post(mutex_write);
        printf("sender - get msg: %s\n",buff);
        if(strcmp(buff,"over") == 0)
            break;
    }
    
    sem_close(mutex_read);
    sem_unlink(sem_name);
    sem_close(mutex_write);
    sem_unlink(sem_name1);
    shmctl(shmid,IPC_RMID,NULL);
    exit(0);
    
}
