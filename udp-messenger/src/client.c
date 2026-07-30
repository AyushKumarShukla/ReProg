#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>     
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<arpa/inet.h>
#define MAX_MSG_SIZE 1000
#define THIS_SERVER_PORT 8888
#define THAT_SERVER_PORT 8889
#define BUFFER_SIZE 1024

int sockfd;
struct sockaddr_in this_serv_addr, that_serv_addr;
unsigned int len = sizeof(that_serv_addr);
char buffer[BUFFER_SIZE];

void* listenLoop(void* arg){
	
	while(1){
	int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&that_serv_addr, &len);
	buffer[bytes_received] = '\0';
	puts(buffer);
	}

	return NULL;
}

void* sendLoop(void* arg){
	char msg[MAX_MSG_SIZE];
	while(1){
		printf(">>\t");
		fgets(msg,sizeof(msg),stdin);
		sendto(sockfd, msg, strlen(msg), 0,(struct sockaddr*) &that_serv_addr, sizeof(struct sockaddr_in));
		if(strcmp(msg,"quit") == 0)
			break;
	}
	return NULL;
}

int main(int argc, char* argv[]){
	pthread_t thread_send, thread_receive;
	
	//create socket 
	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Error: Socket creation failed\n");
		return -1;
	}

	//set receiver address info
	memset(&this_serv_addr, 0, sizeof(this_serv_addr));

	this_serv_addr.sin_family = AF_INET;
	this_serv_addr.sin_port = htons(THIS_SERVER_PORT);
	this_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*
	that_serv_addr.sin_family = AF_INET;
	that_serv_addr.sin_port = htons(THAT_SERVER_PORT);
	that_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	*/
	// servers need to call bind if they are not going to call sendto or send in the first place
	// Without binding, the server does not have an associated port
	// If we have two programs that 'listen' like a 'server' both the programs must 'bind'
	// themselves to a 'port' so that they have a permanent address
	// Since 'clients' only 'send' data, they get a port associated ephemerally by the OS
	// so 'clients' may not 'bind' to a 'port' explicitly
	// But for 'servers' this ephemeral port selection is not possble as they only 'listen'
	
	if (bind(sockfd, (struct sockaddr *)&this_serv_addr, sizeof(this_serv_addr)) < 0) {
	    perror("Bind failed! Port might already be in use.");
	    return -1;
	}
	printf("\nlog: listening on port: %d\n",THIS_SERVER_PORT);
	if(pthread_create(&thread_send,NULL,sendLoop,NULL) != 0){
		perror("failed to create thread_send");
		return -1;
	}
	
	if(pthread_create(&thread_receive,NULL,listenLoop,NULL) != 0){
		perror("failed to create thread_receive");
		return -1;
	}

	if(pthread_join(thread_send,NULL) != 0 ){
		perror("failed to join worker thread_send with main thread");
		return -1;
	}		
	
	if(pthread_join(thread_receive,NULL) != 0 ){
		perror("failed to join worker thread_receive with main thread");
		return -1;
	}		

	return 0;
}
