#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int swap(int *n, int *m)
{
	int temp;
	temp = *n;
	*n = *m;
	*m = temp;
	return 0;
}
int quicksort(int num, int rank, int n, int *arr)
{
	int i, l, r;
	int mid, p;
	int *brr, len;
	//printf("n value %d ",n);
	//for(i = 0; i < n; i++)
	{
	//	printf("%d ",arr[i]);
	}
	//printf("\n");
	MPI_Status Stat;
	mid = (n - 1)/2;
	p = arr[mid];
	swap(&arr[0],&arr[mid]);
	l = 1;
	r = n - 1;
	while(1)
	{
		while(arr[l] <= p)l = l + 1;
		while(arr[r] > p)r = r - 1;
		if(l >= r)break;
		swap(&arr[l], &arr[r]);
	}
	swap(&arr[0],&arr[r]);
	if(n >= 2)
	{
		if(n == 2 && arr[0] > arr[1])swap(&arr[0],&arr[1]); 
		else if((rank + 1) * 2 + 1 <= num){
			//printf("hello aaaaaa\n");
			len = n - r - 1;
			MPI_Send(&r,1,MPI_INT,(rank+1)*2-1,rank,MPI_COMM_WORLD);
			MPI_Send(&len,1,MPI_INT,(rank+1)*2,rank,MPI_COMM_WORLD);
			for(i = 0; i < n; i++){
				if(i < r){
					MPI_Send(&arr[i],1,MPI_INT,(rank+1)*2-1,rank,MPI_COMM_WORLD);
				}
				else if(i >= r+1){
					MPI_Send(&arr[i],1,MPI_INT,(rank+1)*2,rank,MPI_COMM_WORLD);
				}
			}
			for(i = 0; i < n; i++){
				if(i < r){
					MPI_Recv(&arr[i],1,MPI_INT,(rank+1)*2-1,rank,MPI_COMM_WORLD,&Stat);
				}
				else if(i >= r+1){
					MPI_Recv(&arr[i],1,MPI_INT,(rank+1)*2,rank,MPI_COMM_WORLD,&Stat);
				}
			}
		}
		else
		{ 
			brr = (int *)malloc(sizeof(int)*n);
			for(i = 0; i < r; i++){
				brr[i] = arr[i];
			}
			if(r > 0)quicksort(num, rank, r, arr);
			/*for(i = 0; i < r; i++){
				arr[i] = brr[i];
			}*/
			len = n - r - 1;
			for(i = r + 1; i < n; i++){
				brr[i - r - 1] = arr[i];
			}
			if(len > 0)quicksort(num, rank, len, brr);
			for(i = r + 1; i < n; i++){
				arr[i] = brr[i - r - 1];
			}
			free(brr);
		}
	}
	return 0;
}
int main(int argc, char **argv)
{
	int rank, num, i;
	int *arr;
	int n, m, l, r, mid;
	int p, len = 2000;
	int p_rank;
	double start_time, end_time;
	MPI_Status Stat;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &num);
	if(rank == 0){
		scanf("%d",&len);
	}
	start_time = MPI_Wtime();
	if(rank == 0){
		srand((unsigned)time(NULL));
		arr = (int*)malloc(sizeof(int)*len);
		for(i = 0; i < len; i++){
			arr[i] = rand() % 100;
		}
	//	printf("First array is \n");
	//	for(i = 0; i < len; i++){
	//		printf("%d ",arr[i]);
	//	}
		n = len;
	//	printf("\n");
	}
	else{
	//	printf("hello rank is %d aaa\n",rank);
		p_rank = (rank+1)/2-1;
		MPI_Recv(&n,1,MPI_INT,p_rank,p_rank,MPI_COMM_WORLD,&Stat);
		arr = (int*)malloc(sizeof(int)*n);
		for(i = 0; i < n; i++){
			MPI_Recv(&arr[i],1,MPI_INT,p_rank,p_rank,MPI_COMM_WORLD,&Stat);
		}
	//	printf("Rank is %d\n",rank);
	}
	if(n >= 2){
		quicksort(num, rank, n, arr);
	}
	else if(n == 2){
		if(arr[0] > arr[1])swap(&arr[0],&arr[1]);
	}
	if(rank != 0)
	{
		p_rank = (rank+1)/2-1;
		for(i = 0; i < n; i++)
		{
			MPI_Send(&arr[i],1,MPI_INT,p_rank,p_rank,MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	if(rank == 0)
	{
		end_time = MPI_Wtime();
		printf("Elapsed time is %f seconds\n",end_time-start_time);
	//	printf("array is\n");
	//	for(i = 0; i < len; i++){
	//		printf("%d ",arr[i]);
	//	}
	//	printf("\n");
	}
	free(arr);
	return 0;
}
