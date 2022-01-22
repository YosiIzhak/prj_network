#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>
#include <errno.h>
#include "list.h"
#include "gen.list.h"
#include "server.h"
#define SERVER_PORT 5555
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 5500
#define FAIL 0
#define SUCCESS 1
#define TRUE 1
#define MAX_FD 1024
#define OFF 0
#define ON 1
#define ACCEPT_CLIENT 1
#define REJECT_CLIENT 0
#define MAGIC_NUMBER 8123475

/********************************StructServer******************************/

struct Server
{
AcceptClient m_acceptClient;
ReceiveMessage m_receiveMessage;
CloseClient m_closeClient;
Fail m_fail;
List* m_list;
fd_set* m_socketsList;
void* m_context;
int m_listenSocket;
int m_maxSokets;
int m_pause;
int m_magicNumber;
int m_maxMessageZize;
};

/**************************GlobalVariables***************************/

int clientsCountr = 1;

/**************************FunctionSignatures***************************/

static socklen_t SinSetting(struct sockaddr_in *_sin);
static int ServerInitialization(Server* _server, struct sockaddr_in *_sin, fd_set* _socketsList, int _queueSize);
static int AcceptNewClient(Server* _server, struct sockaddr_in *_clientSin);
static int SendAndReceiveData(Server* server, int _clientSocket, List* _list);
static void TakeCareClients(Server* _server, fd_set* _activeSockets, int* _activity);
static void CloseAllClient(List* list);

/**************************CreateServer***************************/

Server* CreateServer(CreateInputs* _inputs)
{
struct sockaddr_in sin;
static fd_set socketsList;
Server* server;
	
	if(_inputs -> m_receiveMessage == NULL || _inputs -> m_fail == NULL)
	{
		return NULL;
	}
	
	if((server = (Server*) malloc(sizeof(Server))) == NULL)
	{
		return NULL;
	}
	
	FD_ZERO(&socketsList);
	
	server -> m_acceptClient = _inputs ->  m_acceptClient;
	server -> m_receiveMessage = _inputs ->  m_receiveMessage;
	server -> m_closeClient = _inputs -> m_closeClient;
	server -> m_fail = _inputs -> m_fail;
	server -> m_maxSokets = _inputs -> m_maxSokets;
	server -> m_context = _inputs -> m_context;
	server -> m_pause = OFF;
	server -> m_magicNumber = MAGIC_NUMBER;
	server -> m_socketsList  = &socketsList;


	if((server -> m_list = ListCreate()) == NULL)
	{
		return NULL;
	}

	if((server ->  m_listenSocket = ServerInitialization(server, &sin, server -> m_socketsList, _inputs -> m_clientQueueSize)) == FAIL)
	{
		ListDestroy(&server -> m_list, NULL);
		return NULL;
	}
		
return server;
}

static socklen_t SinSetting(struct sockaddr_in *_sin)
{
	memset (_sin, 0, sizeof(*_sin));
	
	_sin -> sin_family = AF_INET;
	_sin -> sin_addr.s_addr =  inet_addr(SERVER_IP);
	_sin -> sin_port = htons(SERVER_PORT);
	
return sizeof(*_sin);
}

static int ServerInitialization(Server* _server, struct sockaddr_in *_sin, fd_set* _socketsList, int _queueSize)
{
int listenSocket;
socklen_t sinLen;
int optval = 1;
int flags;

	/**********************OpenListenSocket***********************/

	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(listenSocket < 0)
	{
		_server -> m_fail(_server, SOCKET_FAIL, strerror(errno), _server -> m_context);
		return FAIL;
	}
	
	/****************EnterTo fd_set forSelect*****************/
	
	FD_SET(listenSocket, _socketsList);	
	
	/******************FreesListenSocketImmediately*****************/
		
	if(setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		_server -> m_fail(_server, SETSOCKOPT_FAIL, strerror(errno), _server -> m_context);
		return FAIL;
	}
	
	/******************SetFixIpAndPortForServer*****************/
	
	sinLen = SinSetting(_sin);
			
	if(bind(listenSocket, (struct sockaddr*)_sin, sinLen) < 0)
	{
		_server -> m_fail(_server, BIND_FAIL, strerror(errno), _server -> m_context);
		return FAIL;
	}
	
	/****************SetListenSocketOnlyForListen****************/
		
	if(listen(listenSocket, _queueSize) < 0)
	{
		_server -> m_fail(_server, LISTEN_FAIL, strerror(errno), _server -> m_context);	
		return FAIL;
	}	

return listenSocket;
}

/**************************RunServer***************************/

void RunServer(Server* _server)
{
struct sockaddr_in clientSin;
fd_set activeSockets;
static int activity;

	if(_server == NULL)
	{
		return;
	}
	
	FD_ZERO(&activeSockets);
	
	while(_server -> m_pause == OFF)
	{	
		activeSockets = *(_server -> m_socketsList);	
		activity = select(MAX_FD, &activeSockets, NULL, NULL, NULL);
		if((activity < 0) && (errno != EINTR))
		{
			_server -> m_fail(_server, SELECT_FAIL, strerror(errno), _server -> m_context);		
		}
		
		if(FD_ISSET(_server -> m_listenSocket, &activeSockets))
		{
			AcceptNewClient(_server, &clientSin);
			activity--;
		}

		TakeCareClients(_server, &activeSockets, &activity);
	}	
}

