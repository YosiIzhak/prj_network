#ifndef __USER_STRUCT_H__
#define __USER_STRUCT_H__
#include "protocol.h"
#include "gen.list.h"
#define ACTIVE 1
#define NO_ACTIVE 0
#define SIZE 30

typedef enum userStrResult
{
	USER_STRUCT_SUCCESS,
	USER_STRUCT_MALLOC_FAIL,
	USER_STRUCT_NOT_FOUND_IN_HASH,
	ALMOST_IN_THE_GROUP /****/
}userStrResult;

struct User
{
	char m_username[SIZE];
	char m_password[SIZE];
	List* m_groups;
	int m_active;
	int m_magicNumber;
};
typedef struct User User;

User* CreateNewUser (NameAndPassword* _namePsw);

void DestroyUser (User* _user);

void GetUserPassword (User* _user, char _password[]);

int SetUserStatus (User* _user, int _status);

int RemoveGroupFromUser (User* _user, char* _groupName);

int AddGroupForUser (User* _user, char* _groupName);

#endif
