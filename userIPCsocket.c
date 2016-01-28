#include "headers/user.h"
#include "headers/commonSocket.h"
#include <string.h>
#include <stdio.h>

int checkAvailabilityUser (char * movie_name, char * buffer){
	struct com command;
	command.op=AVAIL;
	strcpy(command.info,movie_name);
	int fd=sendCommand(&command);
	struct ret ret;
	receiveAnswer(&ret,fd);
	strcpy(buffer,ret.info);
	return ret.extra;

}

int buyTicketsUser (char * movie_name, int seats[], int lenght){
	struct com command;
	command.op=BUY_TICK;
	strcpy(command.info,movie_name);
	int i = 0;
	for (; i < lenght; ++i)
	{
		command.seats[i]=seats[i];
	}
	command.extra=lenght;
	int fd=sendCommand(&command);
	struct ret ret;
	receiveAnswer(&ret,fd);
	return ret.extra;
}

int cancelTicketsUser (char * movie_name, int id){
	struct com command;
	command.op=CANCEL_TICK;
	strcpy(command.info,movie_name);
	command.extra=id;
	int fd=sendCommand(&command);
	struct ret ret;
	receiveAnswer(&ret,fd);
	return ret.extra;
}