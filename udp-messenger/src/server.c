#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int sockfd;
struct sockaddr_in serv_addr, client_addr;
unsigned int len = sizeof(client_addr);
char buffer[BUFFER_SIZE];

void* listenLoop(void* arg){
	
	while(1){
	int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &len);
	buffer[bytes_received] = '\0';
	puts(buffer);
	}

	return NULL;
}


int main(int argc, char* argv[]){
	pthread_t thread1;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		perror("error: failed to create socket");
		return -1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	
	int bind_status = bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(bind_status < 0){
		perror("error: failed to bind name to socket\n");
		return -1;
	}

	if(pthread_create(&thread1,NULL,listenLoop,NULL) != 0){
		perror("failed to create thread1");
		return -1;
	}

	if(pthread_join(thread1,NULL) != 0 ){
		perror("failed to join worker thread with main thread");
		return -1;
	}		
	return 0;
}
