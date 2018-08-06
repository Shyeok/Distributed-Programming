#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#define MAX_SIZE 5000
#define _DEBUG

typedef struct link{
	int client_socket;
	struct link *p;
}job_q;

job_q *head = NULL;
int client_num = 0;
pthread_rwlock_t rwlock;

char caching[5][MAX_SIZE];
int caching_check[5];

int put_job(int client_socket)
{
	job_q *node, *tmp;
	node = (job_q*)malloc(sizeof(job_q));
	node->client_socket = client_socket;
	node->p = NULL;
	if(head == NULL){
		head = node;
	}
	else{
		tmp = head;
		while(tmp->p != NULL){
			tmp = tmp->p;
		}
		tmp->p = node;
	}
	return 0;
}

int pop_job()
{
	int client_socket = -1;
	job_q *tmp_1, *tmp_2;;
	if(head == NULL){
		client_socket = -1;
	}
	else{
		tmp_1 = head->p;
		client_socket = head->client_socket;
		tmp_2 = head;
		head = tmp_1;
		free(tmp_2);
	}
	return client_socket;
}

void *service_dispatch(void * arg)
{
	char command[MAX_SIZE], file_name[MAX_SIZE];
	char buffer[MAX_SIZE];
	char c;
	struct stat sb;
	int client_socket, i;
	int caching_flag = 0;
	int n;
	//int thread_id;
	FILE *fp;

	//thread_id = *(int *)arg;

	while(1)
	{
		/*Write Opertaion in Queue*/
		pthread_rwlock_wrlock(&rwlock);
		client_socket = pop_job();
		pthread_rwlock_unlock(&rwlock);

		memset(buffer, '\0', MAX_SIZE);
		memset(command, '\0', MAX_SIZE);
		memset(file_name, '\0', MAX_SIZE);

		if(client_socket != -1){
			if(read(client_socket, buffer, MAX_SIZE) > 0){
				if(chdir("/var/tmp/gr120170214") < 0){
					printf("500 Internal Error.");
				}
				sscanf(buffer,"%s %s", command, file_name);
				memset(buffer, '\0', MAX_SIZE);
				if(strcmp(command,"GET") != 0){
					strcpy(buffer,"501 Not Implemented");
				}
				else if(stat(file_name,&sb) < 0){
					strcpy(buffer,"404 Not Found");
					printf("%s\n",file_name);
				}
				else{
					fp = fopen(file_name, "r");
					if(fp == NULL){
						strcpy(buffer,"403 Forbidden");
					}
					else{
						caching_flag = 0;
						n = file_name[0] - '0';
						if(caching_check[n] != 0)caching_flag = 1;
						if(caching_flag == 0){
							i = 0;
							while((c = getc(fp)) != EOF){
								i += sprintf(buffer + i,"%c",c);
							}
							caching_check[n] = 1;
							strcpy(caching[n],buffer);
						}
						if(caching_flag == 1)strcpy(buffer, caching[n]);
						fclose(fp);
					}
				}
				write(client_socket, buffer, strlen(buffer) + 1);
				close(client_socket);
			}
		}
	}
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
	int server_socket, client_socket[MAX_SIZE];
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int one, port_id;
	int num = 0;
	int pool_size = 0;
	pthread_t service_t[MAX_SIZE];

	pthread_rwlock_init(&rwlock, NULL);

	if(argc != 3){
		printf("Input Correct Param!\n");
		return 0;
	}

	port_id = atoi(argv[1]);
	pool_size = atoi(argv[2]);

	for(int i = 0; i < pool_size; i++){
		if(pthread_create(&service_t[i], NULL, service_dispatch, (void *)(&i)) < 0){
			printf("Pthread Create Error!\n");
			return 0;
		}
		pthread_detach(service_t[i]);
	}

	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Error: Unable to open socket in parmon server.\n");
		exit(1);
	}

	memset((char*)&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port_id);
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));

	if(bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("Error: Unable to bind socket in parmon server.\n");
		exit(1);
	}

	listen(server_socket, 10);

	num = 0;
	while(1){
		clilen = sizeof(cli_addr);
		client_socket[num] = accept(server_socket, (struct sockaddr *)&serv_addr, &clilen);

		if(client_socket < 0){
			printf("connection to client failed in server.\n");
			continue;
		}
		/* Write Opertion in Queue*/
		pthread_rwlock_wrlock(&rwlock);
		put_job(client_socket[num]);
		pthread_rwlock_unlock(&rwlock);
	}
	return 0;
}
