#include<linux/unistd.h>
#include<sys/syscall.h>

int main(void)
{
	syscall(330,3844,1,25);
	return 0;
}
