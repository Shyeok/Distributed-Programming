all:
	gcc -Wall -o array_gen array_gen.c
	gcc -Wall -o A_VERSION a.c
	nvcc -o B_VERSION b.cu
	nvcc -o C_VERSION c.cu
	nvcc -o D_VERSION d.cu
clean:
	rm array_gen
	rm A_VERSION
	rm B_VERSION
	rm C_VERSION
	rm D_VERSION
