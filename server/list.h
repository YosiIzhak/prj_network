#ifndef __LIST_H__
#define __LIST_H__
#include <stddef.h>  /*size_t*/

/************************Error messages****************************/

typedef enum List_Result{
LIST_SUCCESS,
LIST_UNINITIALIZED_ERROR,			
LIST_ALLOCATION_ERROR,				
LIST_NULL_ELEMENT_ERROR,
LIST_UNDERFLOW_ERROR
} ListResult;

/********************rename to struct Node************************/

typedef struct Node Node;

/********************rename to struct List************************/

typedef struct List List;


/*Description:
Create generic List in which each member data is void* and can point to any type.

Output:
NULL - If the memory allocations fails.
list - Pointer to the List structure.*/
List* ListCreate(void);

/*Description:
Destroys the list completely. optionally destroys the nodes data using user provided function. 

Input:
**_list - Pointer to pointer to the List structure. get &(*list).
_elementDestroy -  pointer to function that destroys all the nodes data. NULL if no such destroy is required*/
void ListDestroy(List** _list, void (*_elementDestroy)(void* _item));

/*Description:
insert new node to the head of the list. get data as void* or pointer of another type.

Input:
*_list - Pointer to the List structure.
*_item - new data (void*) that will enter in new node to the head of the list.

Output:
LIST_UNINITIALIZED_ERROR -  if _list == NULL || _item == NULL.
LIST_ALLOCATION_ERROR - If the memory allocations fails.
LIST_SUCCESS - if the new node enter the head of the list Successfully.*/
ListResult ListPushHead(List* _list, void* _item);

/*Description:
insert new node to the end of the list. get data as void* or pointer of another type.

Input:
*_list - Pointer to the List structure.
*_item - new data (void*) that will enter in new node to the end of the list.

Output:
LIST_UNINITIALIZED_ERROR -  if _list == NULL || _item == NULL.
LIST_ALLOCATION_ERROR - If the memory allocations fails.
LIST_SUCCESS - if the new node enter the end of the list Successfully.*/
ListResult ListPushTail(List* _list, void* _item);

/*Description:
Deletes the first node in the List. Stores the data of the deleted node in "_item" [&(void*)].

Input:
*_list - Pointer to the List structure.
**_item - pointer to pointer variable that will receive the deleted data. get &(*void).

Output:
LIST_UNINITIALIZED_ERROR -  if _list == NULL || _item == NULL.
LIST_UNDERFLOW_ERROR - If there are no nods in the list
LIST_SUCCESS - if the first node deleted successfully.*/
ListResult ListPopHead(List* _list, void** _item);

/*Description:
Deletes the last node in the List. Stores the data of the deleted node in "_item" [&(void*)].

Input:
*_list - Pointer to the List structure.
**_item - pointer to pointer variable that will receive the deleted data. get &(*void).

Output:
LIST_UNINITIALIZED_ERROR -  if _list == NULL || _item == NULL.
LIST_UNDERFLOW_ERROR - If there are no nods in the list
LIST_SUCCESS - if the last node deleted successfully.*/
ListResult ListPopTail(List* _list, void** _pItem);

/*Description:
Returns the number of elements in the List. o(n).

Input:
*_list - Pointer to the List structure. 

Output:
0 - if _list == NULL.
count - the number of nodes in the List.*/
size_t ListSize(const List* _list);

#endif /* __LIST_H__ */

