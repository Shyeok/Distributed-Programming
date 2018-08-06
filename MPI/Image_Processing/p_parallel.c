#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	char m, n;
	int width, height;
	int l_w, l_h;
	int max;
	unsigned char **p, *p_send, *p_buf, *p_flip;
	unsigned char *p_g, *p_f;
	unsigned char g;

	int rank, num;
	int temp;
	int i, j, k, l;
	int x, y;
	double start_time, end_time;

	FILE *p_fp_1 = fopen("p_g_p.ppm","w");
	FILE *p_fp_2 = fopen("p_f_p.ppm","w");
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	start_time = MPI_Wtime();

	if(rank == 0)
	{
		FILE *p_fp = fopen("picture.ppm","r");
		//FILE *p_fp_1 = fopen("p_g_p.ppm","w");
		//FILE *p_fp_2 = fopen("p_f_p.pom","w");

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
		for(i = 0; i < height; i++){
			p[i] = (unsigned char *)malloc(sizeof(char)*width*3);
		}
		k = (int)(height / num);
		l = 0;
		p_send = (unsigned char *)malloc(sizeof(char)*k*width*num*3);
		for(i = 0; i < height; i++){
			for(j = 0; j < width; j++){
				fscanf(p_fp,"%c%c%c",&p[i][j*3],&p[i][j*3+1],&p[i][j*3+2]);
				//fprintf(p_fp_1,"%c%c%c",p[i][j*3],p[i][j*3+1],p[i][j*3+2]);
				if(l < k*width*num*3){
					p_send[l] = p[i][j*3];
					p_send[l + 1] = p[i][j*3+1];
					p_send[l + 2] = p[i][j*3+2];
					l = l + 3;
				}
			}
		}
		fclose(p_fp);
	}
	MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

	p_buf = (unsigned char *)malloc(sizeof(unsigned char)*k*width*3);
	p_flip = (unsigned char *)malloc(sizeof(unsigned char)*k*width*3);

	MPI_Scatter(p_send, k*width*3, MPI_CHAR, p_buf, k*width*3, MPI_CHAR, 0, MPI_COMM_WORLD);

	for(i = 0; i < k; i++){
		for(j = 0; j < width; j++){
			x = (i * width + j) * 3;
			temp = (int)(p_buf[x]) + (int)(p_buf[x+1]) + (int)(p_buf[x+2]);
			temp = (int)(temp / 3);

			y = (i * width + (width - j - 1)) * 3;
			p_flip[y] = p_buf[x];
			p_flip[y + 1] = p_buf[x + 1];
			p_flip[y + 2] = p_buf[x + 2];

			p_buf[x] = (unsigned char)(temp);
			p_buf[x + 1] = (unsigned char)(temp);
			p_buf[x + 2] = (unsigned char)(temp);
		}
	}
	if(rank == 0)
	{
		p_g = (unsigned char*)malloc(sizeof(unsigned char)*width*k*num*3);
		p_f = (unsigned char*)malloc(sizeof(unsigned char)*width*k*num*3);
	}

	MPI_Gather(p_buf, k*width*3, MPI_CHAR, p_g, k*width*3, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Gather(p_flip, k*width*3, MPI_CHAR, p_f, k*width*3, MPI_CHAR, 0, MPI_COMM_WORLD);

	free(p_buf);
	free(p_flip);

	if(rank == 0)
	{
		printf("k value is %d %d %d \n",k,height,width);
		for(i = 0; i < height; i++){
			for(j = 0; j < width; j++){
				if(i < k * num){
					x = (i * width + j) * 3;
					fprintf(p_fp_1,"%c%c%c",p_g[x],p_g[x+1],p_g[x+2]);
					fprintf(p_fp_2,"%c%c%c",p_f[x],p_f[x+1],p_f[x+2]);
				}
				else
				{
					temp = (int)p[i][j*3];
					temp = temp + (int)p[i][j*3 + 1];
					temp = temp + (int)p[i][j*3 + 2];
					temp = temp / 3;
					g = (unsigned char)(temp);
					fprintf(p_fp_1,"%c%c%c",g,g,g);
					y = (width - j - 1) * 3;
					fprintf(p_fp_2,"%c%c%c",p[i][y],p[i][y + 1],p[i][y + 2]);
				}
			}
		}
		/*fclose(p_fp_1);
		fclose(p_fp_2);
		free(p);
		free(p_g);
		free(p_f);*/
		//printf("hello");
	}
	MPI_Finalize();
	end_time = MPI_Wtime();
	if(rank == 0){
		printf("Elapsed time is %f\n",end_time - start_time);
	}
	return 0;
}
