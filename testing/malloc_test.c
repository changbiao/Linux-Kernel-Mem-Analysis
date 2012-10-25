#include <stdio.h>
#include <stdlib.h>

int main()
{
	int size = 4096;
	int faults = 5;
	int j = 0;
	int i = 0;
	int * heap;

	
	heap = (int *) malloc(faults * size * sizeof(int*));
	for(i = 0; i <= size; i+=4096)
	{
		heap[i] = j;	
		j++;
	}
}