static int AcceptNewClient(Server* _server, struct sockaddr_in *_clientSin)
{
socklen_t clientSinLen;
int clientSocket;
int* listData;
int flags;

		clientSinLen = sizeof(*_clientSin);
		clientSocket = accept(_server -> m_listenSocket, (struct sockaddr*) _clientSin, &clientSinLen);
		if(clientSocket < 0)
		{
			_server -> m_fail(_server, ACCEPT_FAIL, strerror(errno), _server -> m_context);				
			return FAIL;				
		}
		else if (clientsCountr < _server -> m_maxSokets)
		{
			if (_server -> m_acceptClient == NULL || _server -> m_acceptClient(clientSocket, _server -> m_context) == ACCEPT_CLIENT)
			{								
				/****************EnterTo fd_set forSelect*****************/
			
				FD_SET(clientSocket, _server -> m_socketsList);	
			
				/***************EnterSocketToList***************/	

				if((listData = (int*) malloc(sizeof(int))) == NULL)
				{
					_server -> m_fail(_server, DATA_MALLOC_FAIL, strerror(errno), _server -> m_context);
					return FAIL;
				}
				*listData = clientSocket;
				if(ListPushHead(_server -> m_list, listData) != LIST_SUCCESS)
				{
					_server -> m_fail(_server, LIST_PUSH_HEAD_FAIL, strerror(errno), _server -> m_context);
					return FAIL;		
				}
			
				clientsCountr++;
				return clientSocket;
			}
			if(_server -> m_closeClient != NULL)
			{
				_server -> m_closeClient(clientSocket, _server -> m_context);
			}
			close(clientSocket);	
			return SUCCESS;		
		}
		
		_server -> m_fail(_server, SERVER_FULL, "Server full!\n", _server -> m_context);
		if(_server -> m_closeClient != NULL)
		{	
			_server -> m_closeClient(clientSocket, _server -> m_context);
		}
		close(clientSocket);	
		return SUCCESS;							
}

static void TakeCareClients(Server* _server, fd_set* _activeSockets, int* _activity)
{
int* clientSocket;
ListItr nodeEnd;
ListItr node;

	node = ListItrBegin(_server -> m_list);
	nodeEnd = ListItrEnd(_server -> m_list);
		
	while((node != nodeEnd) && (*_activity > 0)) 
	{
		clientSocket = ListItrGet(node);
		
		if(FD_ISSET(*clientSocket, _activeSockets))
		{
			if(SendAndReceiveData(_server, *clientSocket,  _server -> m_list) == FAIL)
			{
				FD_CLR(*clientSocket, _server -> m_socketsList);
				node = ListItrNext(node);
				close(*clientSocket);
				free(ListItrGet(ListItrPrev(node)));	
				ListItrRemove( ListItrPrev(node));
				clientsCountr--;
				if(_server -> m_closeClient != NULL)
				{
					_server -> m_closeClient(*clientSocket, _server -> m_context);
				}
			}
			else
			{		
				node = ListItrNext(node);
			}
			*_activity--;
		}
		else
		{		
			node = ListItrNext(node);
		}	 
	} 
}

static int SendAndReceiveData(Server* _server, int _clientSocket, List* _list)
{
char buffer[BUFFER_SIZE];
int receiveBytes;

		receiveBytes = recv(_clientSocket, buffer, sizeof(buffer),0);
		if(receiveBytes == 0)
		{
			_server -> m_fail(_server, CONNECTION_CLOSED, strerror(errno), _server -> m_context);
			return FAIL;
		}
		if(receiveBytes < 0)
		{
			_server -> m_fail(_server, RECEIVE_FAIL, strerror(errno), _server -> m_context);
			return FAIL;
		}

		buffer[receiveBytes] = '\0';

		_server -> m_receiveMessage (_server, _clientSocket, buffer, receiveBytes, _server -> m_context);
		
return SUCCESS;
}

/**************************SendMessageToClien***************************/

int SendMessageToClien(Server* _server, int _clientId,  void* _message,  int _messageSize)
{
int sentByte;

	if(_server == NULL)
	{
		return FAIL;
	}

	sentByte = send(_clientId, (char*)_message, _messageSize, 0);
	if(sentByte < 0)
	{
		_server -> m_fail(_server, SEND_FAIL, strerror(errno), _server -> m_context);
		/*close*/
		return FAIL;		
	}
	
return SUCCESS;	
}

/**************************DestroyServer***************************/

void DestroyServer(Server* _server)
{
	if(_server == NULL || _server ->  m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	
	_server ->  m_magicNumber = 0;

	CloseAllClient(_server -> m_list);
	ListDestroy(&_server -> m_list, NULL);
	free(_server);
}


static void CloseAllClient(List* _list)
{
int* clientSocket;
ListItr nodeEnd;
ListItr node;

	node = ListItrBegin(_list);
	nodeEnd = ListItrEnd(_list);
		
	while(node != nodeEnd) 
	{
		clientSocket = ListItrGet(node);
		close(*clientSocket);
		free(ListItrGet(node));
		node = ListItrNext(node);	
	} 
}

/**************************PauseServer***************************/

void PauseServer(Server* _server)
{
	if(_server == NULL)
	{
		return;
	}

	_server -> m_pause = ON;
}


