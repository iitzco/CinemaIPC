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
			return "add a movie";
			break;
		case DEL_MOVIE:
			return "delete a movie";
			break;
		case AVAIL:
			return "see availability";
			break;
		case BUY_TICK:
			return "buy tickets";
			break;
		case CANCEL_TICK:
			return "cancel tickets";
			break;
		case SEE_BOARD:
			return "see movie list";
			break;
	}
}

void worker(int fd){
	printf("\nWorker Designado con el fd: %d\n",fd);
	struct com command;
	receiveCommand(&command,fd);
	printf("Handling client who looks for %s\n", getString(command.op));
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
		printf("The socket was recently closed.\nWait some minuts in order to open it again.\n\n");
		return -1;
	}
	printf("Server initialized.\n");
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
