#include<linux/unistd.h>
#include<sys/syscall.h>

int main(void)
{
	syscall(330,5365,0,10);
	return 0;
}
