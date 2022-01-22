#include <stdio.h>
#include <stdlib.h>
#include "hash.map.h"
#include "gen.list.h"
#include "list.h"
#define IDENTICAL_KEYS 1
#define DIFFERENT_KEYS 0

/*************************structHash****************************/

struct HashMap
{
List** m_lists;                
HashFunction m_HashFunction;
EqualityFunction m_EqualityFunction;
size_t m_capacity;
size_t m_numOfItem;
};

/*************************structHash****************************/

struct Element
{
void* m_key;
void* m_value;
};

/**********************FunctionSignatures*************************/

static size_t CalculateCapacity(size_t _capacity);
static void* CreateElement(const void* _key, const void* _value);
static int CheckDuplicates (HashMap* _map, List* list, const void* _key);
static void* FindNodeByKey(HashMap* _map, List* list, const void* _searchKey);
static void DestroyList(List* list, keyDestroy _keyDestroy, valDestroy _valDestroy);
static void PrintList(List* list, PrintKey _PrintKey, PrintValue _PrintValue);
static void PullKeysFromList(List* list, char _str[], PullKey _PullKey);

/*************************HashMapCreate****************************/

HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
HashMap* hashMap;
size_t capacity;

	if(_capacity == 0 || _hashFunc == NULL || _keysEqualFunc == NULL)
	{
		return NULL;
	}
		
	if((hashMap = (HashMap*) malloc(sizeof(HashMap))) == NULL)
	{
        	return NULL;
   	}
   	
   	capacity = CalculateCapacity(_capacity); 
   	 		
	if ((hashMap -> m_lists = ((List**) calloc(capacity , sizeof(List*)))) == NULL)
	{
		free(hashMap);
		return NULL;
	}

	hashMap -> m_HashFunction = _hashFunc;
	hashMap -> m_EqualityFunction = _keysEqualFunc;
	hashMap -> m_capacity = capacity;
	hashMap -> m_numOfItem = 0;

return hashMap;
}

static size_t CalculateCapacity(size_t _capacity)
{
int index;
int prime;
int flag;

	prime = _capacity;
	while(1)
	{
		flag = 0;
		
		for (index = 2; index < prime; index++)
		{
			if(prime % index == 0)
			{
				flag = 1;
				index = prime;
			}
 		}
 		
 		if(flag == 0){break;}
 		
 		prime++;
	}
		
return prime;
}

/*************************HashMapDestroy****************************/

void HashMapDestroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
size_t i;

	if(_map == NULL || *_map == NULL){return;}
	
	for(i = 0; i < (*_map) -> m_capacity; i++)
	{
		if((*_map) -> m_lists[i] != NULL)
		{
			DestroyList((*_map) -> m_lists[i], _keyDestroy, _valDestroy);		
		}
	}
	
	free((*_map) -> m_lists);
	free(*_map);
	*_map = NULL;
}

static void DestroyList(List* list, keyDestroy _keyDestroy, valDestroy _valDestroy)
{
void* data;
Node* node;
Node* nodeEnd;

	
	if( _keyDestroy == NULL && _valDestroy == NULL)
	{
		ListDestroy(&list, NULL);
	}
	
	node = ListItrBegin(list);
	nodeEnd = ListItrEnd(list);
			
	while(node != nodeEnd) 
	{
		data = ListItrGet(node);
		
		if(_keyDestroy != NULL)
		{
			 _keyDestroy(((Element*)data) -> m_key);
		}
		if(_valDestroy != NULL)
		{

			_valDestroy(((Element*)data) -> m_value);
		}
		
		free(data);
		
		node = ListItrNext(node);	 
	}
	
ListDestroy(&list, NULL); 
}

/*************************HashMapInsert****************************/

MapResult HashMapInsert(HashMap* _map, const void* _key, const void* _value)
{
size_t index;
List* list;
void* element;

	if(_map == NULL){return MAP_UNINITIALIZED_ERROR;}
	if(_key == NULL){return MAP_KEY_NULL_ERROR;}
	
	index = _map -> m_HashFunction((void*)_key)%_map -> m_capacity;
	
	if(_map -> m_lists[index] == NULL)
	{
		if((list = ListCreate()) == NULL)
		{
			return MAP_LIST_ALLOCATION_ERROR;
		}
		if((element = CreateElement(_key, _value)) == NULL)
		{
			return MAP_ELEMENT_ALLOCATION_ERROR;
		}
		if(ListPushHead(list, element) != LIST_SUCCESS)
		{
			return MAP_LIST_INSERT_ERROR;
		}
		
		_map -> m_lists[index] = list;
		_map -> m_numOfItem++;
		return MAP_SUCCESS;
	}
	
	if(CheckDuplicates (_map, _map -> m_lists[index] , _key) == IDENTICAL_KEYS)
	{
		return MAP_KEY_DUPLICATE_ERROR;
	}
	if((element = CreateElement(_key, _value)) == NULL)
	{
		return MAP_ELEMENT_ALLOCATION_ERROR;
	}
	if(ListPushHead( _map -> m_lists[index], element) != LIST_SUCCESS)
	{
		return MAP_LIST_INSERT_ERROR;
	}
	_map -> m_numOfItem++;
				
return MAP_SUCCESS;
}

static void* CreateElement(const void* _key, const void* _value)
{
void* element;

	if((element = (Element*) malloc(sizeof(Element))) == NULL)
	{
        	return NULL;
   	}
	
	((Element*)element) -> m_key = (void*) _key;
	((Element*)element) -> m_value = (void*) _value;

return element;
}

