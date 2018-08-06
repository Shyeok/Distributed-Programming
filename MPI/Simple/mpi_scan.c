#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	int rank, num;
	int n, m;
	double start_time, end_time;
	const int root = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num);

	start_time = MPI_Wtime();
	srand((unsigned)time(NULL) + rank * num);
	n = rand() % (num * 5);

	//printf("Process %d's value is %d\n",rank,n);

	MPI_Scan(&n, &m, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	//printf("Process %d's final value is %d\n",rank,m);

	MPI_Finalize();

	if(rank == 0){
		end_time = MPI_Wtime();
		printf("Elapsed time is %f seconds\n",end_time-start_time);
	}
	return 0;
}
