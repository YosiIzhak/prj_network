#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client_net.h"
#include "client_app.h"
#include "vector.h"
#define SERVER_PORT 5555
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 500
#define MAGIC_NUMBER 8123475
#define GROUPS_AMOUNT 20



/*void GetIP (GroupAndId* _grpID, char _ip[]) 
{
	if (_grpID == NULL)
	{
		return;
	}
	strcpy (_ip, _grpID->m_groupIp);
}*/
/*******************GetUserPID***********************/
/*int GetUserPID(Client* _client)
{
	 return _client-> m_groupVector -> m_idUser;
}*/
/*******************GetGroupPID***********************/
/*int GetGroupPID(Client* _client)
{
	return _client->m_groupVector->m_idGroup;
}*/

/**************************FunctionSignatures***************************/

static void SinSetting(struct sockaddr_in *_sin);
int ClientInitialization(struct sockaddr_in *_sin);
static int ConnectToServer(struct sockaddr_in *_sin, int _socketNum);
static int SendAndReceiveData(int _socketNum, char* _buffer, int _length);


int soket;

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
/*int socketNum;*/
int optval = 1;

	/*socketNum*/ soket = socket(AF_INET, SOCK_STREAM, 0);
	if(soket /*socketNum*/ < 0)
	{
		perror("Socket fail!\n");
		return CLIENT_FAIL;
	}
		
	if(setsockopt(soket/*socketNum*/, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("Setsockopt fail!\n");
		return CLIENT_FAIL;
	}
		
return /*socketNum*/ soket;
}

/********************RunClientNet*************************/

void RunClientNet(Client* _client, char* _buffer, int _length)
{
struct sockaddr_in sin;

	if(_client == NULL)
	{
		return;
	}
	
	SinSetting(&sin);
	ConnectToServer(&sin, soket /*_client -> m_socket*/);
	SendAndReceiveData(soket /*_client -> m_socket*/, _buffer, _length);
}

/********************SendRecive*************************/

void SendRecive(Client* _client, char* _buffer, int _length)
{
	if(_client == NULL)
	{
		return;
	}

	SendAndReceiveData(soket/*_client -> m_socket*/, _buffer, _length);
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



/*******************getClientName***********************/

/*void getClientName(Client* _client, char _str[])
{
	strcpy(_str, _client ->  m_name);
}*/
/*******************setClientName***********************/

/*void setClientName(Client* _client, char _str[])
{
	strcpy(_client ->  m_name, _str);
}*/

/*******************setPassword***********************/

/*void setPassword(Client* _client, char _str[])
{
	strcpy(_client ->  m_passWord, _str);
}*/





	
	
	
	
