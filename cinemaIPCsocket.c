#include "headers/cinema.h"
#include "headers/commonSocket.h"
#include <stdio.h>
#include <string.h>


int addMovieCinema(char * movie_name){
	struct com command;
	command.op=ADD_MOVIE;
	strcpy(command.info,movie_name);
	int fd=sendCommand(&command);
	struct ret ret;
	receiveAnswer(&ret,fd);
	return ret.extra;
}

int deleteMovieCinema (char * movie_name){
	struct com command;
	command.op=DEL_MOVIE;
	strcpy(command.info,movie_name);
	int fd=sendCommand(&command);
	struct ret ret;
	receiveAnswer(&ret,fd);
	return ret.extra;
}