static int CheckDuplicates (HashMap* _map, List* list, const void* _key)
{
void* data;
Node* node;
Node* nodeEnd;

	node = ListItrBegin(list);
	nodeEnd = ListItrEnd(list);
			
	while(node != nodeEnd) 
	{
		data = ListItrGet(node);
		if(_map -> m_EqualityFunction(((Element*)data) -> m_key, (void*) _key) == 1)
		{
			return IDENTICAL_KEYS;
		}
		node = ListItrNext(node);	 
	} 
return DIFFERENT_KEYS;
}

/*************************HashMapRemove****************************/

MapResult HashMapRemove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
size_t index;
Node* node;
void* element;

	if(_map == NULL){return MAP_UNINITIALIZED_ERROR;}
	if(_searchKey == NULL){return MAP_KEY_NULL_ERROR;}
	if(_pKey == NULL || _pValue == NULL){return MAP_NULL_INPUT_ERROR;}

	index = _map -> m_HashFunction((void*)_searchKey)%_map -> m_capacity;
	
	if(_map -> m_lists[index] == NULL){return MAP_KEY_NOT_FOUND_ERROR;}
		
	node = FindNodeByKey(_map, _map -> m_lists[index] , _searchKey);
	if(node == NULL){return MAP_KEY_NOT_FOUND_ERROR;}
		
	element =  ListItrGet(node);
	*_pKey = ((Element*)element) -> m_key;
	*_pValue = ((Element*)element) -> m_value;
	
	free(element);	
	ListItrRemove(node);
	_map -> m_numOfItem--;
	
return MAP_SUCCESS;
}

static void* FindNodeByKey(HashMap* _map, List* list, const void* _searchKey)
{
void* data;
Node* node;
Node* nodeEnd;

	node = ListItrBegin(list);
	nodeEnd = ListItrEnd(list);
		
	while(node != nodeEnd) 
	{
		data = ListItrGet(node);
		if(_map -> m_EqualityFunction(((Element*)data) -> m_key, (void*) _searchKey) == 1)
		{
			return node;
		}
		node = ListItrNext(node);	 
	} 
return NULL;
}

/*************************HashMapFind****************************/

MapResult HashMapFind(const HashMap* _map, const void* _key, void** _pValue)
{
void* element;
size_t index;
Node* node;

	if(_map == NULL){return MAP_UNINITIALIZED_ERROR;}
	if(_key == NULL){return MAP_KEY_NULL_ERROR;}
	if(_pValue == NULL){return MAP_NULL_INPUT_ERROR;}
	
	index = _map -> m_HashFunction((void*)_key)%_map -> m_capacity;
	
	if(_map -> m_lists[index] == NULL){return MAP_KEY_NOT_FOUND_ERROR;}
	
	node = FindNodeByKey((void*) _map, _map -> m_lists[index] , _key);
	if(node == NULL){return MAP_KEY_NOT_FOUND_ERROR;}
	
	element =  ListItrGet(node);
	*_pValue = ((Element*)element) -> m_value;

return MAP_SUCCESS;
}

/*************************HashMapSize****************************/

size_t HashMapSize(const HashMap* _map)
{
	if(_map == NULL){return MAP_UNINITIALIZED_ERROR;}
	
return _map -> m_numOfItem;
}



/*************************Test function****************************/

int GetCapasity( HashMap* _map)
{
	return _map -> m_capacity;
}

int FindIndex(HashMap* _map, void* _key)
{
return _map -> m_HashFunction((void*)_key)%_map -> m_capacity;
}

void* GetKey(HashMap* _map, int index, void* _Key)
{
Node* node;
void* data; 
 node = FindNodeByKey(_map, _map -> m_lists[index],  _Key);
 if(node == NULL)
 {
 	return NULL;
 }
 data = ListItrGet(node);
 return ((Element*)data) -> m_key;
} 


void HashMapPrint(HashMap* _map, PrintKey _PrintKey, PrintValue _PrintValue)
{
size_t i;

	if(_map == NULL || _PrintKey == NULL){return;}
		
	for(i = 0; i < _map -> m_capacity; i++)
	{
		if(_map-> m_lists[i] != NULL)
		{
			PrintList(_map -> m_lists[i], _PrintKey, _PrintValue);		
		}
	}
}


static void PrintList(List* list, PrintKey _PrintKey, PrintValue _PrintValue)
{
void* data;
Node* node;
Node* nodeEnd;

	node = ListItrBegin(list);
	nodeEnd = ListItrEnd(list);
		
	while(node != nodeEnd) 
	{
		data = ListItrGet(node);

		_PrintKey(((Element*)data) -> m_key);
		if(_PrintValue != NULL)
		{
			_PrintValue(((Element*)data) -> m_value);
		}
		
		node = ListItrNext(node);	 
	} 
}


void PutKeysInStr(HashMap* _map, char _str[], PullKey _PullKey)
{
size_t i;

		
	for(i = 0; i < _map -> m_capacity; i++)
	{
		if(_map-> m_lists[i] != NULL)
		{
			PullKeysFromList(_map -> m_lists[i], _str,  _PullKey);		
		}
	}
}


static void PullKeysFromList(List* list, char _str[], PullKey _PullKey)
{
void* data;
Node* node;
Node* nodeEnd;

	node = ListItrBegin(list);
	nodeEnd = ListItrEnd(list);
		
	while(node != nodeEnd) 
	{
		data = ListItrGet(node);

		_PullKey(((Element*)data) -> m_key, _str);

		node = ListItrNext(node);	 
	} 
}




