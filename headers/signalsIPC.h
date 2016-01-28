#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../lib/stdion.h"



#define SERVERFILE "./signalsIPC/server.txt"
#define CLIENTPIDS "./signalsIPC/clientpids.txt"
#define NEWCOMM SIGUSR1
#define NEWCLIENT SIGUSR2
#define REQUEST sizeof(struct com)
#define RESPONSE sizeof(struct ret)

int waitForClientPid();
int addClientPid(int clientpid);
int writeServerPid();
int getServerPid();
int readComm(void* str,int method,int clientpid);
int addComm(void* str,int method,int clientpid);
int waitSignal(int asignal);
int doesFileExists(const char* file);
void doNothing(int signal);