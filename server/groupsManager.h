#ifndef __GROUPS_MANAGER_H__
#define __GROUPS_MANAGER_H__
#include "protocol.h"

typedef struct GrupsMng GrupsMng;

typedef enum GrupsMngResult
{
MANAGER_NOT_INITIALIZED,
GROUP_NOT_INITIALIZED,
GROUP_MALLOC_FAIL,
HASH_MAP_INSERT_FAIL,
DUPLICATE_GROUP_NAME_FAIL,
GROUP_NOT_FOUND_IN_HASH,
GROUP_DELETE,
GROUP_SUCCESS,
NO_EXISTING_GROUPS_GM,
PRINT_EXISTING_GROUPS_SUCCESS_GM
}GrupsMngResult;



GrupsMng* CreateGroupsManager (int _capacity);

void DestroyGroupsManager (GrupsMng* _manager);

GrupsMngResult CreateGroup (char _groupName[] , GrupsMng* _manager, char _buffer[]);

GrupsMngResult JoinExistingGroup (char _groupName[] , GrupsMng* _manager, char _buffer[]);

GrupsMngResult LeaveGroup (char _groupName[] , GrupsMng* _manager, char _buffer[]);

GrupsMngResult PackExistingGroup (GrupsMng* _manager, char* _buffer, MessagesTypes _messagesTypes);

#endif
