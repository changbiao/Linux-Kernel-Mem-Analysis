#include <stdio.h>
#include <stdlib.h>

#define SIZE 4096
#define MULTI 1024

int list[SIZE];

int main()
{
	int i;
	for(i = 0; i < MULTI*SIZE; i++)
	{
		list[i] = i;
	}
}
