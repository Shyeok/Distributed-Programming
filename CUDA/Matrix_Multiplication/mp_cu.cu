#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX_SIZE 1024

#define GET_TIME(now){ \
	struct timeval t; \
	gettimeofday(&t, NULL); \
	now = t.tv_sec * 1000 + t.tv_usec / 1000.0; \
}

int arr[MAX_SIZE * MAX_SIZE];
int brr[MAX_SIZE * MAX_SIZE];
int crr[MAX_SIZE * MAX_SIZE];

__global__ void mp(int *a, int *b, int *c)
{
	int i, sum;
	int id_0, id_1, id_2;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int row = blockIdx.y * blockDim.y + threadIdx.y;

	sum = 0;

	for(i = 0; i < MAX_SIZE; i++){
		id_0 = row * MAX_SIZE + i;
		id_1 = i * MAX_SIZE + col;
		sum += a[id_0] * b[id_1];
	}

	id_2 = row * MAX_SIZE + col;
	c[id_2] = sum;
}

int main(void) {

	int *d_a, *d_b, *d_c;
	int block_size;
	int size = sizeof(int) * MAX_SIZE * MAX_SIZE;
	int i, j;
	double start_time, end_time;

	FILE *fp = fopen("matrix.txt","r");

	printf("Input the Block Size: ");
	scanf("%d",&block_size);

	for(i = 0; i < MAX_SIZE; i++){
		for(j = 0; j < MAX_SIZE; j++){
			fscanf(fp,"%d",&arr[i * MAX_SIZE + j]);
		}
	}

	for(i = 0; i < MAX_SIZE; i++){
		for(j = 0; j < MAX_SIZE; j++){
			fscanf(fp,"%d",&brr[i * MAX_SIZE + j]);
		}
	}

	fclose(fp);

	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);
	cudaMalloc((void **)&d_c, size);

	cudaMemcpy(d_a, arr, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, brr, size, cudaMemcpyHostToDevice);

	GET_TIME(start_time);
	dim3 dimBlock (block_size, block_size);
	dim3 dimGrid(MAX_SIZE / dimBlock.x, MAX_SIZE/dimBlock.y);
	mp<<<dimGrid, dimBlock>>>(d_a, d_b, d_c);
	GET_TIME(end_time);

	cudaMemcpy(crr, d_c, size, cudaMemcpyDeviceToHost);

	fp = fopen("matrix_cu.txt","w");
	for(i = 0; i < MAX_SIZE; i++){
		for(j = 0; j < MAX_SIZE; j++){
			fprintf(fp,"%d ",crr[i * MAX_SIZE + j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);

	printf("Matrix Mulitplication Cuda VER: Elapsed time is %e (msec)\n",end_time - start_time);

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

	return 0;
}
