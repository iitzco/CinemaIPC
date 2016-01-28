/*
Cada mecanismo de IPC va a implementar esta API. 
*/

#define ADD_MOVIE 0
#define DEL_MOVIE 1
#define SEE_BOARD 2
#define BUY_TICK 3
#define CANCEL_TICK 4
#define AVAIL 5
#define EXIT 6


struct com
{
	int op;
	char info[50];
	int seats[50];
	int extra;
};

struct ret
{
	char info[50];
	char board[10][50];
	int extra;
};


int sendCommand (struct com * command);

int receiveCommand (struct com * command, int fd);

int sendAnswer (struct ret * ret, int fd);

int receiveAnswer (struct ret * ret, int fd);

int receiveRequest (int fdSocketServer);

int initializeServerSocket();
