#include "headers/client.h"
#include "headers/common.h"
#include <string.h>

int getBoardClient (char buf[][50]){
	struct com command;
	command.op=SEE_BOARD;
	sendCommand(&command,getpid());
	struct ret ret;
	receiveAnswer(&ret,getpid());
	int i = 0;
	for (; i < 10; ++i)
	{
		strcpy(buf[i],ret.board[i]);	
	}
	buf=ret.board;
	return ret.extra;


}