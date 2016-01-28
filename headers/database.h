/*
API para manejar la base de datos. Esta API la va a usar los clientes en el primer modelo y los workers en el segundo modelo.
*/

#define TRUE 1
#define FALSE 0
#define MOVIE_SEATS 50
#define MOVIE_SIZE 53


/*
Recibe el nombre de la pelicula, crea una funcion con MOVIE_SEATS lugares y retorna:
0: si OK;
-1: si ya existe la pelicula
-2: si hubo algun error
-3: si ya hay 10 peliculas en cartelera
No lockea ningun archivo.
*/
int addMovie (char * movie_name);

/*
Recibe el nombre de la pelicula y lo elimina del directorio, retorna:
0: si OK;
-1: si no esta la pelicula en cartelera
-2: si hubo error
Hace un lock de lectura/escritura sobre todo el archivo de la pelicula.
*/
int deleteMovie (char * movie_name);

/*
Recibe el nombre de la pelicula y un vector previamente allocado donde se llena la disponibilidad. Retorna:
0: OK
-1: si no existe pelicula
-2: si hubo algun error
Hace un lock de lectura sobre el archivo de la pelicula.
*/
int checkAvailability (char * movie_name, char * buffer);


/*
Recibe un matriz donde se van a copiar las peliculas de la cartelera. Retorna:
n: las cantidad de peliculas en cartelera (<=10)
-1: si hubo algun error
Realiza un lock de lectura sobre el directorio que contiene la base de datos (?).
*/
int getBoard (char  buf[][50]);

/*
Recibe el nombre de la pelicula y un array de enteros que representan los asientos que se desean adquirir. Retorna:
>0: id de la compra
-1: si hubo algun error
-2: si estan ocupados esos asientos
-3: la pelicula no existe
Realiza un lock de escritura sobre parte del archivo de la pelicula.
*/
int buyTickets (char * movie_name, int seats[], int lenght);

/*
Recibe el nombre de la pelicula y un entero que representa el id de la compra. Retorna:
0: OK
-1: si no existe esa compra para esa pelicula
-2: si hubo algun error
Reliza un lock de escritura sobre parte del archivo de la pelicula.
*/
int cancelTickets (char * movie_name, int id);

/*
Retorna:
n (>0): siendo n la cantidad de peliculas en cartelera.
-1: si hubo algun error.
*/
int getMovieQuantity();




