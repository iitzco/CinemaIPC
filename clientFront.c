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
   	printf(KBLU"\nIngresar nombre de la pelicula que se desea agregar: "RESET);
   	scanf("%s", name);
   	int c=addMovieCinema((char *)name);
  	if (c==-1){
  		printf(KRED"\nLa pelicula ya se encuentra en cartelera.\n\n"RESET);
  		return;
  	}
  	if (c==-2){
  		printf(KRED"\nError inesperado. Intente nuevamente mas tarde.\n\n"RESET);
  		return;
  	}
  	if (c==-3){
  		printf(KRED"\nLa cartelera esta completa. Elimina una pelicula para poder agregas otra.\n\n"RESET);
  		return;
  	}
  	printf(KGRN"\nSe agrego la pelicula %s a la cartelera con capacidad inicial: %d \n\n"RESET, name,MOVIE_SEATS);
  	return;
}


void deleteMovieFront(){
char name[50];
   printf(KBLU"\nIngresar nombre de la pelicula que se desea eliminar:  "RESET);
   scanf("%s", name);
   int c=deleteMovieCinema(name);
   if(c==-1){
   	printf(KRED"\nLa pelicula %s no se encuentra en cartelera.\n\n"RESET,name);
   	return;
   }
   if (c==-2){
  		printf(KRED"\nError inesperado. Intente nuevamente mas tarde.\n\n"RESET);
  		return;
  	}
   printf(KGRN"\nSe ha eliminado la pelicula %s de la cartelera\n\n"RESET, name);
  	return;
}

void checkAvailabilityFront()
{
	char name[50];
   	printf(KBLU"\nIngresar nombre de la pelicula: "RESET);
   	scanf("%s", name);

   	char * buf= malloc (MOVIE_SEATS*sizeof(char));
   	char * buf2= malloc ((MOVIE_SEATS)*sizeof(char));
   	if (checkAvailabilityUser(name,buf)!=0){
   		printf(KRED"\nNo se encuentra esa pelicula en cartelera.\n\n"RESET);
   		return;
   	}
   	int i = 0;
   	printf("\nAsientos disponibles: \n");
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
   	printf(KBLU"\nIngresar nombre de la pelicula: "RESET);
   	scanf("%s", name);
   	int n=getint(KBLU"\nIngrese la cantidad de asientos a comprar: "RESET);
   	int vec[n];
   	int i=0;
   	while (i<n){
   		vec[i++]=getint(KBLU"\nAsiento nro %d: "RESET,i);
   	}
   	int id=buyTicketsUser(name,vec,n);
	if (id==-1)
		printf(KRED"\nError inesperado. Intente nuevamente mas tarde.\n\n"RESET);
	else if (id==-2)
		printf(KRED"\nEsos asientos no estan disponibles.\n\n"RESET);
	else if(id==-3)		
		printf(KRED"\nLa pelicula no existe!\n\n"RESET);
	else
		printf(KGRN"\nCompra exitosa.\nID de la compra: %d \n\n"RESET, id);
	
}

void cancelTicketsFront(){
	char name[50];
   	printf(KBLU"\nIngresar nombre de la pelicula: "RESET);
   	scanf("%s", name);
   	int n=getint(KBLU"\nIngrese el ID de la compra: "RESET);
   	int ret=cancelTicketsUser(name,n);
   	if (ret==-1)
   		printf(KRED"\nNo existe compra con ese ID\n\n"RESET);
   	else if (ret==-2)
   		printf(KRED"\nError inesperado. Intente nuevamente mas tarde.\n\n"RESET);
   	else if (ret==-3)
   		printf(KRED"\nNo existe esa pelicula.\n\n"RESET);
   	else
   		printf(KGRN"\nTickets cancelados.\n\n"RESET);	
}

void printUserMenu()
{
	printf("1 - Comprar tickets\n");
	printf("2 - Cancelar tickets\n");
	printf("3 - Disponibilidad\n");
	printf("4 - Cartelera\n");
	printf("Presione 5 para salir\n\n");
}

void printCinemaMenu()
{
	printf("1 - Agregar pelicula\n");
	printf("2 - Eliminar pelicula\n");
	printf("3 - Cartelera\n");
	printf("Presione 4 para salir\n\n");
}

int getBoardFront(){
	char mat[10][50];
	 CLEAR_SCREEN
	int n=0;
	if ((n=getBoardClient(mat))==-1)
		printf("Error inesperado. Intente nuevamente mas tarde.\n");
	printf(KYEL"\nCARTELERA\n\n"RESET);
	int i=0;
	for (; i < n; ++i)
	{
		printf(KYEL"-> %s \n\n"RESET,mat[i]);
	}
	printf("\n");
}

int user() {
	CLEAR_SCREEN
	printf(KCYN"Bienvenido a su cine virtual\n\n"RESET);
	printf("Aqui se encuentra el menu de opciones disponibles:\n");	
	while(TRUE)
	{
		printUserMenu();
		int c=getint(KBLU"Ingrese la opcion deseada: "RESET);
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
					printf(KRED"Comando invalido\n"KRED);
					break;
		}

	}

	return 0;
}

int cinema() {	
	CLEAR_SCREEN
	printf(KCYN"Administracion de cartelera\n\n"RESET);
	printf("Aqui se encuentra el menu de opciones disponibles:\n");
	while(TRUE)
	{
		printCinemaMenu();
		int c=getint(KBLU"Ingrese la opcion deseada: "RESET);
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
					printf(KBLU"Comando invalido\n"RESET);
					break;
		}

	}
	return 0;
}

int main(int argc, char const *argv[])
{
	CLEAR_SCREEN;
	printf("Desea ingresar como:\n");
	printf("1 - Cliente\n");
	printf("2 - Administrador\n");
	printf("Presione 3 para salir\n\n");
	
	while(TRUE){
		int c=getint(KBLU"Ingrese el numero deseado: "RESET);
		switch(c){
			case 1:
					return user();
			case 2:
					return cinema();
			case 3:
					return 0;
			default:
					printf(KRED"Comando invalido\n"RESET);
					break;
		}
	}

	

	return 0;
}