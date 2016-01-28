#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "headers/signalsIPC.h"
#include "headers/common.h"
#include "lib/stdion.h"


int sendCommand (struct com * command, int pid){
	//printf("Enviando comando.\n");
	int spid = getServerPid();
	// printf("Ya se leyo el pid del servidor: %d\n",spid);
	if(addClientPid(pid)==-1)
		return-1;
	if(addComm(command,REQUEST,pid)==-1)
		return-1;
	// printf("Se envia señal %d\n",NEWCLIENT);
	kill(spid,NEWCLIENT);
	//printf("Esperando respuesta del servidor.\n");
	if(waitSignal(NEWCOMM)!=0)
		return -1;
	return 0;
}



int receiveCommand (struct com * command, int pidClient){
	//printf("Leyendo comando.\n");
	if(readComm(command,REQUEST,pidClient)!=0)
		return -1;
	return 0;
}

int sendAnswer (struct ret * response, int pid){
	if(addComm(response,RESPONSE,pid)==-1)
		return-1;
	sleep(1);
	//printf("Enviando respuesta al cliente.\n");
	kill(pid,NEWCOMM);
	return 0;
}

int receiveAnswer (struct ret * response, int pid){
	if(readComm(response,RESPONSE,pid)!=0)
		return -1;
	return 0;
}

int receivePID (){
	//printf("receivePID\n");
	//printf("El tamaño de com es: %d.\nEl tamaño de ret es: %d.\n",REQUEST,RESPONSE);
	if(writeServerPid()==-1)
		return -1;
	return waitForClientPid();
}

//Método que espera hasta que se encuentre un cliente.
int waitForClientPid(){
	//printf("Esperando cliente.\n");
	if(waitSignal(NEWCLIENT)!=0){
		printf("Error en señal.\n");
		return -1;
	}
	// printf("Nuevo cliente encontrado.\n");
	int fd_cl;
	fd_cl = open(CLIENTPIDS,O_RDONLY);
	int pidclient = -1;
	if(readn(fd_cl,&pidclient,sizeof(int))!=sizeof(int)){
		//printf("Error al leer pid del cliente.\n");
		return -1;
	}
	close(fd_cl);
	remove(CLIENTPIDS);
	//printf("Retorno\n");
	return pidclient;
}

//Método que agrega un pid a la lista de pids de clientes.
int addClientPid(int clientpid){
	// printf("addClientPid\n");
	int fd_clientpids = open(CLIENTPIDS,O_WRONLY|O_CREAT,0666);
	if(fd_clientpids == -1 || writen(fd_clientpids,&clientpid,sizeof(int))!=sizeof(int)){
		// printf("Error al escribir el archivo de clientes.\n");
		return -1;
	}
	close(fd_clientpids);
	return clientpid;
}

//Método que escribe el pid del servidor en el archivo.
int writeServerPid(){
	if(doesFileExists(SERVERFILE))
		remove(SERVERFILE);
	// printf("Escribiendo pid del servidor\n");
	int fd_srv = open(SERVERFILE,O_WRONLY|O_CREAT,0666);
	int pid_srv = getpid();
	//printf("El pid de este servidor es: %d.\n",pid_srv);
	if(fd_srv ==-1 || writen(fd_srv,&pid_srv,sizeof(int))!=sizeof(int)){
		// printf("Error al escribir el archivo del servidor.\n");
		return -1;
	}
	close(fd_srv);
	return 0;
}

//Método que devuelve el pid del servidor
int getServerPid(){
	//printf("Esperando al Servidor.\n");
	//Espero a que el servidor escriba el archivo
	while(!doesFileExists(SERVERFILE))
		sleep(1);
	int fd_srv = open(SERVERFILE,O_RDONLY);
	int pid_srv = -1;
	if(fd_srv==-1){
		// printf("Error al abrir el archivo del servidor\n");
		return -1;
	}
	if(readn(fd_srv,&pid_srv,sizeof(int))!=sizeof(int)){
		// printf("Error al leer el archivo del servidor\n");
		return -1;	
	}
	close(fd_srv);
	return pid_srv;
}

//Método que lee un request/response del archivo de comunicacion cliente-servidor
int readComm(void* str,int method,int clientpid){
	// printf("Leyendo el archivo de comunicacion\n");
	char filename[40];
	sprintf(filename,"./signalsIPC/%dcom.txt",clientpid);
	int fd_com = open(filename,O_RDWR);
	if(fd_com==-1 || readn(fd_com,str,method)!=method){
		switch(method){
			case REQUEST:
				// printf("Error al leer request.\n");
				break;
			case RESPONSE:
				// printf("Error al leer response.\n");
				break;
			default:
				// printf("Error en el método al leer.\n");
		}
		return -1;
	}
	close(fd_com);
	remove(filename);
	return 0;
}

//Método que escribe un request/response del archivo de comunicacion cliente-servidor
int addComm(void* str,int method,int clientpid){
	// printf("Escribiendo en el archivo de comunicacion\n");
	char filename[40];
	sprintf(filename,"./signalsIPC/%dcom.txt",clientpid);
	int fd_com = open(filename,O_WRONLY|O_CREAT,0666);
	if(fd_com==-1||writen(fd_com,str,method)!=method){
		switch(method){
			case REQUEST:
				// printf("Error al escribir request.\n");
				break;
			case RESPONSE:
				// printf("Error al escribir response.\n");
				break;
			default:
				// printf("Error en el método al escribir.\n");

		}
		return -1;
	}
	close(fd_com);
	return 0;
}

//Método que hace muchas cosas
void doNothing(int signal){}

//Método que espera hasta que se ejecute una señal
int waitSignal(int asignal){
	// printf("Esperando señal %d\n",asignal);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,asignal);
	signal(asignal,doNothing);
	int sig=0;
	if(sigwait(&set,&sig)!=0){
		// printf("Error en la señal.\n");
		return -1;
	}
	// printf("Recibio señal %d\n",sig);
	return 0;
}

//Método que dice si un archivo existe o no
int doesFileExists(const char* file) {
    return (access(file, 0) == 0);
}
