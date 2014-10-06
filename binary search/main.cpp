#include "stdio.h"

int bsearch ( int *array, int n, int key)
{
	int left = 0;
	int rigth = n - 1;
	int result = n - 1;
	
	while(left <= rigth)
	{
		if((array[left] > array[rigth]))
			return -1;
			
		int mid = (left + rigth)/2;
		
		if(key == array[mid])
		{
			result = mid;
			break;
		}
		if(key > array[mid])
		{
			left = mid + 1;
		}
		else if(key < array[mid])
		{
			rigth = mid - 1;
		}
	}
	
	return result;
}

int ms[] = { 15, 16, 20, 22,23,24,200};

int main()
{
	printf("%d", bsearch( ms, 7, 24));

	return 0;
}
