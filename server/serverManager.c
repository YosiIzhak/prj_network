#include "serverManager.h"
#include "protocol.h"
#include "server.h"
#include "groupsManager.h"
#include "usersManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 5000
#define QUE_SIZE 1000
#define MAX_SOCKETS 1000
#define ACCEPT_CLIENT 1
#define MAGIC_NUMBER 144522
#define NOT_FROME_LOWD 0
#define MAX_GROUPS 100 
#define PORT "555"

/***********struct of application**************/
struct Application
{
	Server* m_server;
	CreateInputs* m_input;
	UserMng* m_users;
	GrupsMng* m_groups;
	int m_magicNumber;
};

/****************************************************************Create*****************************************************************/
/*####################internal functions#######################*/
CreateInputs* CreateInputsStruct(AcceptClient _newClient, ReceiveMessage _getMessage, CloseClient _removeClient, Fail _failErr, void* _application);
int NewClient(int _clientId, void* _application);
void GetMessage(Server* _server, int _clientId, void* _message, int _messageSize, void* _application);
void RemoveClient(int _clientId, void* _application);
void FailErr(Server* _server, ServerErr _failErr, char* _perror, void* _application);
/*########################################################*/

Application* CreateServerApplication ()
{
	Application* application;
	application = (Application*) malloc (sizeof(application));
	if (application == NULL)
	{
		return NULL;
	}
	application->m_input = CreateInputsStruct(NULL, GetMessage, NULL, FailErr, application);
	if (application->m_input == NULL)
	{
		return NULL;
	}
	application->m_server = CreateServer(application->m_input);
	if (application->m_server == NULL)
	{
		free (application->m_input);
		return NULL;
	}
	application->m_users = CreateRegisteredUsersPull (application->m_input-> m_maxSokets); 
	if (application->m_users == NULL) 
	{
		DestroyServer(application->m_server);
		free (application->m_input);
		return NULL;
	}
	application->m_groups = CreateGroupsManager (MAX_GROUPS);
	if (application->m_groups == NULL)
	{
		DestroyRegisteredUsersPull (application->m_users);
		DestroyServer(application->m_server);
		/*free (application->m_input);*/
		return NULL;
	}
	application->m_magicNumber = MAGIC_NUMBER;
	return application; 
}

/**********************************************************destroy***************************************************************************/

