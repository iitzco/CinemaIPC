#include "headers/commonSocket.h"
#include "headers/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

char* getString(int com){
	switch(com){
		case ADD_MOVIE:
			return "agregar una pelicula";
			break;
		case DEL_MOVIE:
			return "borrar una pelicula";
			break;
		case AVAIL:
			return "ver disponibilidad";
			break;
		case BUY_TICK:
			return "comprar tickets";
			break;
		case CANCEL_TICK:
			return "cancelar tickets";
			break;
		case SEE_BOARD:
			return "ver cartelera";
			break;
	}
}

void worker(int fd){
	printf("\nWorker Designado con el fd: %d\n",fd);
	struct com command;
	receiveCommand(&command,fd);
	printf("Atendiendo al cliente que busca %s\n", getString(command.op));
	struct ret ret;
	int i=0;
	switch (command.op){
		case ADD_MOVIE:
			printf("\n");
			i= addMovie(command.info);			
			ret.extra=i;
			sendAnswer(&ret,fd);
			break;
		case DEL_MOVIE:
			printf("\n");
			i= deleteMovie(command.info);
			ret.extra=i;
			sendAnswer(&ret,fd);
			break;
		case AVAIL:
			printf("\n");
			i=checkAvailability(command.info,ret.info);
			ret.extra=i;
			sendAnswer(&ret,fd);
			break;
		case BUY_TICK:
			printf("\n");
			i=buyTickets(command.info, command.seats, command.extra);
			ret.extra=i;
			sendAnswer(&ret,fd);
			break;
		case CANCEL_TICK:
			printf("\n");
			i=cancelTickets (command.info,command.extra);
			ret.extra=i;
			sendAnswer(&ret,fd);
			break;
		case SEE_BOARD:
			printf("\n");
			i=getBoard(ret.board);
			ret.extra=i;
			sendAnswer(&ret,fd);
			break;

	}
	close(fd);
}


int main(int argc, char const *argv[])
{
	int done=0;
	int fdServer;
	system("clear");
	fdServer=initializeServerSocket();
	if (fdServer==-1){
		printf("El socket ha sido abierto recientemente.\nEspere unos minutos antes de abrirlo nuevamente.\n\n");
		return -1;
	}
	printf("Server Inicializado.\n");
	while (!done){
		int fd=receiveRequest(fdServer);
		if (fd!=-1){
			int pid;
			switch (pid=fork()){
				case 0:
					close(fdServer);
					worker(fd);
					return 0;
					break;
				default:
					close(fd);
					break;
			}
		}else{
			//printf("No puedo recibir un request\n");
		}
		
	}	

}
