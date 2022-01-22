#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "groupsManager.h"
#include "groupStruct.h"
#include "hash.map.h"
#include "queue.h"
#include <math.h>
#define MAGIC_NUMBER 144522
#define QUEUE_SIZE 100
#define BUFFER_SIZE 800

/**************************FunctionSignatures***************************/

static void FillTheIpDatabase(Queue* _queue);
void DestroyGroupGen (void* _group);
void DestroyIp (void* _ip);
void DestroyGroupName (void* _groupName);
static void printKey(void* _word);
static void printValue(void* _value);

/**************************StructGrupsMng***************************/

struct GrupsMng
{
	HashMap* m_groupDatabase;
	Queue* m_ipDatabase;
	int m_magicNumber;
};

/**************************ConvertGroupNameForHash***************************/

size_t ConvertGroupNameForHash (void* _key)
{
int i;
char letter;
size_t multi = 1;
int len = strlen ((char*)_key);

	for (i=0; i< len; i++)
	{
		letter = ((char*)_key)[i];
		multi *= letter;
	}
	
return multi;
}

/**************************CompareGroupsNames***************************/

int CompareGroupsNames (void* _firstKey, void* _secondKey)	
{
	if (strcmp ((char*)_firstKey, (char*)_secondKey) == 0)
	{
		return 1;
	}
	
return 0;
}

/**************************CreateGroupsManager***************************/

GrupsMng* CreateGroupsManager (int _capacity)
{
GrupsMng* manager;
HashMap* map;
Queue* queue;

	manager = (GrupsMng*) malloc (sizeof (GrupsMng));
	if (manager == NULL)
	{
		return NULL;
	}
	
	map = HashMapCreate(_capacity, ConvertGroupNameForHash, CompareGroupsNames);
	if (map == NULL)
	{
		return NULL; 
	}
		
	queue = QueueCreate(QUEUE_SIZE);
	if (queue == NULL)
	{
		return NULL; 
	}
	manager -> m_groupDatabase = map;
	manager -> m_ipDatabase = queue;
	
	FillTheIpDatabase(manager -> m_ipDatabase);
	
return manager;
}

/**************************FillTheIpDatabase***************************/

int printQueue (const void* _element, void* _context)
{
	printf ("%s\n", (char*)_element);
	return 1;
}

static void FillTheIpDatabase(Queue* _queue)
{
int i;
void* ip;
char ipTemp [IP_SIZE] = "224.255.255.";
char buf[IP_SIZE];

	for(i = 1; i < QUEUE_SIZE; i++)
	{
		if ((ip = (void*) malloc (sizeof (void*))) == NULL)
		{return;}
		sprintf(buf, "%d", i);
		strcpy(ip, ipTemp);
		strcat(ip, buf);
		QueueInsert(_queue, ip);	
	}
}


/**************************DestroyGroupsManager***************************/

void DestroyGroupsManager (GrupsMng* _manager)
{
HashMap* map;
Queue* queue;

	if ( _manager == NULL ||  _manager -> m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	map = (HashMap*)_manager -> m_groupDatabase;
	queue = (Queue*) _manager -> m_ipDatabase;
	
	_manager -> m_magicNumber = 0;
	
	HashMapDestroy(&map, DestroyGroupName, DestroyGroupGen);
	QueueDestroy(&queue, DestroyIp);	
	free (_manager);
}

/**************************CreateGroup***************************/

GrupsMngResult CreateGroup (char _groupName[] , GrupsMng* _manager, char _buffer[])
{
GrupsMngResult result;
void* pValue;
Group* group;
int status;
char* ip;
char* groupName;
int len;

	if (_manager == NULL)
	{printf ("manager null\n"); return MANAGER_NOT_INITIALIZED;}
	
	result = HashMapFind(_manager -> m_groupDatabase , _groupName , &pValue);
	if (result == MAP_KEY_NOT_FOUND_ERROR)
	{	
	len = strlen (_groupName);
		groupName = (char*) malloc (sizeof(char)* len);
		if (groupName == NULL)
		{
			return GROUP_NOT_INITIALIZED;
		}
		strcpy (groupName, _groupName);
		group = CreateNewGroup (_groupName);
		if (group == NULL)
		{
				printf ("group null\n");
			return GROUP_NOT_INITIALIZED;
		}
		status = HashMapInsert( _manager -> m_groupDatabase, groupName, group);
		HashMapPrint(_manager -> m_groupDatabase, printKey, printValue);
		if (status != MAP_SUCCESS)
		{
						printf ("insert fail\n");
			DestroyGroup (group);
			return HASH_MAP_INSERT_FAIL;
		}
		
		QueueRemove(_manager -> m_ipDatabase , (void*)&ip);
		SetGroupIp (group, ip);
		strcpy (_buffer, ip);
		printf ("success\n");
		return GROUP_SUCCESS;
	}
			printf ("out\n");
	
return DUPLICATE_GROUP_NAME_FAIL;
}


/**************************JoinExistingGroup***************************/

GrupsMngResult JoinExistingGroup (char _groupName[] , GrupsMng* _manager, char _buffer[])
{
MapResult result;
void* pValue;

	if (_manager == NULL)
	{return MANAGER_NOT_INITIALIZED;}
	
	result = HashMapFind(_manager -> m_groupDatabase , _groupName , &pValue);
	if (result == MAP_KEY_NOT_FOUND_ERROR)
	{return GROUP_NOT_FOUND_IN_HASH;}
	GetGroupIp ((Group*) pValue, _buffer);
	IncreseNumOfCleints ((Group*)pValue );
	
return GROUP_SUCCESS;
}

/**************************LeaveGroup***************************/

GrupsMngResult LeaveGroup (char _groupName[] , GrupsMng* _manager, char _buffer[])
{
void* pValue;
void* pKey;
MapResult result;

	if (_manager == NULL)
	{return MANAGER_NOT_INITIALIZED;}
	
	result = HashMapFind(_manager -> m_groupDatabase , _groupName , &pValue);
	printf ("I want to leave %s\n", _groupName);
	printf ("leave: \n");
	HashMapPrint(_manager -> m_groupDatabase, printKey, printValue);
	if (result == MAP_KEY_NOT_FOUND_ERROR)
	{return GROUP_NOT_FOUND_IN_HASH;}
	
	DecreseNumOfCleints ((Group*)pValue);
	GetGroupIp ((Group*) pValue, _buffer);
	printf ("num of clients in group: %d\n", GetGroupNumOfClients((Group*)pValue ));
	if(GetGroupNumOfClients((Group*)pValue ) == 0)
	{
		HashMapRemove(_manager -> m_groupDatabase, _groupName, &pKey, &pValue);
		free(pValue);
		return GROUP_DELETE;
	}
	
return GROUP_SUCCESS;
}

/**************************DestroyGroupGen***************************/

void DestroyGroupGen (void* _group) 
{
	free(_group);
}

/**************************DestroyIp***************************/

void DestroyIp (void* _ip) 
{
	free(_ip);
}

/**************************DestroyGroupName***************************/

void DestroyGroupName (void* _groupName) 
{
	free(_groupName);
}

/**************************printKey***************************/

static void printKey(void* _word)
{
	
	printf("Key: %s\n", ((char*)_word));
}

/**************************printValue***************************/

static void printValue(void* _value)
{
	
}

void PullKey2(void* _data, char _str[])
{

strcat(_str, (char*)_data);
strcat(_str, "\n");
}



void giveGroups(GrupsMng* _groups, char _str[])
{

	PutKeysInStr(_groups -> m_groupDatabase, _str , PullKey2);

}



