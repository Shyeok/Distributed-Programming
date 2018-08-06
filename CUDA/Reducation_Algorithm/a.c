#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define GET_TIME(now) { \
	struct timeval t; \
	gettimeofday(&t, NULL); \
	now = t.tv_sec * 1000 + t.tv_usec / 1000.0; \
}
#define MAX_SIZE 15000

int arr[MAX_SIZE];

int main()
{
	int i;
	int num = 0;
	double start_time, end_time;
	FILE *fp = fopen("array.txt","r");

	for(i = 0; i < MAX_SIZE; i++){
		fscanf(fp,"%d",&arr[i]);
	}

	GET_TIME(start_time);
	for(i = 0; i < MAX_SIZE; i++){
		if(num < arr[i]){
			num = arr[i];
		}
	}
	GET_TIME(end_time);
	printf("MAXIMUM NUMBER IS %d\n",num);
	printf("VER A: Elapsed time is %e (msec)\n",end_time - start_time);
	return 0;
}
