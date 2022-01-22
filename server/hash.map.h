#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__
#include <stddef.h>  /* size_t */

/********************returnMessages***********************/

typedef enum Map_Result {
	MAP_SUCCESS = 0,
	MAP_UNINITIALIZED_ERROR, 		
	MAP_KEY_NULL_ERROR,
	MAP_NULL_INPUT_ERROR, 			
	MAP_KEY_DUPLICATE_ERROR, 		
	MAP_KEY_NOT_FOUND_ERROR, 		
	MAP_ALLOCATION_ERROR, 
	MAP_LIST_ALLOCATION_ERROR,
	MAP_ELEMENT_ALLOCATION_ERROR,
	MAP_LIST_INSERT_ERROR			
} MapResult;

/********************rename structHashMap***********************/

typedef struct HashMap HashMap;

/********************rename structElement***********************/

typedef struct Element Element;

/********************function typedef***********************/

typedef size_t (*HashFunction)(void* _key);
typedef int (*EqualityFunction)(void* _firstKey, void* _secondKey);
typedef void (*keyDestroy)(void* _key);
typedef void (*valDestroy)(void* _value);
typedef void (*PrintKey)(void*);
typedef void (*PrintValue)(void*);
typedef void (*PullKey)(void*, char _str[]);


/*Description:
Create generic HashMap in which each element is generic List* which contains pointers to structure with key and value that each of them itself is a void* pointer that can point to any type.

Input:
_capacity - The size of the HashMap.
_hashFunc - pointer to mathematical function that make hash on the key. (received from the user)
_keysEqualFunc - pointer to function that compares the key received from the user to the key in the HashMap. Returns 1 if they are the same and 0 if different. (received from the user)

Output:
NULL - if _capacity == 0 || _hashFunc == NULL || _keysEqualFunc == NULL or if one of the memory allocations failed.
hashMap - Pointer to the HashMap structure.*/
HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc);


/*Description:
Destroys all the Lists elements,  all the Lists, the HashMap arrays, and the HashMap structure.

Input:
**_map - Pointer to pointer to the HashMap structure. get &*hashMap.
_keyDestroy - pointer to function that destroys all the key elements. (if not necessary put NULL)
_valDestroy - pointer to function that destroys all the value elements. (if not necessary put NULL)*/
void HashMapDestroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value));


/*Description:
insert new key and value to the HashMap. each of them is a void* that can point to any type.

Input:
*_map - Pointer to the HashMap structure.
*_key - new key (pointer) that will enter the HashMap.
*_value - new value (pointer) that will enter the HashMap.

Output:
MAP_UNINITIALIZED_ERROR - if _map == NULL.
MAP_KEY_NULL_ERROR - if _key == NULL.
MAP_LIST_ALLOCATION_ERROR - If the list memory allocation failed.
MAP_ELEMENT_ALLOCATION_ERROR - If the memory allocation to the structure containing the key and value fails.
MAP_LIST_INSERT_ERROR - If the key and value insertion to the list failed.
MAP_SUCCESS - If the new key and value enters the HashMap successfully.
MAP_KEY_DUPLICATE_ERROR - If the key that the user wants to enter into the HashMap already exists in the HashMap. (No duplicate key in the HashMap)*/
MapResult HashMapInsert(HashMap* _map, const void* _key, const void* _value);


/*Description:
Deletes key and value frome the HashMap and put their data in _pKey and _pValue variables - get &(void*). Finds what to remove just using the key he gets _searchKey.

Input:
*_map - Pointer to the HashMap structure.
*_searchKey - The key we want to delete from the hash. It finds the value associated with this key and deletes it as well.
**_pKey - pointer to pointer variable that will receive the deleted key - get &(*void).
**_pValue - pointer to pointer variable that will receive the deleted value - get &(*void).

Output:
MAP_UNINITIALIZED_ERROR - if _map == NULL.
MAP_KEY_NULL_ERROR - if _searchKey == NULL.
MAP_NULL_INPUT_ERROR - if _pKey == NULL || _pValue == NULL.
MAP_KEY_NOT_FOUND_ERROR - If the key we want to delete is not found in the HashMap.
MAP_SUCCESS - if the key and value deleted successfully and their data enter to _pKey and _pValue.*/
MapResult HashMapRemove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue);


/*Description:
search if value exists through its key and returns it to the user in  _pValue variable.

Input:
*_map - Pointer to the HashMap structure.
*_key - The key by which the function searches the value.
**_pValue - pointer to pointer variable that will receive the value the function found - get &(*void).

Output:
MAP_UNINITIALIZED_ERROR - if _map == NULL.
MAP_KEY_NULL_ERROR -  if  _key == NULL.
MAP_NULL_INPUT_ERROR - if  _pValue == NULL.
MAP_KEY_NOT_FOUND_ERROR - If the key is not found in the HashMap.
MAP_SUCCESS - If the function found the value and returned its data to the user.*/
MapResult HashMapFind(const HashMap* _map, const void* _key, void** _pValue);


/*Description:
Returns the number of elements in the HashMap.

Input:
*_map - Pointer to the HashMap structure.

Output:
_map -> m_numOfItem -  the number of elements in the HashMap.*/
size_t HashMapSize(const HashMap* _map);


/*Description:
Prints all the keys and values (if any) that exist in the HashMap.

Input:
*_map - Pointer to the HashMap structure.
_PrintKey - Pointer to the function that prints the key (received from the user)
_PrintValue - Pointer to the function that prints the value (received from the user)*/
void HashMapPrint(HashMap* _map, PrintKey _PrintKey, PrintValue _PrintValue);


/****testFunction******/
void* GetKey(HashMap* _map, int index, void* _Key);


void PutKeysInStr(HashMap* _map, char _str[], PullKey _PullKey);

#endif /*#ifndef__HASH_MAP_H__*/
