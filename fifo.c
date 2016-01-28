#include "headers/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <fcntl.h>

int createFIFO(char * path){
	int i;
	i=mkfifo(path,0666);
	return 0;
}

char * makePath(int pid, int isCommand){
	char * path=malloc(20);
	if (isCommand)
		sprintf(path,"/tmp/%dc",pid);
	else
		sprintf(path,"/tmp/%dr",pid);
	return path;
}


int sendCommand (struct com * command,int pid){
	int fd;
	char * path=makePath(pid,1);
	//Escribo el pid en el fifo principal

	if ((fd=open("/tmp/fifo",O_WRONLY))==-1){
		createFIFO("/tmp/fifo");
		fd=open("/tmp/fifo",O_WRONLY);
	}
	if (writen(fd,&pid,sizeof(int))!=sizeof(int)){
		printf("No puede escribir el pid\n");
		return -1;
	}

	//Escribo en el fifo del cliente

	if ((fd=open(path,O_WRONLY))==-1){
		createFIFO(path);
		fd=open(path,O_WRONLY);
	}
	
	if (writen(fd,command,sizeof(struct com))!=sizeof(struct com)){
		printf("No puede mandar el comando\n");
		return -1;
	}
	return 0;
}

int receiveCommand (struct com * command, int pidClient){
	int fd;
	char * path=makePath(pidClient,1);
	//Abro el fifo del cliente para lectura.

	if ((fd=open(path,O_RDONLY))==-1){
		createFIFO(path);
		fd=open(path,O_RDONLY);
	}
	
	if (readn(fd,command,sizeof(struct com))!=sizeof(struct com)){
		printf("No llega a leer\n");
	}
	return 0;
}


int sendAnswer (struct ret * ret, int pid){
	int fd;
	char * path=makePath(pid,0);
	//Abro el fifo del cliente para escribir
	if ((fd=open(path,O_WRONLY))==-1){
		createFIFO(path);
		fd=open(path,O_WRONLY);
	}
	
	if (writen(fd,ret,sizeof(struct ret))!=sizeof(struct ret)){
		printf("No puede mandar la respuesta\n");
		return -1;
	}
	return 0;
}

int receiveAnswer (struct ret * ret, int pid){

	char * path=makePath(pid,0);
	int fd;
	if ((fd=open(path,O_RDWR))==-1){
		createFIFO(path);
		fd=open(path,O_RDONLY);
	}	
	if (readn(fd,ret,sizeof(struct ret))!=sizeof(struct ret)){
		printf("No llega a leer\n");
		return -1;
	}
	return 0;
}

int receivePID (){
	int fd;
	createFIFO("/tmp/fifo");
	if ((fd=open("/tmp/fifo",O_RDONLY))==-1){
		printf("No se puede abrir\n");
		return -1;
	}
	int pid;
	if (readn(fd,&pid,sizeof(int))!=sizeof(int)){
		printf("No puede leer del principal el pid\n");
		return -1;
	}
	return pid;
}


