#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientNet.h"
#include "client_app.h"
#include "vector.h"
#define SERVER_PORT 5555
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 500

/**************************FunctionSignatures***************************/

static void SinSetting(struct sockaddr_in *_sin);
int ClientInitialization(struct sockaddr_in *_sin);
static int ConnectToServer(struct sockaddr_in *_sin, int _socketNum);
static int SendAndReceiveData(int _socketNum, char* _buffer, int _length);

/*********************SinSetting****************************/

static void SinSetting(struct sockaddr_in *_sin)
{
	memset (_sin, 0, sizeof(*_sin));
	
	_sin -> sin_family = AF_INET;
	_sin -> sin_addr.s_addr =  inet_addr(SERVER_IP);
	_sin -> sin_port = htons(SERVER_PORT);
}

/******************ClientInitialization************************/

int ClientInitialization(struct sockaddr_in *_sin)
{
int optval = 1;
int soket;

	soket = socket(AF_INET, SOCK_STREAM, 0);
	if(soket < 0)
	{
		perror("Socket fail!\n");
		return CLIENT_FAIL;
	}
		
	if(setsockopt(soket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("Setsockopt fail!\n");
		return CLIENT_FAIL;
	}
		
return soket;
}

/********************RunClientNet*************************/

void RunClientNet(int _socket, char* _buffer, int _length)
{
struct sockaddr_in sin;

	if(_buffer == NULL)
	{
		return;
	}
	
	SinSetting(&sin);
	ConnectToServer(&sin, _socket);
	SendAndReceiveData(_socket, _buffer, _length);
}

/********************SendRecive*************************/

void SendRecive(int _socket, char* _buffer, int _length)
{
	if(_buffer == NULL)
	{
		return;
	}

	SendAndReceiveData(_socket, _buffer, _length);
}

/*****************ConnectToServer********************/

static int ConnectToServer(struct sockaddr_in *_sin, int _socketNum)
{
	SinSetting(_sin);
		
	if(connect(_socketNum, (struct sockaddr*)_sin, sizeof(*_sin)) < 0)
	{	
		perror("Connect fail!\n");
		return CLIENT_FAIL;	
	}
	
return CLIENT_SUCCESS;
}

/******************SendAndReceiveData*************************/

static int SendAndReceiveData(int _socketNum, char* _buffer, int _length)
{
char* recvBuffer;
int receiveBytes;
int sentByte;
		
		sentByte = send(_socketNum, _buffer, _length, 0);
		if(sentByte < 0)
		{
			perror("Send fail!\n");	
			return CLIENT_FAIL;	
		}
		
		receiveBytes = recv(_socketNum, _buffer, BUFFER_SIZE,0);
		if(receiveBytes == 0)
		{
			printf("Connection closed!\n");
			return CLIENT_FAIL;
		}
		if(receiveBytes < 0)
		{
			perror("Receive fail!\n");
			return CLIENT_FAIL;	
		}
		
		_buffer[receiveBytes] = '\0';
		
return CLIENT_SUCCESS;
}





	
	
	
	
