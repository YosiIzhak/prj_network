#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <stdio.h>


/*Error messages*/

typedef enum VectorResult {
VECTOR_SUCCESS,
VECTOR_UNITIALIZED_ERROR, 
VECTOR_ALLOCTION_ERROR,
VECTOR_INDEX_OUT_OF_BOUNDS_ERROR,
VECTOR_OVERFLOW_ERROR
}VectorResult;

/*Rename to struct Vector*/

typedef struct Vector Vector;


/*Description:
Create generic vector in which each member is void*  and can point to any type.

Input:
_initialSize - Vector size (in void* units).
_extensionBlockSize - realloc size in case the place in the vector runs out(in void* units).
 
Output:
NULL - if _initialSize == 0 && _extensionBlockSize == 0 or If one of the memory allocations fails.
vector - Pointer to the vector structure.*/
Vector *VectorCreate (size_t _initialSize, size_t _extensionBlockSize);

/*Description:
Destroys all the vector elements, the vector and the  vector structure.
Input:
**_vector - Pointer to pointer to the vector structure. get &(*vector).
*_elementDestroy - pointer to function that destroys all the Vector elements.*/
void VectorDestroy (Vector** _vector, void(*_elementDestroy)(void* _item));

/*Description:
insert new element to the last place in the vector. get void* or pointer of another type.

Input:
*_vector - Pointer to the vector structure.
*_item - new pointer that will enter the last place in Vector.

Output:
VECTOR_UNITIALIZED_ERROR - if _vector == NULL || _item == NULL or if the realloc fail.
VECTOR_OVERFLOW_ERROR - If there is no place in the vector && _extensionBlockSize == 0.
VECTOR_SUCCESS - If the new pointer enters the vector successfully.*/
VectorResult VectorAppend (Vector* _vector,void* _item);

/*Description:
Deletes the last element (void*) in the Vector. get &(void*).

Input:
*_vector - Pointer to the vector structure.
**_item - pointer to pointer variable that will receive the deleted pointer.get &(*void).

Output:
VECTOR_UNITIALIZED_ERROR -  if _vector == NULL || _item == NULL.
VECTOR_OVERFLOW_ERROR - If the number of elements in the vector is 0.
VECTOR_SUCCESS - If the last element was deleted successfully.*/
VectorResult VectorRemove(Vector* _vector, void** _item);

/*Description:
Gives the element in the index place and Stores it in a variable void*. get &void*. (Does not delete the element from the vector)

Input:
*_vector - Pointer to the vector structure.
_index - index place.
**_item - void* variable that will get the pointer from the index place.

Output:
VECTOR_UNITIALIZED_ERROR - if _vector == NULL || _item == NULL.
VECTOR_INDEX_OUT_OF_BOUNDS_ERROR - If the index is greater than the number of elements in the vector.
VECTOR_SUCCESS - If the pointer in index place successfully stored in the _item variable.*/
VectorResult VectorGet(const Vector* _vector, size_t _index, void** _item);

/*Description:
insert pointer to the index place. get void* or pointer of another type with casting.

Input:
*_vector -  Pointer to the vector structure.
_index - index place.
*_item - The pointer will enter the index place.

Output:
VECTOR_UNITIALIZED_ERROR - if _vector == NULL || _item == NULL.
VECTOR_INDEX_OUT_OF_BOUNDS_ERROR - If the index is greater than the number of elements in the vector.
VECTOR_SUCCESS - If the pointer successfully enters the index place*/
VectorResult VectorSet(Vector* _vector, size_t _index, void* _item);

/*Description:
Returns the number of elements in the vector.

Input:
*_vector - Pointer to the vector structure.

Output:
0 - if _vector == NULL.
_vector -> m_nItems - the number of elements in the vector.*/
size_t  VectorSize (const Vector* _vector);

/*Description:
Returns the size of the vector memory allocation (in void* units)

Input:
*_vector - Pointer to the vector structure.

Output:
0 - if _vector == NULL.
_vector -> m_nItems - the size of the vector memory allocation (in void* units)*/
size_t  VectorCapacity (const Vector* _vector);

/*Description:
generic function used for all actions that need to be done on all the Vector elements.

Input:
*_vector - Pointer to the vector structure.
*_vectorElementAction - Pointer to function that defines the action required to perform on all the Vector elements. Returns 0 to stop when no need to continue running on the other Vector elements.
*_context - External information (if required) to perform the action. (Eg value you want to check if exsist in the vector)

Output:
i - The index where the function vectorElementAction returned 0. or the index at the last place + 1 if the function ran on all the elements without stopping.*/
size_t VectorForEach(const Vector* _vector, int(*_vectorElementAction)(void*, size_t , void*), void* _context);



#endif /*#ifndef__VECTOR_H__*/




























