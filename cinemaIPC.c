#include "headers/cinema.h"
#include "headers/common.h"
#include <stdio.h>
#include <string.h>


int addMovieCinema(char * movie_name){
	struct com command;
	command.op=ADD_MOVIE;
	strcpy(command.info,movie_name);
	sendCommand(&command,getpid());
	struct ret ret;
	receiveAnswer(&ret,getpid());
	return ret.extra;
}

int deleteMovieCinema (char * movie_name){
	struct com command;
	command.op=DEL_MOVIE;
	strcpy(command.info,movie_name);
	sendCommand(&command,getpid());
	struct ret ret;
	receiveAnswer(&ret,getpid());
	return ret.extra;
}

