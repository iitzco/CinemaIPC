#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include "headers/database.h"

#define CYCLE int x=0;while(x++<2){}


struct flock initializeFlock(short type){
	struct flock lock;
	lock.l_type=type;
	lock.l_whence= SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	return lock;
}

//return 1 si esta lockeado para ese tipo o 0 si no esta lockeado.
int isLocked(int fd, short type){
	struct flock lock=initializeFlock(type);
	fcntl(fd,F_GETLK,&lock);
	return lock.l_type!=F_UNLCK;
}

//Retorn -1 si fallo, 0 si ok.
int lock(int fd, short type){
	struct flock lock=initializeFlock(type);
	if (fcntl(fd,F_SETLKW,&lock)==-1)
		return -1;
	return 0;
}

//Retorn -1 si fallo, 0 si ok.
int unlock(int fd){
	struct flock lock=initializeFlock(F_UNLCK);
	if (fcntl(fd,F_SETLKW,&lock)==-1)
		return -1;
	return 0;
}


char * getRelativePath(char * movie_name)
{
	char * relative_path=malloc(MOVIE_SIZE*sizeof(char));
	strcpy(relative_path,"DB/");
	strcat(relative_path,movie_name);
	return relative_path;
}

int movieExists (char * movie_name)
{
	char * relative_path = getRelativePath(movie_name);
	int fd;
	if ((fd=open(relative_path,O_RDWR))!=-1){
		return fd;
	}
	return FALSE;
}



char * buildCapacityString()
{
	char * buf=malloc(MOVIE_SEATS*sizeof(char));
	int i = 0;
	for (; i < MOVIE_SEATS; i++)
	{
		buf[i]='0';
	}
	return buf;
}

int addMovie (char * movie_name)
{
	int n=0;
	int fd1=open("DB/.lock",O_RDWR);
	if (fd1==-1){
		//printf("Creando\n");
		fd1=open("DB/.lock",O_RDWR|O_CREAT);
		lock(fd1,F_WRLCK);
		if (write(fd1,&n,sizeof(int))==-1){
			// printf("No pudo escribir el cero\n");
			return -2;
		}
	}
	if (read(fd1,&n,sizeof(int))==-1){
		// printf("No puede leer de .lock\n");
		return -2;
	}
	if (n==10){
		return -3;
	}
	n++;
	unlock(fd1);
	close(fd1);
	fd1=open("DB/.lock",O_RDWR);
	if (fd1==-1){
		// printf("No puede abrir .lock\n");
		return -2;
	}

	if (write(fd1,&n,sizeof(int))==-1){
		// printf("No pudo escribir el n en .lock\n");
		return -2;
	}


	int fd;
	//CYCLE
	if (fd=movieExists(movie_name)){
		if (close(fd)==-1){
			unlock(fd1);
			return -2;
		}
		unlock(fd1);
		return -1;
	}
	char * relative_path = getRelativePath(movie_name);
	fd = creat(relative_path,S_IRWXU);
	if (write(fd,buildCapacityString(),MOVIE_SEATS)==-1){
		unlock(fd1);
		return -2;
	}
	unlock(fd1);
	close(fd1);
	close(fd);
	return 0;
}


int deleteMovie (char * movie_name)
{
	int fd1=open("DB/.lock",O_RDWR|O_CREAT);
	if (fd1==-1){
		return -2;
	}
	lock(fd1,F_WRLCK);
	int fd=movieExists(movie_name);
	if(!fd){
		unlock(fd1);
		return -1;
	}
	lock(fd,F_WRLCK);	
	//printf("Borrando Pelicula...\n");
	//CYCLE;
	//printf("Borrada!\n");
	char * relative_path = getRelativePath(movie_name);
	if (unlink(relative_path)==-1){
		unlock(fd);
		return -2;
	}
	int n=0;
	if (read(fd1,&n,sizeof(int))==-1){
		// printf("No puede leer el .lock para borrar\n");
		return -1;
	}
	//printf("cantidad de pelis %d\n",n);
	n--;
	lseek(fd1,SEEK_SET,0);
	if (write(fd1,&n,sizeof(int))==-1){
		// printf("No puede escribir el .lock para borrar\n");
		return -1;
	}
	unlock(fd);
	unlock(fd1);
	close(fd);			
	return 0;
}

