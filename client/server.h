#ifndef __SERVER_H__
#define __SERVER_H__

/*********************ServerErrors********************/

typedef enum ServerErr{
LIST_CREATE_FAIL,
SOCKET_FAIL,
SETSOCKOPT_FAIL,
BIND_FAIL,
LISTEN_FAIL,
SELECT_FAIL,
DATA_MALLOC_FAIL,
LIST_PUSH_HEAD_FAIL,
ACCEPT_FAIL,
SERVER_FULL,
CONNECTION_CLOSED,
BUFFER_MALLOC_FAIL,
RECEIVE_FAIL,
SEND_FAIL
}ServerErr;

/*********************RenameToStructs********************/

typedef struct Server Server;
typedef struct  CreateInputs CreateInputs;

/*********************RenameToApplicationFanction********************/

typedef int (*AcceptClient)(int _clientId, void*_context);
typedef void (*ReceiveMessage)(Server* _server, int _clientId, void* _message, int _messageSize, void*_context);
typedef void (*CloseClient)(int _clientId, void*_context);
typedef void (*Fail)(Server* _server, ServerErr _failErr, char* _perror, void*_context);

/*********************StructCreateInputs********************/
/*Input structure that needs to be transferred to the server in the CreateServer function*/

struct CreateInputs
{
/*Application Function - Receiving new client.
Optional - NULL can be provided*/
AcceptClient m_acceptClient;
/*Application Function - Receive Messag. Note! In this function you call SendMessageToClien function.
Required - NULL cannot be provided*/            
ReceiveMessage m_receiveMessage;  
/*Application Function - Remove Client.
Optional - NULL can be provided*/      
CloseClient m_closeClient;
/*Application Function - Server Errors. Note! In this function you call DestroyServer and PauseServer functions.
Required - NULL cannot be provided*/ 	     
Fail m_fail;
/*Queue size in listen function*/ 			      
int m_clientQueueSize;
/*Maximum amount of sockets. Note! It is important to choose a number lower than 1024 (1000 is recommended).*/ 			      
int m_maxSokets;
/*Pointer that can point to anything that is important for you to pass to the server.*/ 			      
void* m_context;
/*The maximum message size that the server can receive*/
int m_maxMessageZize;
};


/*Description:
Creates a new server.

Input:
_inputs - Structure with all the data required to create the server.

Output:
NULL - if m_receiveMessage or m_fail == NULL if one of the memory allocations failed or if ServerInitialization fail.
server - Pointer to the server structure.*/
Server* CreateServer(CreateInputs* _inputs);

/*Description:
Run the server.

Input:
*_server -  Pointer to the server structure.*/
void RunServer(Server* _server);

/*Description:
Send message to clien. Note! call this function within m_receiveMessage function.

Input:
*_server -  Pointer to the server structure.
_clientId - clien socket number.
*_message - Pointer to the message you want to send.
_messageSize - Message size (in bits).

Output:
FAIL - if *_server == NULL or if send fail.
SUCCESS - If the message was sent successfully*/
int SendMessageToClien(Server* _server, int _clientId,  void* _message,  int _messageSize);

/*Description:
Destroy the server. Note! call this function within m_fail function.

Input:
*_server -  Pointer to the server structure.*/
void DestroyServer(Server* _server);

/*Description:
Pause the server. Note! call this function within m_fail function.

Input:
*_server -  Pointer to the server structure.*/
void PauseServer(Server* _server);


#endif /*#ifndef__SERVER_H__*/
