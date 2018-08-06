#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX_SIZE 10000

#define GET_TIME(now){ \
	struct timeval t; \
	gettimeofday(&t, NULL); \
	now = t.tv_sec * 1000 + t.tv_usec / 1000.0; \
}

int arr[MAX_SIZE];
int brr[MAX_SIZE];

__global__ void mp(int *a, int *b)
{
	extern __shared__ int srr[];
	int i;
	int id_0 = blockIdx.x * blockDim.x;
	int id_1;

	srr[threadIdx.x] = a[id_0 + threadIdx.x];
	__syncthreads();

	for(i = 1 ;; i=i*2){
		if(i >= blockDim.x)break;
		id_1 = i * 2 * threadIdx.x;
		if(id_1 + i < blockDim.x){
			if(srr[id_1] < srr[id_1 + i]){
				srr[id_1] = srr[id_1 + i];
			}
		}
		__syncthreads();
	}
	if(threadIdx.x == 0)b[blockIdx.x] = srr[0];

}

int main(void) {

	int *d_a, *d_b;
	int block_size;
	int size = sizeof(int) * MAX_SIZE;
	int i, n, num;
	double start_time, end_time;

	FILE *fp = fopen("array.txt","r");

	printf("Input the Block Size: ");
	scanf("%d",&block_size);

	for(i = 0; i < MAX_SIZE; i++){
		fscanf(fp,"%d",&arr[i]);
	}

	fclose(fp);

	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);

	cudaMemcpy(d_a, arr, size, cudaMemcpyHostToDevice);

	GET_TIME(start_time);
	dim3 dimBlock (block_size, 1);
	if(MAX_SIZE / dimBlock.x != 0)n = 1;
	else{
		n = 0;
	}
	size = sizeof(int) * 1000;
	dim3 dimGrid(MAX_SIZE / dimBlock.x + n, 1);
	mp<<<dimGrid, dimBlock, size>>>(d_a,d_b);
	GET_TIME(end_time);

	cudaMemcpy(brr, d_b, size, cudaMemcpyDeviceToHost);

	num = 0;
	for(i = 0; i < MAX_SIZE / block_size + n; i++){
		if(num < brr[i]){
			num = brr[i];
		}
	}

	printf("THE MAXIMUM NUMBER IS %d\n",num);
	printf("VER D: Elapsed time is %e (msec)\n",end_time - start_time);

	cudaFree(d_a);
	cudaFree(d_b);

	return 0;
}
