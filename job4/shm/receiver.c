#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define SHMSZ 32
char sem_name[]="read";
char sem_name1[] = "write";
int main()
{
    key_t key = 1000;
    char buff[SHMSZ];
    char *shm;
    int shmid;
    sem_t *mutex_write,*mutex_read;
    mutex_read = sem_open(sem_name,0,0644,0);
    if(SEM_FAILED == mutex_read)
    {
        perror("receiver - unable to execute semaphore");
        sem_close(mutex_read);
        exit(-1);
    }

    mutex_write = sem_open(sem_name1,0,0644,0);
    if(SEM_FAILED == mutex_write)
    {
        perror("receiver - unable to execute semaphore");
        sem_close(mutex_write);
        exit(-1);
    }
    shmid = shmget(key,SHMSZ,0666);
    if(shmid<0)
    {
    perror("reader:failure in shmget");
    exit(-1);
    }
    shm = shmat(shmid,NULL,0);
    
    while(1)
    {
        sem_wait(mutex_read);
        strcpy(buff,shm);
        sem_post(mutex_write);
        printf("receiver - get msg: %s\n",buff);
        if(strcmp(buff,"exit") == 0)
        {
            break;
        }
    }
    sem_wait(mutex_write);
    strcpy(shm,"over");
    sem_post(mutex_read);
    printf("receiver - send msg: %s\n","over");
    // sem_close(mutex_read);
    // sem_close(mutex_write);
    // shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}