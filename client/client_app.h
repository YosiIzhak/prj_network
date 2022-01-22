#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client_net.h"
#include "protocol.h"


/*this funcion create new client.
params- there aren't params
the function return pointer to client*/
Client* ClientCreate(void);

/*this funcion destroy client.
params- the specific client
the function return void*/
void DestroyClient(Client* _client);

/*this funcion receive the user choice in the main menu of the program.
params- the user choice.
the function return void*/
void RunMainMenu(int _resFromMenu);

/*this funcion receive open a gnome terminal for the chat of specific group
params- the IP and the name of the group. tha params are from the server.
the function return the proccess ID*/
int GroupWindow(char* _ip, char* _groupName);

/*this funcion receive open a gnome terminal for user
params- the IP and the name of the group. tha params are from the server.
the function return the proccess ID*/
int UserWindow(char* _ip,char* _userName, char* _groupName);

/*this funcion create new struct of GroupAndId for the IP and two proccess ID.
params- there aren't params
the function return pointer to this struct*/
GroupAndId* groupAndIdCreate(void);

#endif 



