#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAX_SIZE 1024
int arr[MAX_SIZE][MAX_SIZE];
int brr[MAX_SIZE][MAX_SIZE];
int crr[MAX_SIZE][MAX_SIZE];

int main()
{
	int i, j, k;
	int sum = 0;
	double start_time, end_time;
	FILE *fp = fopen("matrix.txt","r");

	for(i = 0; i < MAX_SIZE; i++){
		for(j = 0; j < MAX_SIZE; j++){
			fscanf(fp,"%d",&arr[i][j]);
		}
	}

	for(i = 0; i < MAX_SIZE; i++){
		for(j = 0; j < MAX_SIZE; j++){
			fscanf(fp,"%d",&brr[i][j]);
		}
	}
	fclose(fp);

	start_time = omp_get_wtime();
	for(i = 0; i < MAX_SIZE; i++){
		for(j = 0; j < MAX_SIZE; j++){
			sum = 0;
			for(k = 0; k < MAX_SIZE; k++){
				sum += (arr[i][k] * brr[k][j]);
			}
			crr[i][j] = sum;
		}
	}
	end_time = omp_get_wtime();

	fp = fopen("matrix_c.txt","w");
	for(i = 0; i < MAX_SIZE; i++){
		for(j = 0; j < MAX_SIZE; j++){
			fprintf(fp,"%d ",crr[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);

	printf("Matrix Multiplication ver C: Elapsed Time is %f sec\n",end_time - start_time);

	return 0;
}
