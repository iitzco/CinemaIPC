#include "headers/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <mqueue.h>
#include <signal.h>
#include <sys/stat.h>

#define SERV_Q "/cola_servidor"
#define SERV_Q_AUX "/cola_servidor_aux"
#define RET_Q "/cola_ret%d"
#define CLIENT_Q "/cola_cliente_%d"
#define MQ_MESSAGE_PRIORITY 0


int sendCommand (struct com * command,int pid){

	mqd_t mqdServer, mqdClient, mqdClientWrite, mqdServerAux;

	struct mq_attr attrServer;
	attrServer.mq_maxmsg = 10;
	attrServer.mq_msgsize = sizeof(int);	
	struct mq_attr attrServerAux;
	attrServerAux.mq_maxmsg = 10;
	attrServerAux.mq_msgsize = sizeof(int);	

	int n = pid;
	int ret;

	if((mqdServer=mq_open(SERV_Q, O_WRONLY|O_CREAT, 0666, &attrServer))==-1){
		perror("Error en mq_open en sendCommand");
		return -1;
	}

	if((mqdServerAux=mq_open(SERV_Q_AUX, O_RDONLY|O_CREAT, 0666, &attrServerAux))==-1){
		perror("Error en mq_open del serveraux en sendCommand");
		return -1;
	}
	if (mq_receive(mqdServerAux, (char*)&ret, sizeof(int), 0) < 0){
		perror("No puede leer mq_receive el 1 del servidor en sendCommand\n");
		return -1;
	}
	if(mq_send(mqdServer, (char*) &n,  sizeof(int), MQ_MESSAGE_PRIORITY)<0){
		perror("Error en mq_send en sendCommand");
		return -1;
	}

	char mqpidpath[30]={0};
	sprintf(mqpidpath,CLIENT_Q, pid);

	struct mq_attr attr_client;
	attr_client.mq_maxmsg = 10;
	attr_client.mq_msgsize = sizeof(struct com);
	
	struct mq_attr attr_client_write;
	attr_client_write.mq_maxmsg = 10;
	attr_client_write.mq_msgsize = sizeof(struct com);

	if (mqdClient = mq_open(mqpidpath, O_CREAT, 0666, &attr_client)==-1){
		return -1;
	}
	
	mqdClientWrite = mq_open(mqpidpath, O_WRONLY, 0666, &attr_client_write);

	if(mqdClientWrite==-1 || mq_send(mqdClientWrite, (char *) command, sizeof(struct com), MQ_MESSAGE_PRIORITY)==-1){
		return -1;
	}
}

int receiveCommand (struct com * command, int pidClient){
	mqd_t mqd_client;
	struct mq_attr attr_client;
	attr_client.mq_maxmsg=10;
	attr_client.mq_msgsize=sizeof(struct com);

	char mqpidpath[30]={0};
	sprintf(mqpidpath,CLIENT_Q,pidClient);
	
	while((mqd_client=mq_open(mqpidpath, O_RDONLY, 0666, &attr_client))==-1){
	}

	if(mq_receive(mqd_client, (char*) command, sizeof(struct com), NULL) < 0){
		perror("Error en receive de receiveCommand");
		//return -1;
	}
}


int sendAnswer (struct ret * ret, int pid){
	mqd_t  mqdClient, mqdClientWrite;

	char mqpidpath[30]={0};
	sprintf(mqpidpath,RET_Q, pid);

	struct mq_attr attr_client;
	attr_client.mq_maxmsg=10;
	attr_client.mq_msgsize=sizeof(struct ret);

	if( (mqdClient = mq_open(mqpidpath,O_WRONLY|O_CREAT, 0666, &attr_client))==-1){
		perror("Error en mq_open en sendAnswer");
		return -1;
	}
	
	while(mq_send(mqdClient, (char * ) ret,  sizeof(struct ret), MQ_MESSAGE_PRIORITY)<0){
		perror("Error en mq_send de sendAnswer");
	}

}

int receiveAnswer (struct ret * ret, int pid){
	mqd_t mqd_client;
	
	struct mq_attr attr;
	attr.mq_maxmsg=10;
	attr.mq_msgsize=sizeof(struct ret);

	char mqpidpath[30]={0};
	sprintf(mqpidpath, RET_Q, pid);

	while((mqd_client=mq_open(mqpidpath, O_RDONLY|O_CREAT, 0666, &attr))==-1){
		int x=0;
		while(x++<100000000){}
		perror("Error en mq_open en receiveAnswer");
	}
		
	if(mq_receive(mqd_client, (char*) ret, sizeof(struct ret), 0) < 0){
		perror("Error en mq_receive en receiveAnswer");
	}
}


int receivePID (){
	
	//Descriptores
	mqd_t mqdServer, mqdServerRead, mqdServerAux;
	int num, n =1;
	
	//Atributos de mi server queue
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(int);	

	if((mqdServerAux=mq_open(SERV_Q_AUX, O_WRONLY|O_CREAT, 0666, &attr))==-1){
		//perror("Error en mq_open en receivepid");
		return -1;
	}
		

	if(mq_send(mqdServerAux, (char*) &n, sizeof(int), MQ_MESSAGE_PRIORITY)<0){
		//perror("error en mq_send en receivepid");
		return -1;
	}

	if((mqdServerRead=mq_open(SERV_Q, O_RDONLY, 0666, &attr))==-1){
		//perror("Error en mq_open en receivepid");
		return -1;
	}
		

	if(mq_receive(mqdServerRead, (char*) &num, sizeof(int), 0)==-1){
		//perror("error en mq_receive el receivepid");
	}

	return num;	

}

