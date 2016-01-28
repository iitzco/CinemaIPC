#include <stdio.h>
#include <stdlib.h>
#include "headers/user.h"
#include "headers/cinema.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define TRUE 1
#define MOVIE_SEATS 50

#define CLEAR_SCREEN system("clear");

void addMovieFront()
{
	char name[50];
   	printf(KBLU"\nType the name of the movie you want to add to the list: "RESET);
   	scanf("%s", name);
   	int c=addMovieCinema((char *)name);
  	if (c==-1){
  		printf(KRED"\nMovie already in the movie list.\n\n"RESET);
  		return;
  	}
  	if (c==-2){
  		printf(KRED"\nError. Try again later.\n\n"RESET);
  		return;
  	}
  	if (c==-3){
  		printf(KRED"\nList of movies if full. Delete one movie in order to add another one.\n\n"RESET);
  		return;
  	}
  	printf(KGRN"\nThe movie %s was added to the movie list with initiali capacity: %d \n\n"RESET, name,MOVIE_SEATS);
  	return;
}


void deleteMovieFront(){
char name[50];
   printf(KBLU"\nEnter the name of the movie you want to delete from the list::  "RESET);
   scanf("%s", name);
   int c=deleteMovieCinema(name);
   if(c==-1){
   	printf(KRED"\nThe movie %s is not on the list.\n\n"RESET,name);
   	return;
   }
   if (c==-2){
  		printf(KRED"\nError. Try again later.\n\n"RESET);
  		return;
  	}
   printf(KGRN"\nThe movie %s was deleted from the movie list.\n\n"RESET, name);
  	return;
}

void checkAvailabilityFront()
{
	char name[50];
   	printf(KBLU"\nEnter the name of the movie: "RESET);
   	scanf("%s", name);

   	char * buf= malloc (MOVIE_SEATS*sizeof(char));
   	char * buf2= malloc ((MOVIE_SEATS)*sizeof(char));
   	if (checkAvailabilityUser(name,buf)!=0){
   		printf(KRED"\nThat movie is not on the movie list.\n\n"RESET);
   		return;
   	}
   	int i = 0;
   	printf("\nAvailable seats: \n");
   	for (; i < MOVIE_SEATS; i++){
   		if (i%5==0)
   			printf("\n");
   		printf(KYEL"%c (%d)		", buf[i]!='0'? 'X':'0',i);

   	}
   	printf("\n"RESET);
   	return;
}


void buyTicketsFront(){
	char name[50];
   	printf(KBLU"\nEnter the name of the movie: "RESET);
   	scanf("%s", name);
   	int n=getint(KBLU"\nEnter how many seats you want to buy: "RESET);
   	int vec[n];
   	int i=0;
   	while (i<n){
   		vec[i++]=getint(KBLU"\nSeat number %d: "RESET,i);
   	}
   	int id=buyTicketsUser(name,vec,n);
	if (id==-1)
		printf(KRED"\nError. Try again later.\n\n"RESET);
	else if (id==-2)
		printf(KRED"\nThat seats are not available.\n\n"RESET);
	else if(id==-3)		
		printf(KRED"\nThat movie is not on the movie list!\n\n"RESET);
	else
		printf(KGRN"\nSuccess.\nPurchase ID: %d \n\n"RESET, id);
	
}

void cancelTicketsFront(){
	char name[50];
   	printf(KBLU"\nEnter the name of the movie: "RESET);
   	scanf("%s", name);
   	int n=getint(KBLU"\nEnter the purchase ID: "RESET);
   	int ret=cancelTicketsUser(name,n);
   	if (ret==-1)
   		printf(KRED"\nThat purchase ID does not exist\n\n"RESET);
   	else if (ret==-2)
   		printf(KRED"\nError. Try again later.\n\n"RESET);
   	else if (ret==-3)
   		printf(KRED"\nThat movie is not on the movie list.\n\n"RESET);
   	else
   		printf(KGRN"\nPurchase canceled.\n\n"RESET);	
}

void printUserMenu()
{
	printf("1 - Buy tickets\n");
	printf("2 - Cancel tickets\n");
	printf("3 - Availability\n");
	printf("4 - Movie List\n");
	printf("Enter 5 to exit\n\n");
}

void printCinemaMenu()
{
	printf("1 - Add movie\n");
	printf("2 - Delete movie\n");
	printf("3 - Availability\n");
	printf("Press 4 to exit\n\n");
}

int getBoardFront(){
	char mat[10][50];
	 CLEAR_SCREEN
	int n=0;
	if ((n=getBoardClient(mat))==-1)
		printf("Error. Try again later.\n");
	printf(KYEL"\nMOVIE LIST\n\n"RESET);
	int i=0;
	for (; i < n; ++i)
	{
		printf(KYEL"-> %s \n\n"RESET,mat[i]);
	}
	printf("\n");
}

int user() {
	CLEAR_SCREEN
	printf(KCYN"Welcome to the Virtual Cinema\n\n"RESET);
	printf("Available options:\n");	
	while(TRUE)
	{
		printUserMenu();
		int c=getint(KBLU"Enter the desired option (number): "RESET);
		switch(c)
		{
			case 1: 
					buyTicketsFront();
					break;
			case 2:
					cancelTicketsFront();
					break;
			case 3:
					checkAvailabilityFront();
					break;
			case 4:
					getBoardFront();
					break;
			case 5:
					return 0;
			default:
					printf(KRED"Invalid command\n"KRED);
					break;
		}

	}

	return 0;
}

int cinema() {	
	CLEAR_SCREEN
	printf(KCYN"Cinema Administration\n\n"RESET);
	printf("Available options:\n");
	while(TRUE)
	{
		printCinemaMenu();
		int c=getint(KBLU"Enter the desired option (number): "RESET);
		switch(c)
		{
			case 1: 
					addMovieFront();
					break;
			case 2:
					deleteMovieFront();
					break;
			case 3:
					getBoardFront();
					break;
			case 4:
					return 0;
					break;
			default:
					printf(KBLU"Invalid command\n"RESET);
					break;
		}

	}
	return 0;
}

int main(int argc, char const *argv[])
{
	CLEAR_SCREEN;
	printf("Wish to enter as:\n");
	printf("1 - Client\n");
	printf("2 - Admin\n");
	printf("Press 3 to exit\n\n");
	
	while(TRUE){
		int c=getint(KBLU"Enter the desired option (number): "RESET);
		switch(c){
			case 1:
					return user();
			case 2:
					return cinema();
			case 3:
					return 0;
			default:
					printf(KRED"Invalid command\n"RESET);
					break;
		}
	}

	

	return 0;
}