#include "headers/common.h"
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

void worker(int pid){
	printf("\nWorker Designado para el cliente: %d\n",pid);
	int done=0;
	struct com command;
	receiveCommand(&command,pid);
	printf("Atendiendo cliente %d que busca %s\n",pid,getString(command.op));
	struct ret ret;
	int i=0;
	switch (command.op){
		case ADD_MOVIE:
			printf("\n");
			i= addMovie(command.info);			
			ret.extra=i;
			sendAnswer(&ret,pid);
			break;
		case DEL_MOVIE:
			printf("\n");
			i= deleteMovie(command.info);
			ret.extra=i;
			sendAnswer(&ret,pid);
			break;
		case AVAIL:
			printf("\n");
			i=checkAvailability(command.info,ret.info);
			ret.extra=i;
			sendAnswer(&ret,pid);
			break;
		case BUY_TICK:
			printf("\n");
			i=buyTickets(command.info, command.seats, command.extra);
			ret.extra=i;
			sendAnswer(&ret,pid);
			break;
		case CANCEL_TICK:
			printf("\n");
			i=cancelTickets (command.info,command.extra);
			ret.extra=i;
			sendAnswer(&ret,pid);
			break;
		case SEE_BOARD:
			printf("\n");
			i=getBoard(ret.board);
			ret.extra=i;
			sendAnswer(&ret,pid);
			break;

	}
}


int main(int argc, char const *argv[])
{
	int done=0;
	system("clear");
	printf("Server Inicializado.\n");
	while (!done){
		printf("Esperando cliente...\n");
		int pidClient=receivePID();
		if (pidClient!=-1){
			int pid;
			switch (pid=fork()){
				case 0:
					worker(pidClient);
					return 0;
					break;
			}
		
			
		}
	}	

}
