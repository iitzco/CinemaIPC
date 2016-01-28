#include "headers/user.h"
#include "headers/database.h"

int checkAvailabilityUser (char * movie_name, char * buffer){
	return checkAvailability(movie_name,buffer);
}

int buyTicketsUser (char * movie_name, int seats[], int lenght){
	return buyTickets(movie_name,seats,lenght);
}

int cancelTicketsUser (char * movie_name, int id){
	return cancelTickets(movie_name,id);
}

