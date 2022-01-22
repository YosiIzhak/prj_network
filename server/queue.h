#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <stdio.h>

/************************Error messages****************************/

typedef enum{
QUEUE_SUCCESS,
QUEUE_UNINITIALIZED_ERROR,
QUEUE_OVERFLOW_ERROR,
QUEUE_UNDERFLOW_ERROR,
QUEUE_DATA_NOTFOUND_ERROR,
QUEUE_THERE_ARE_DATA,
QUEUE_DATA_UNINITIALIZED_ERROR
}QueueResult;


/********************rename to struct Queue************************/

typedef struct Queue Queue;

/********************rename to function************************/

typedef void (*DestroyItem)(void* _element);
typedef int (*ActionFunction)(const void* _element, void* _context);

/*Description:
Create generic Queue in which each member is void* and can point to any type.

Input:
_size - Queue size (In units of void*).

Output:
NULL - if _size == 0 or if one of the memory allocations failed.
queue - pointer to the Queue structure.*/
Queue *QueueCreate(size_t _size);

/*Description:
Destroys all the Queue elements, the Queue array and the Queue structure.

Input:
**_queue - pointer to pointer to the Queue structure. get &(*queue)
DestroyItem - pointer to function that destroys all the Queue elements.*/
void QueueDestroy(Queue**_queue, void(*DestroyItem)(void* _element));

/*Description:
insert new element to the m_tail place in the Queue. get void* or pointer of another type.

Input:
*_queue - pointer to the Queue structure.
_item - new pointer that will enter the m_tail place in the Queue.

Output:
QUEUE_UNINITIALIZED_ERROR - if _queue == NUUL.
QUEUE_OVERFLOW_ERROR - If the number of elements in the Queue equal to the Queue size.
QUEUE_SUCCESS - If the new pointer enters the Queue successfully.*/
QueueResult QueueInsert(Queue *_queue, void* _item);

/*Description:
Deletes the element (void*) in the m_head place. get &(void*).

Input:
*_queue - pointer to the Queue structure.
**_item - pointer to pointer variable that will receive the deleted pointer.get &(*void).

Output:
QUEUE_UNINITIALIZED_ERROR - if _queue == NULL || _item == NULL.
QUEUE_UNDERFLOW_ERROR - If the number of elements (m_nItems) is 0.
QUEUE_SUCCESS - If the element in the m_head place was deleted successfully.*/
QueueResult QueueRemove(Queue *_queue, void**_item);

/*Description:
Checks if the Queue is empty or not.

Input:
*_queue - pointer to the Queue structure.

Output:
QUEUE_DATA_UNINITIALIZED_ERROR - If _queue == NULL or if Queue is empty. 
QUEUE_THERE_ARE_DATA - If Queue is not empty*/
int QueueIsEmpty(Queue *_queue);

/*Description:
generic function used for all actions that need to be done on all the Queue elements.

Input:
*_queue - Pointer to the Queue structure.
Action - Pointer to function that defines the action required to perform on all the Queue elements. Returns 0 to stop when no need to continue running on the other Queue elements.
*_context - External information (if required) to perform the action. (Eg value you want to check if exsist in the Queue)

Output:
i - The number of times the function ran. (On how many elements?)*/
size_t QueueForEach(Queue *_queue, ActionFunction Action , void* _context);

#endif /*#ifndef__QUEUE_H__*/
