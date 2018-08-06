#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 10000
int main()
{
	int i, num;
	FILE *fp = fopen("array.txt","w");
	srand(time(NULL));
	for(i = 0; i < MAX_SIZE; i++){
		num = rand() % 300000;
		fprintf(fp,"%d ",num);
	}
	fclose(fp);
	return 0;
}
