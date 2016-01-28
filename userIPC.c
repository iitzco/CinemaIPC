#include "headers/user.h"
#include "headers/common.h"
#include <string.h>
#include <stdio.h>

int checkAvailabilityUser (char * movie_name, char * buffer){
	struct com command;
	command.op=AVAIL;
	strcpy(command.info,movie_name);
	sendCommand(&command,getpid());
	struct ret ret;
	receiveAnswer(&ret,getpid());
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
	sendCommand(&command,getpid());
	struct ret ret;
	receiveAnswer(&ret,getpid());
	return ret.extra;
}

int cancelTicketsUser (char * movie_name, int id){
	struct com command;
	command.op=CANCEL_TICK;
	strcpy(command.info,movie_name);
	command.extra=id;
	sendCommand(&command,getpid());
	struct ret ret;
	receiveAnswer(&ret,getpid());
	return ret.extra;
}