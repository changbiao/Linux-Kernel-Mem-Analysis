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
	}else{
		i=i+2;
		printf("%d\n", i);
	}
	return 0;
}