int checkAvailability (char * movie_name, char * buffer){
	int fd;
	fd=movieExists(movie_name);
	if (!fd){
		return -1;
	}
	lock(fd,F_RDLCK);
	if (fillBufferWithSeats(fd,movie_name,buffer)==-2){
		unlock(fd);
		close(fd);
		return -2;
	}
	//printf("Adquiriendo disponibilidad...\n");
	//CYCLE;
	unlock(fd);
	close(fd);
	return 0;
}


int fillBufferWithSeats (int fd, char * movie_name, char * buffer)
{
	if (read(fd,buffer,MOVIE_SEATS)==-1){		
		return -2;
	}
	return 0;
}


int buyTickets (char * movie_name, int seats[], int lenght)
{
	int fd = movieExists(movie_name);
	int id='0';
	if (!fd){
		return -3;
	}
	lock(fd,F_WRLCK);
	char * buf= malloc(MOVIE_SEATS*sizeof(char));
	if (fillBufferWithSeats(fd,movie_name,buf)!=0)
		return -1;
	int i=0;
	for (; i < MOVIE_SEATS; ++i)
	{
		if (buf[i]>id){
			id=buf[i];
		}
	}
	id++;
	i=0;
	for (; i < lenght; i++)
	{
		if (buf[seats[i]]!='0'){
			unlock(fd);
			return -2;
		}
	}
	i=0;
	for (; i < lenght; i++)
	{
		buf[seats[i]]=id;
	}
	lseek(fd,SEEK_SET,0);
	// unlock(fd);
	// close(fd);
	// fd = movieExists(movie_name);
	// if (!fd){
	// 	return -3;
	// }
	// lock(fd,F_WRLCK);
	//printf("Realizando compra...\n");
	//CYCLE;
	//printf("Compra exitosa\n");	
	if (write(fd,buf,MOVIE_SEATS)==-1){
		unlock(fd);
		close(fd);
		return -1;
	}
	unlock(fd);
	close(fd);
	return id;
}

/*
Recibe el nombre de la pelicula y un entero que representa el id de la compra. Retorna:
0: OK
-1: si no existe esa compra para esa pelicula
-2: si hubo algun error
Reliza un lock de escritura sobre parte del archivo de la pelicula.
*/
int cancelTickets (char * movie_name, int id){
	int fd = movieExists(movie_name);
	if (!fd){
		return -3;
	}
	lock(fd,F_WRLCK);
	char * buf= malloc(MOVIE_SEATS*sizeof(char));
	if (fillBufferWithSeats(fd,movie_name,buf)!=0){
		unlock(fd);
		return -2;
	}
	int flag=0;
	int i = 0;
	for (; i < MOVIE_SEATS; ++i)
	{
		if (buf[i]==id){
			buf[i]='0';
			flag=1;
		}
	}
	unlock(fd);
	close(fd);
	fd = movieExists(movie_name);
	lock(fd,F_WRLCK);
	//CYCLE;
	if (write(fd,buf,MOVIE_SEATS)==-1){
		unlock(fd);
		close(fd);
		return -2;
	}
	unlock(fd);
	close(fd);
	if (!flag)
		return -1;
	return 0;
}



int getBoard (char buf[][50]){
	DIR * dir= opendir("DB");
	int fd=open("DB/.lock",O_RDWR|O_CREAT);
	if (fd==-1){
		return -1;
	}
	//printf("Obteniendo Cartelera...\n");
	//CYCLE;
	if (dir==NULL)
		return -1;
	struct dirent * dirEntry;
	int i=0;
	while ((dirEntry=readdir(dir))!=NULL){
		if (strcmp(dirEntry->d_name,"..") != 0 && strcmp(dirEntry->d_name,".")!=0 && strcmp(dirEntry->d_name,".lock")!=0){
			strcpy(buf[i++],dirEntry->d_name);
		}
	}
	unlock(fd); 
	close(fd);
	if (closedir(dir)==-1)
		return -1;
	return i;
}

int getMovieQuantity(){
	DIR * dir= opendir("DB");
	int fd=open("DB/.lock",O_RDWR|O_CREAT);
	if (fd==-1){
		return -1;
	}
	lock(fd,F_RDLCK);
	if (dir==NULL)
		return -1;
	struct dirent * dirEntry;
	int i=0;
	while ((dirEntry=readdir(dir)) !=NULL){
		i++;
	} 
	if (closedir(dir)==-1)
		return -1;
	return i-3;
}

