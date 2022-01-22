#ifndef __GROUP_STRUCT_H__
#define __GROUP_STRUCT_H__
#include "protocol.h"
#include "list.h"
#define SIZE 30



struct User
{
	char m_username[SIZE];
	char m_password[SIZE];
	List* m_groups;
	int m_active;
	int m_magicNumber;
};


typedef struct Group Group;

Group* CreateNewGroup (char _groupName[]);

void DestroyGroup (Group* _group);

void GetGroupIp (Group* _group, char _groupIp[]);

int GetGroupNumOfClients (Group* _group);

void DecreseNumOfCleints (Group* _pValue);

void IncreseNumOfCleints (Group* _pValue);

int GetGroupNumOfClients(Group* _pValue);

void SetGroupIp (Group* _group, char _groupIp[]);

#endif
