#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(void)
{
	char cmd[5];
	printf("input your command\n");
	while(1)
	{
		fflush(stdin);
		scanf("%s",cmd);
		if(0 == strcmp(cmd,"cmd1"))
		{
			system("./cmd1.exe");
		}
		else if(0 == strcmp(cmd,"cmd2"))
		{
			system("./cmd2.exe");
		}
		else if(0 == strcmp(cmd,"cmd3"))
		{
			system("./cmd3.exe");
		}
		else if(0 == strcmp(cmd,"exit"))
		{
			return 0;
		}
		else
		{
			printf("invalid command!\n");
		}		
	}
}
