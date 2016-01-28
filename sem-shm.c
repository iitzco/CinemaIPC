#include "headers/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SIZE 1000
#define SEM_SERVER "sem_server"
#define SEM_SERVER_AUX "sem_server_aux"
#define SEM_CLI "sem_client"
#define SHM_SERVER "shm_server"

int createShm(char * path){
	int fd;
	if ((fd = shm_open(path,O_RDWR|O_CREAT, 0666)) == -1 ){
		// printf("Error al inicializar la memoria compartida\n");
		return -1;
	}
	return fd;
}

int existsShm(char * path){
	return (shm_open(path,O_RDWR,0666)!=-1);
}


sem_t * createSem(char * path, int init){
	sem_t * i;
	if ((i=sem_open(path,O_RDWR|O_CREAT,0666,init))==SEM_FAILED){
		// printf("No pudo crear el semaforo\n");
		return NULL;
	}
	return i;
}

int existsSem(char * path){
	return sem_open(path,O_RDWR)!=SEM_FAILED;
}

int up(sem_t * sem){
	if (sem_post(sem)==-1){
		// printf("No se puede hacer up sobre semaforo\n");
		return -1;
	}
	return 0;
}

int down(sem_t * sem){
	if (sem_wait(sem)==-1){
		// printf("No se puede hacer up sobre semaforo\n");
		return -1;
	}
	return 0;
}

int getShm(char * path){
	int fd;
	if (!existsShm(path)){
		fd=createShm(path);
		if (fd==-1){
			return -1;
		}
		ftruncate(fd, SIZE);
	}else{
		fd=shm_open(path,O_RDWR,0666);
	}
	return fd;
}

sem_t * getSem(char * path){
	sem_t * i;
	if (!existsSem(path)){
		i=createSem(path,0);
		if (i==NULL){
			// printf("Error al crear el semaforo del servidor\n");
			return NULL;
		}
	}else{
		i=sem_open(path,O_RDWR);
	}
	return i;
}

int sendCommand (struct com * command,int pid){
	sem_t * j;
	j=getSem(SEM_SERVER);
	//Levanto al servidor si esta dormido
	up(j);

	//El cliente se duerme hasta que el servidor lo levante
	sem_t * i;
	i=getSem(SEM_CLI);
	down(i);

	int fd;
	fd=getShm(SHM_SERVER);
	int * mem;
	if (!(mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))){
		// printf("Error al escribir de la memoria compartida\n");
		return -1;
	}
	//Sobre el otro del servudor
	int n=pid;
	memcpy(mem, &n,sizeof(int));

	sem_t * x;
	x=getSem(SEM_SERVER_AUX);
	up(x);


	close(fd);
	sem_close(i);
	sem_close(j);
	sem_close(x);

	//Semaforo especifico del cliente y se pone a dormir
	char * pathC=malloc(20*sizeof(char));
	sprintf(pathC,"c%d",pid);
	i=getSem(pathC);
	down(i);

	//Levanta al worker
	char * pathW=malloc(20*sizeof(char));
	sprintf(pathW,"w%d",pid);
	j=getSem(pathW);
	up(j);

	char * pathShm=malloc(20*sizeof(char));
	sprintf(pathShm,"m%d",pid);
	fd=getShm(pathShm);
	void * msg;
	if (!(msg = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))){
		// printf("Error al escribir de la memoria compartida\n");
		return -1;
	}

	memcpy(msg, command,sizeof(struct com));
	char * pathSemAux=malloc(20*sizeof(char));
	sprintf(pathSemAux,"a%d",pid);

	x=getSem(pathSemAux);
	up(x);

	close(fd);
	sem_close(i);
	sem_close(j);
	sem_close(x);

	return 0;
}

int receiveCommand (struct com * command, int pidClient){
	sem_t * i;
	sem_t * j;
	sem_t * x;

	//Semaforo especifico del cliente y lo levanta el worker
	char * pathC=malloc(20*sizeof(char));
	sprintf(pathC,"c%d",pidClient);
	i=getSem(pathC);
	up(i);

	//Se duerme hasta que el cliente lo despierta
	char * pathW=malloc(20*sizeof(char));
	sprintf(pathW,"w%d",pidClient);
	j=getSem(pathW);
	down(j);

	//Se levanta cuando ya esta escrito la mem comp
	char * pathSemAux=malloc(20*sizeof(char));
	sprintf(pathSemAux,"a%d",pidClient);
	x=getSem(pathSemAux);
	down(x);

	int fd;
	char * pathShm=malloc(20*sizeof(char));
	sprintf(pathShm,"m%d",pidClient);
	fd=getShm(pathShm);

	void * msg;
	if (!(msg = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))){
		// printf("Error al escribir de la memoria compartida\n");
		return -1;
	}

	memcpy(command, msg,sizeof(struct com));

	close(fd);
	sem_close(i);
	sem_close(j);
	sem_close(x);
	return 0;
}

int sendAnswer (struct ret * ret, int pid){

	int fd;
	char * pathShm=malloc(20*sizeof(char));
	sprintf(pathShm,"m%d",pid);
	fd=getShm(pathShm);

	void * msg;
	if (!(msg = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))){
		// printf("Error al escribir de la memoria compartida\n");
		return -1;
	}

	memcpy(msg, ret ,sizeof(struct ret));

	//Levantar la respuesta
	sem_t * i;
	char * pathC=malloc(20*sizeof(char));
	sprintf(pathC,"c%d",pid);
	i=getSem(pathC);

	up(i);
	close(fd);
	sem_close(i);
	return 0;
}

int receiveAnswer (struct ret * ret, int pid){

	//Se levanta cuando esta la respuesta disponible
	sem_t * i;
	char * pathC=malloc(20*sizeof(char));
	sprintf(pathC,"c%d",pid);
	i=getSem(pathC);
	down(i);

	int fd;
	char * pathShm=malloc(20*sizeof(char));
	sprintf(pathShm,"m%d",pid);
	fd=getShm(pathShm);

	void * msg;
	if (!(msg = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))){
		// printf("Error al escribir de la memoria compartida\n");
		return -1;
	}

	memcpy(ret, msg ,sizeof(struct ret));
	close(fd);
	sem_close(i);
	return 0;
}

int receivePID (){

	sem_t * j;
	j=getSem(SEM_SERVER);
	//El servidor se duerme hasta que alguien lo levanta
	down(j);


	//Levanta al cliente que este dormido
	sem_t * i;
	i=getSem(SEM_CLI);
	up(i);

	//Para que lo levante el cliente una vez que escribio algo
	sem_t * x;
	x=getSem(SEM_SERVER_AUX);
	down(x);

	int fd=0;	
	fd=getShm(SHM_SERVER);
	if (fd==-1){
		// printf("No se puede abrir la memoria compartida\n");
	}
	int *mem=NULL;
	if (!(mem = mmap(NULL, SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0))){
		// printf("Error al leer de la memoria compartida\n");
		return -1;
	}
	close(fd);
	sem_close(i);
	sem_close(j);
	sem_close(x);
	int n=*mem;
	return n;
}