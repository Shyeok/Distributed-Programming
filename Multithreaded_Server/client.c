#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#define _RANDM
#define MAX_SIZE 1000

int access_num;
int port_id;
int num = 0, b_num = 0;
int file_n = 0;
char file_names[10][MAX_SIZE];

pthread_mutex_t b_mutex;

void *access_server()
{
	int clientSocket;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char buffer[MAX_SIZE];

	for(int i = 0; i < access_num; i++){

		clientSocket = socket(PF_INET, SOCK_STREAM, 0);
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serverAddr.sin_port = htons(port_id);
		memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
		addr_size = sizeof serverAddr;
		if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0){
			printf("Client Socket Connection Error!\n");
			exit(1);
		}
		num += 1;

		memset(buffer, '\0', MAX_SIZE);

		strcpy(buffer,"GET ");
		strcat(buffer,file_names[i % file_n]);

		//printf("i am a buffer %s\n",buffer);

		/*if(i % 3 == 0)strcpy(buffer,"GET 1.html");
		if(i % 3 == 1)strcpy(buffer,"GET 2.html");
		if(i % 3 == 2)strcpy(buffer,"GET 3.html");*/

		write(clientSocket, buffer, strlen(buffer));

		memset(buffer, '\0', MAX_SIZE);
		read(clientSocket, buffer, MAX_SIZE);
		if(buffer[0] < '0' || buffer[0] > '9'){
			pthread_mutex_lock(&b_mutex);
			b_num += strlen(buffer);
			pthread_mutex_unlock(&b_mutex);
		}
		else{
			printf("%s\n",buffer);
		}
		//printf("send value is %s and return value is %s\n",arr,buffer);
		//printf("buffer value is %s %d socket is close\n",buffer,clientSocket);
#ifdef _RANDM
		usleep(100);
#endif
	}
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
	int thread_num, n;
	clock_t start_t;
	double elapsed_t;
	char input[MAX_SIZE], file_name[MAX_SIZE];
	pthread_t service_t[MAX_SIZE];
	FILE *fp;

	pthread_mutex_init(&b_mutex, NULL);
	memset(input,'\0', MAX_SIZE);

	start_t = clock();

	if(argc != 5){
		printf("Input Correct Param! %d\n",argc);
		return 0;
	}
	thread_num = atoi(argv[1]);
	access_num = atoi(argv[2]);
	port_id = atoi(argv[3]);
	strcpy(input, argv[4]);

	fp = fopen(input,"r");
	while(fgets(file_name, sizeof(file_name), fp) != NULL){
		strcpy(file_names[file_n],file_name);
		file_n += 1;
		memset(file_name,'\0', MAX_SIZE);
	}
	printf("input value is %d %d %d\n",thread_num, access_num, port_id);
	for(int i = 0; i < thread_num; i++){
		n = pthread_create(&service_t[i], NULL, access_server, NULL);
		if(n != 0){
			printf("Fail to Create Thread!\n");
			return 0;
		}
	}
	for(int i = 0; i < thread_num; i++){
		pthread_join(service_t[i], NULL);
	}

	printf("Done total recv byte %d\n",b_num);

	elapsed_t = (double)(clock() - start_t)/CLOCKS_PER_SEC;
	printf("Elapsed time %lf(sec)\n",elapsed_t);

	return 0;
}