void DestroyServerApplication (Application* _application)
{
	if (_application == NULL || _application->m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	DestroyServer(_application->m_server);
	DestroyRegisteredUsersPull(_application->m_users);
	DestroyGroupsManager (_application->m_groups);
	free (_application->m_input);
	_application->m_magicNumber = 0;
	free (_application);
}

/**************************************runApplication*******************************************************************************************/

void RunApplication (Application* application)
{
	RunServer(application->m_server);
}
/******************************************input for create server********************************************************************************/
CreateInputs* CreateInputsStruct(AcceptClient _newClient, ReceiveMessage _getMessage, CloseClient _removeClient, Fail _failErr, void* _application)
{
	CreateInputs* inputs;
	if(_getMessage == NULL || _failErr == NULL)
	{
		return NULL;
	}
	if((inputs = (CreateInputs*) malloc(sizeof(CreateInputs))) == NULL)
	{
		return NULL;
	}
	inputs -> m_acceptClient = _newClient;
	inputs -> m_receiveMessage = _getMessage;
	inputs -> m_closeClient = _removeClient;
	inputs -> m_fail = _failErr;
	inputs -> m_clientQueueSize = QUE_SIZE;
	inputs -> m_maxSokets = MAX_SOCKETS;
	inputs -> m_context = _application;
	inputs ->  m_maxMessageZize = BUFFER_SIZE;
	return inputs;
}

/***************************************switch case function**********************************************************************************/

int WhatToDoNow (char _type, char* _buffer, void* _message, int _messageSize, void* _application)
{
	NameAndPassword user;
	int status, status2, length;
	char str[30];
	char strGroups[500] = {0};
	static char name[30];
	switch (_type)
	{
		case REGISTRATION_REQUEST: 
		/*isMassegeComlete*/
		UnpackNameAndPassword (&user, _message, _messageSize); /*function of protocol*/
		
		/*if fail...*/
		status = CreateUser (&user , ((Application*)_application)->m_users, NOT_FROME_LOWD);/*function of usersManager*/
		if (status == DUPLICATE_USERNAME)
		{
			length = PackStatusMassage (_buffer,  REGISTRATION_REQUEST_DUPLICATE_USERNAME);
		}
		else if (status == SUCCESS)
		{
			length = PackStatusMassage (_buffer, REGISTRATION_REQUEST_SUCCESS);
		}		
		else
		{
			length = PackStatusMassage ( _buffer, REGISTRATION_REQUEST_FAIL);
		}
		return length;
		break;
		case LOG_IN_REQUEST:
		/*isMassegeComlete*/
		UnpackNameAndPassword (&user, _message, _messageSize); /*function of protocol*/
		
		status = LogInUser (&user , ((Application*)_application)->m_users);
		if (status == WRONG_DETAILS)
		{
			length = PackStatusMassage (_buffer, LOG_IN_REQUEST_WRONG_DETAILS);
		}
		else if (status == SUCCESS)
		{
			length = PackStatusMassage (_buffer, LOG_IN_REQUEST_SUCCESS);
		}
		else
		{
			length = PackStatusMassage (_buffer, LOG_IN_REQUEST_FAIL);
		}
		return length;
		break;		
		case OPEN_NEW_GROUP_REQUEST:
		/*isMassegeComlete*/
		UnpackNameAndPassword (&user, _message, _messageSize);
		/*status = IsUsernameCorrect (((Application*)_application)->m_users, user.m_password);
		if (status == NO_FOUND_IN_HASH)
		{
			length = PackStatusMassage (_buffer, LOG_IN_REQUEST_WRONG_DETAILS);
		}*/
		strcpy (name, user.m_name);		
		status = CreateGroup (name , ((Application*)_application)->m_groups, str);
		if (status == GROUP_SUCCESS)
		{
			status2 = UserJoinGroup (((Application*)_application)->m_users, user.m_password ,user.m_name);
			if (status2 == SUCCESS)
			{
				strcpy (user.m_name, str);
				strcpy (user.m_password, name);
				length = PackNameAndPassword ( &user,_buffer, OPEN_NEW_GROUP_SUCCESS);
				printf ("buffer: %s\n", _buffer);
			}
		}
		else if (status == DUPLICATE_GROUP_NAME_FAIL)
		{
			length = PackStatusMassage (_buffer, DUPLICATE_GROUP_NAME);
			printf ("status : %d \n buffer: %s\n", status, _buffer);	
		}
		else
		{
			length = PackStatusMassage (_buffer, OPEN_NEW_GROUP_FAIL);
		}
		return length;
		break;
		case PRINT_EXISTING_GROUPS_REQUEST:

			giveGroups(((Application*)_application)-> m_groups, strGroups);
			length =  PackStringMassage(strGroups , _buffer, PRINT_EXISTING_GROUPS_SUCCESS);
			return length;
			
		case JOIN_EXISTING_GROUP_REQUEST:
		/*isMassegeComlete*/
		UnpackNameAndPassword (&user, _message, _messageSize);
		/*status = IsUsernameCorrect (((Application*)_application)->m_users, user.m_name);
		if (status == NO_FOUND_IN_HASH)
		{
			length = PackStatusMassage (_buffer, LOG_IN_REQUEST_WRONG_DETAILS);
		}*/
		strcpy (name, user.m_name);
		status = JoinExistingGroup (user.m_name , ((Application*)_application)->m_groups, str);	
		if (status == GROUP_SUCCESS)
		{
			status2 = UserJoinGroup (((Application*)_application)->m_users, user.m_password ,user.m_name);
		/****/	if (status2 == CONNECT_TO_SAME_GROUP)
			{
				length = PackStatusMassage (_buffer,/**/ DUPLICATE_GROUP_CONNECT/**/);
			}
			else if (status2 == SUCCESS)
			{
				strcpy (user.m_name, str);
				strcpy (user.m_password, name);	
				length = PackNameAndPassword ( &user,_buffer, JOIN_EXISTING_GROUP_SUCCESS);
			}
			else
			{
				length = PackStatusMassage (_buffer, JOIN_EXISTING_GROUP_FAIL);
			}/****/	
		}
		else if (status == GROUP_NOT_FOUND)
		{
			length = PackStatusMassage (_buffer, GROUP_NOT_FOUND);
		}
		else 
		{
			length = PackStatusMassage (_buffer, JOIN_EXISTING_GROUP_FAIL);		
		}									
		return length;
		break;
		
		case LEAVE_GROUP_REQUEST:									
		/*isMassegeComlete*/
		UnpackNameAndPassword (&user, _message, _messageSize);
		/*status = IsUsernameCorrect (((Application*)_application)->m_users, user.m_name);
		if (status == NO_FOUND_IN_HASH)
		{
			length = PackStatusMassage (_buffer, LOG_IN_REQUEST_WRONG_DETAILS);
		}*/
		strcpy (name, user.m_name);
		status = LeaveGroup (user.m_name , ((Application*)_application)->m_groups,str);
		if (status == GROUP_SUCCESS)
		{
		/**/	status2 = UserLeaveGroup (((Application*)_application)->m_users,user.m_password, user.m_name );
			if (status2 == SUCCESS)
			{
				strcpy (user.m_name, str);
				strcpy (user.m_password, name);	
				length = PackNameAndPassword ( &user,_buffer, LEAVE_GROUP_SUCCESS);	
			}
			else if (status2 == NO_FOUND_IN_HASH)
			{
				length = PackStatusMassage (_buffer, GROUP_NOT_FOUND);
			}
			else
			{
				length = PackStatusMassage (_buffer, LEAVE_GROUP_FAIL);
			}/**/					
		}
		else if (status == GROUP_NOT_FOUND_IN_HASH)
		{
			length = PackStatusMassage (_buffer, GROUP_NOT_FOUND);
		}
		else if (status == GROUP_DELETE)
		{
		/**/	status2 = UserLeaveGroup (((Application*)_application)->m_users, user.m_name ,user.m_password);
			if (status2 == SUCCESS)
			{		
				length = PackStatusMassage (_buffer, GROUP_DELETED);	
			}
			else if (status2 == NO_FOUND_IN_HASH)
			{
				length = PackStatusMassage (_buffer, GROUP_NOT_FOUND);
			}
			else
			{
				length = PackStatusMassage (_buffer, LEAVE_GROUP_FAIL);
			}		/***/
		}
		else 
		{
			length = PackStatusMassage (_buffer, LEAVE_GROUP_FAIL);		
		}
		return length;
		
	/***/	/*case LEAVE_CHAT_REQUEST:
		isMassegeComlete
		UnpackStringMassage(name, _message, _messageSize);
		status = UserLogOut (name);
		if (status == NO_FOUND_IN_HASH)
		{

		}*/
	}
}

/*****************************************************************************************************************************************/

int NewClient(int _clientId, void* _application)
{
	return ACCEPT_CLIENT;
}

/****************************************************************************************************************************************/
void GetMessage(Server* _server, int _clientId, void* _message, int _messageSize, void* _application)
{
	int len;
	char type;
	char buffer [BUFFER_SIZE];
	type = ReturnMessageType ( _message); /*function of protocol*/
	len = WhatToDoNow (type, buffer, _message, _messageSize, _application);
	if (len != -1) /*define*/
	{
		printf ("send: %d ", *buffer);
		printf ("%d ", *(buffer+1));
		printf ("%s\n", buffer);		
		SendMessageToClien(_server, _clientId, buffer, len);
	}
}

/****************************************************************************************************************************************/

void RemoveClient(int _clientId, void* _application)
{
}

/****************************************************************************************************************************************/

void FailErr(Server* _server, ServerErr _failErr, char* _perror, void* _application)
{
	char error[20];
	switch (_failErr)
	{
		case 0: strcpy (error, "list create fail"); break;
		case 1: strcpy (error,"socket fail"); break;
		case 2: strcpy (error,"setsockopt fail"); break;
		case 3: strcpy (error, "bind fail"); break;
		case 4: strcpy (error, "listen fail"); break;
		case 5: strcpy (error, "select fail"); break;
		case 6: strcpy (error, "data malloc fail"); break;
		case 7: strcpy (error, "list push head fail"); break; 
		case 8: strcpy (error,"accept fail"); break;
		case 9: strcpy (error, "server full"); break;
		case 10: strcpy (error, "connection closed"); break;
		case 11: strcpy (error, "buffer malloc fail"); break;
		case 12: strcpy (error, "receive fail"); break;
		case 13: strcpy (error, "send fail"); break;
	}
	printf("%s : %s\n", error, _perror);
	if(_failErr<=8)
	{
		PauseServer(_server);
		DestroyServer(_server);	
	}
}

