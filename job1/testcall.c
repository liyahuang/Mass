#include<linux/unistd.h>
#include<sys/syscall.h>

int main(void)
{
	syscall(329);
	return 0;
}
