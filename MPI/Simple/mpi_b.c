#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	int rank, num;
	int n, i, temp;
	double start_time, end_time;
	MPI_Status Stat;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &num);
	start_time = MPI_Wtime();
	srand((unsigned)time(NULL) + rank * num);
	n = rand() % (num * 5);
	i = 1;
	//printf("Process %d's value is %d\n",rank,n);
	while(i < num){
	//	printf(" %d step %d rank %d ",i,rank,n);
		if(rank + i < num){
			MPI_Send(&n,1,MPI_INT,rank+i,rank,MPI_COMM_WORLD);
		}
		if(rank - i >= 0){
			MPI_Recv(&temp,1,MPI_INT,rank-i,rank-i,MPI_COMM_WORLD,&Stat);
			n = n + temp;
		}
		i = i * 2;
	}
	//printf("Process %d's final value is %d \n",rank,n);
	MPI_Finalize();
	if(rank == 0)
	{
		end_time = MPI_Wtime();
		printf("Elapsed time is %f seconds\n",end_time-start_time);
	}
	return 0;
}
