 /***********************************************
 * This is a test code to achieve communiating
 * between Parent Process and Child Process.
 * *********************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <fcntl.h>


char SEM_NAME0[] = "GB";
char SEM_NAME1[] = "SEM_NAME1_o";
char SEM_NAME2[] = "SEM_NAME2_o";
char SEM_NAME3[] = "SEM_NAME3_o";

#define BUFFER_SIZE 255

int main(void)
{
	/*************************************************
	 * Because we can regard a pipe(fd) as a file, and
	 * the fd[0] is the reading port, the fd[1] is the 
	 * writing port. So we create an array.
	 * **********************************************/
	
	int fd[2], pid_1, pid_2, pid_3, i = 0;
	sem_t *mutex0, *mutex1, *mutex2, *mutex3;
	mutex0 = sem_open(SEM_NAME0, O_CREAT, 0644, 1);
	mutex1 = sem_open(SEM_NAME1, O_CREAT, 0644, 0);
	mutex2 = sem_open(SEM_NAME2, O_CREAT, 0644, 0);
	mutex3 = sem_open(SEM_NAME3, O_CREAT, 0644, 0);
	char buf1[BUFFER_SIZE] = "Child_1 is sending a message!";
	char buf2[BUFFER_SIZE] = "Child_2 is sending a message!";
	char buf3[BUFFER_SIZE] = "Child_3 is sending a message!";
	char buf_read[BUFFER_SIZE];
	
	if(mutex0 == SEM_FAILED) {
		sem_unlink(SEM_NAME0);
		exit(-1);
	}
	if(mutex1 == SEM_FAILED) {	
		sem_unlink(SEM_NAME1);
		exit(-1);
	}
	if(mutex2 == SEM_FAILED) {
		sem_unlink(SEM_NAME2);
		exit(-1);
	}
	if(mutex3 == SEM_FAILED) {
		sem_unlink(SEM_NAME3);
		exit(-1);
	}
	//Create a pipe and the pipe must be created by 
	//parent process, and child processes can use it.
	/***************************************************
	 * When we create the array successful, then it will
	 * return 0, or return -1 and the failed reason will
	 * be saved in errno.
	 * There are three kinds of error:
	 * 1. EMFILE means the process has run out of file 
	 * description of the most large number of words.
	 * 2. ENFILE means system has no file description
	 * available.
	 * 3. EFAULT the filedes is illegal.
	 * *************************************************/
	if((pipe(fd)) == -1) {
		printf("Create pipe error!\n");
		return -1;
	}

	//Create a child process
	while((pid_1 = fork()) == -1);
	
	if(pid_1 == 0) {
		sem_wait(mutex0);
		close(fd[0]);
		lockf(fd[1], 1, 0);
		if(write(fd[1], buf1, BUFFER_SIZE) == -1) {
			printf("Write Failed!\n");
			return -1;
		}
		else{		  
//			sleep(3);
			lockf(fd[1], 0, 0);
			close(fd[1]);
			sem_post(mutex0);
			sem_post(mutex1);
		}
	}
	else {
		while((pid_2 = fork()) == -1); //Create second child process
		if(pid_2 == 0) {
			sem_wait(mutex0);
			close(fd[0]);
			lockf(fd[1], 1, 0);
			if(write(fd[1], buf2, BUFFER_SIZE) == -1) {
				printf("Write Failed!\n");
				return -1;
			}
			else {	
//				sleep(3);
				lockf(fd[1], 0, 0);
				close(fd[1]);
				sem_post(mutex0);
				sem_post(mutex2);
			}
		}
		else {
			while((pid_3 = fork()) == -1); //Create third child process
			if(pid_3 == 0) {
				sem_wait(mutex0);
				close(fd[0]);
				lockf(fd[1], 1, 0);
				if(write(fd[1], buf3, BUFFER_SIZE) == -1) {
					printf("Write Failed!\n");
					return -1;
				}
				else {	
					lockf(fd[1], 0, 0);
					close(fd[1]);
					sem_post(mutex0);
					sem_post(mutex3);
				}
			}
			else {
				sem_wait(mutex1);
				sem_wait(mutex2);
				sem_wait(mutex3);
				close(fd[1]);
				lockf(fd[0], 1, 0);
				read(fd[0], buf_read, 255);
				printf("%s\n", buf_read);
				read(fd[0], buf_read, 255);
				printf("%s\n", buf_read);
				read(fd[0], buf_read, 255);
				printf("%s\n", buf_read);
				lockf(fd[0], 0, 0);
			}
		}
	}
	sem_close(mutex0);
	sem_close(mutex1);
	sem_close(mutex2);
	sem_close(mutex3);
	sem_unlink(SEM_NAME0);
	sem_unlink(SEM_NAME1);
	sem_unlink(SEM_NAME2);
	sem_unlink(SEM_NAME3);
	return 0;
}


