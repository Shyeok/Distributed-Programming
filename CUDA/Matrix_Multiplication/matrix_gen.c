#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 1024

int main()
{
	int i, num;
	int n;
	FILE *fp = fopen("matrix.txt","w");

	srand(time(NULL));

	n = MAX_SIZE * MAX_SIZE;

	for(i = 0; i < n; i++){
		num = rand() % 100;
		fprintf(fp,"%d ",num);
	}
	fprintf(fp,"\n");

	for(i = 0; i < n; i++){
		num = rand() % 100;
		fprintf(fp,"%d ",num);
	}
	return 0;
}
