#include "headers/commonSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define PORT_SERV 10000

int sendCommand (struct com * command){

	int socketfd;
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0))==-1){	
		// printf("No puede crear el socket\n");
		return -1;
	}
	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(PORT_SERV);
	serv.sin_addr.s_addr=INADDR_ANY;

	
	while(connect(socketfd,(struct sockaddr *) &serv,sizeof(serv))==-1){
		usleep(100000);
	}

	if(writen(socketfd,command,sizeof(struct com))!=sizeof(struct com)){
		// printf("No se puedo mandar el comando\n");
		return -1;
	}
	return socketfd;

}

int receiveCommand (struct com * command, int fd){

	if (readn(fd,command,sizeof(struct com))!=sizeof(struct com)){
		// printf("No puede leer el comando\n");
		return -1;
	}

	return 0;
}

int sendAnswer (struct ret * ret, int fd){

	if (writen(fd,ret,sizeof(struct ret))!=sizeof(struct ret)){
		// printf("No puede escribir la respuesta\n");
		return -1;
	}
	return 0;
}

int receiveAnswer (struct ret * ret, int fd){
	if (readn(fd,ret,sizeof(struct ret))!=sizeof(struct ret)){
		// printf("No puede leer la respuesta\n");
		return -1;
	}
	return 0;
}

int receiveRequest (int fdSocketServer){

	int fd;
	int len;
	struct sockaddr_in cli;
	printf("Waiting client...\n");
	if((fd=accept(fdSocketServer,NULL,NULL))==-1){
		//printf("No se puede hacer accept\n");
		return -1;
	}
	return fd;

}

int initializeServerSocket(){
	int fdSocketServer;
	if((fdSocketServer = socket(AF_INET, SOCK_STREAM, 0))==-1){	
		return -1;
	}
	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(PORT_SERV);
	serv.sin_addr.s_addr=INADDR_ANY;

	if (bind(fdSocketServer,(struct sockaddr *) &serv,sizeof(serv))==-1){
		// printf("Error al hacer bind\n");
		return -1;
	}

	if (listen(fdSocketServer,10)==-1){
		// printf("No se puedo hacer listen\n");
		return -1;
	}		
	return fdSocketServer;
}



