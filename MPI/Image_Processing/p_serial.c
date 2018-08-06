#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	char m, n;
	int width, height;
	int max;
	unsigned char **p, **p_f;
	unsigned char g;

	int temp;
	int i, j;
	double start_time, end_time;
	MPI_Init(&argc,&argv);
	start_time = MPI_Wtime();
	
	FILE *p_fp = fopen("picture.ppm","r");
	FILE *p_fp_1 = fopen("p_g.ppm","w");
	FILE *p_fp_2 = fopen("p_f.ppm","w");

	fscanf(p_fp,"%c%c\n",&m,&n);
	fprintf(p_fp_1,"%c%c\n",m,n);
	fprintf(p_fp_2,"%c%c\n",m,n);
	fscanf(p_fp,"%d%d\n",&width, &height);
	fprintf(p_fp_1,"%d %d\n",width,height);
	fprintf(p_fp_2,"%d %d\n",width,height);
	fscanf(p_fp,"%d\n",&max);
	fprintf(p_fp_1,"%d\n",max);
	fprintf(p_fp_2,"%d\n",max);

	p = (unsigned char **)malloc(sizeof(char*)*height);
	p_f = (unsigned char **)malloc(sizeof(char*)*height);
	for(i = 0; i < height; i++){
		p[i] = (unsigned char *)malloc(sizeof(char)*width*3);
		p_f[i] = (unsigned char *)malloc(sizeof(char)*width*3);
	}

	printf("hello");

	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			fscanf(p_fp,"%c%c%c",&p[i][j*3],&p[i][j*3+1],&p[i][j*3+2]);
			temp = (int)(p[i][j*3]) + (int)(p[i][j*3+1])+(int)(p[i][j*3+2]);
			p_f[i][(width - j - 1)*3] = p[i][j*3];
			p_f[i][(width - j - 1)*3+1] = p[i][j*3+1];
			p_f[i][(width - j - 1)*3+2] = p[i][j*3+2];
			temp = temp / 3;
			g = (unsigned char)(temp);
			fprintf(p_fp_1,"%c%c%c",g,g,g);
		}
	}
	fclose(p_fp);
	fclose(p_fp_1);
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			fprintf(p_fp_2,"%c%c%c",p_f[i][j*3],p_f[i][j*3+1],p_f[i][j*3+2]);
		}
	}
	fclose(p_fp_2);
	free(p);
	free(p_f);
	MPI_Finalize();
	end_time = MPI_Wtime();
	printf("Elapsed time is %f\n",end_time-start_time);
	return 0;
}
