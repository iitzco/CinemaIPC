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

void worker(int pid){
	printf("\nWorker created to deal with client: %d\n",pid);
	int done=0;
	struct com command;
	receiveCommand(&command,pid);
	printf("Handling client %d who is looking forward to %s\n",pid,getString(command.op));
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
	printf("Server initialized.\n");
	while (!done){
		printf("Waiting client...\n");
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
