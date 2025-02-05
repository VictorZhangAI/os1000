#include "user.h"

void main(void)
{
	while(1)
	{
prompt:
		printf("\n$ ");
		char cmdline[128];
		for(uint32_t i = 0;;i++)
		{
			char ch = getchar();
			putchar(ch);
			if(i >= sizeof(cmdline) - 1)
			{
				printf("ERROR: too long command\n");
				goto prompt;
			}
			else if(ch == '\r')
			{
				printf("\n");
				cmdline[i] = '\0';
				break;
			}
			else
			{
				cmdline[i] = ch;
			}
		}
		if(strcmp(cmdline, "hello") == 0)
			printf("Hello from VZstless!\n");
		else if(strcmp(cmdline, "exit") == 0)
			exit();
		else
			printf("Unknown command!\n");
	}
}
