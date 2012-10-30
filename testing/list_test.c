#include <stdio.h>
#include <stdlib.h>

#define SIZE 4096

int list[SIZE];

int main()
{
	int i;
	for(i = 0; i < SIZE; i++)
	{
		list[i] = i;
	}
}
