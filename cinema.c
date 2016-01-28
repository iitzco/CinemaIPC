#include "headers/cinema.h"
#include "headers/database.h"

int addMovieCinema(char * movie_name){
	return addMovie(movie_name);
}

int deleteMovieCinema(char * movie_name){
	return deleteMovie(movie_name);
}

int getMovieQuantityCinema(){
	return getMovieQuantity();
}