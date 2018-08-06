all:
	gcc -g -Wall -o matrix matrix_gen.c
	gcc -g -Wall -fopenmp -o mp_c mp_c.c
	gcc -g -Wall -fopenmp -o mp_omp mp_omp.c
	nvcc -o mp_cu mp_cu.cu
clean:
	rm matrix
	rm mp_c
	rm mp_omp
	rm mp_cu
