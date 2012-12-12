#include <stdio.h>
#include <stdlib.h>


int main()
{
	int i = 0;
	int pid = fork();
	if(pid == 0)
	{
		i++;	//child
		printf("%d\n", i);
	}
	printf("%d\n", i);
}

