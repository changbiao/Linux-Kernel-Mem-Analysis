#include <stdio.h>
#include <stdlib.h>

#define SIZE 4096

int list[2*SIZE];

int main()
{
	int i;
	for(i = 0; i < 2*SIZE; i++)
	{
		list[i] = i;
	}
}
