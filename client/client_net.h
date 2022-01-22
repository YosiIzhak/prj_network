#ifndef __CLIENT_NET_H__
#define __CLIENT_NET_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>

typedef enum ClientResult
{
CLIENT_SUCCESS,
CLIENT_FAIL,
CLIENT_NULL,
CLIENT_ALLOCATION_FAIL
} ClientResult;

typedef struct Client Client;
typedef struct GroupAndId GroupAndId;


Client* ClientCreate(void);

GroupAndId* groupAndIdCreate(void);

 int ClientInitialization(struct sockaddr_in *_sin);

void RunClientNet(Client* _client, char* _buffer, int _length);

void SendRecive(Client* _client, char* _buffer, int _length);

void DestroyClient(Client* _client);

void getClientName(Client* _client, char _str[]);

void setClientName(Client* _client, char _str[]);

void setPassword(Client* _client, char _str[]);

void GetIP (GroupAndId* _grpID, char _ip[]);

int GetGroupPID(Client* _client);

int GetUserPID(Client* _client);

#endif 
