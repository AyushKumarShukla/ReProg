#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>     
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define MAX_MSG_SIZE 1000
#define CONNECTION_PORT 8888
int main(int argc, char* argv[]){

	/*
	if(argc < 2){
		printf("Error: Too few arguments\n");
		printf("usage: %s <message>\n",argv[0]);
		return -1;
	}
	if(argc > 2){
		printf("Error: Too many arguments\n");
		printf("usage: %s <message>\n",argv[0]);
		return -1;
	}
	*/
	//create socket 
	int sockfd;
	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Error: Socket creation failed\n");
		return -1;
	}

	//set receiver address info
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(CONNECTION_PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//connect to receiver
	if( connect(sockfd,(struct sockaddr*) &serv_addr, sizeof(struct sockaddr_in)) < 0){
		perror("Error: Failed to connect\n");
		return -1;	
	}
	
	char msg[MAX_MSG_SIZE];
	while(1){
		printf(">>\t");
		fgets(msg,sizeof(msg),stdin);
		send(sockfd, msg, strlen(msg), 0);
		if(strcmp(msg,"quit") == 0)
			break;
	}
	return 0;
}
