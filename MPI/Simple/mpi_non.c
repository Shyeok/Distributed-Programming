#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	int rank, num;
	int n, m, i, temp;
	MPI_Request reqs[30];
	MPI_Status stat[30];
	double start_time, end_time;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &num);
	start_time = MPI_Wtime();
	srand((unsigned)time(NULL) + rank * num);
	n = rand() % (num * 5);
	i = 1;
	m = 0;
//	printf("Process %d's value is %d",rank,n);
	while(i < num){
	//	printf(" %d step %d rank %d ",i,rank,n);
		if(rank + i < num){
			MPI_Isend(&n,1,MPI_INT,rank+i,rank,MPI_COMM_WORLD,&reqs[m]);
			MPI_Wait(&reqs[m],&stat[m]);
		}
		if(rank - i >= 0){
			MPI_Recv(&temp,1,MPI_INT,rank-i,rank-i,MPI_COMM_WORLD,&stat[m]);
			n = n + temp;
		}
		i = i * 2;
		m = m + 1;
	}
//	printf("Process %d's final value is %d \n",rank,n);
	MPI_Finalize();
	if(rank == 0)
	{
		end_time = MPI_Wtime();
		printf("Elapsed time is %f seconds\n",end_time-start_time);
	}
	return 0;
}
