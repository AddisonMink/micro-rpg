#pragma once

#include "stddef.h"

// List Interface
/*
typedef struct List
{
    int count;
    int capacity;
    void *data[];
} List;
*/

// STATIC_LIST_TYPE
/* Defines a static list type.
 * Parameters:
 *  NAME - The name of the list type.
 *  TYPE - The type of the elements in the list.
 *  CAPACITY - The maximum number of elements the list can hold.
 *
 * Example:
 * STATIC_LIST_TYPE(int_list, int, 10);
 */
#define STATIC_LIST_TYPE(NAME, TYPE, CAPACITY) \
    typedef struct NAME                        \
    {                                          \
        int count;                             \
        int capacity;                          \
        TYPE data[CAPACITY];                   \
    } NAME

// LIST_INIT
/* Creates an empty list.
 * Parameters:
 *  TYPE - The type of the elements in the list.
 *  CAPACITY - The maximum number of elements the list can hold.
 * Returns:
 *  An empty list.
 *
 * Example:
 * List list = LIST_INIT(10);
 */
#define LIST_INIT(TYPE, CAPACITY) \
    ((TYPE){.count = 0, .capacity = CAPACITY})

// LIST_PUSH
/* Appends an element to a list.
 * Parameters:
 *  LIST - A reference to the list.
 *  VALUE - The value to append.
 *
 * Example:
 * LIST_PUSH(&list, 42);
 */
#define LIST_PUSH(LIST, VALUE)             \
    do                                     \
    {                                      \
        LIST->data[LIST->count++] = VALUE; \
    } while (0)

// LIST_CONCAT
/* Concatenates two lists.
 * Parameters:
 *  LIST - A reference to the list.
 *  OTHER - The list to concatenate.
 *
 * Example:
 * LIST_CONCAT(list, other);
 */
#define LIST_CONCAT(LIST, OTHER)          \
    for (int i = 0; i < OTHER->count; i++) \
    {                                     \
        LIST_PUSH(LIST, OTHER->data[i]);   \
    }

// LIST_DELETE
/* Deletes an element from a list.
 * Parameters:
 *  LIST - A reference to the list.
 *  INDEX - The index of the element to delete.
 *
 * Example:
 * LIST_DELETE(&list, 0);
 */
#define LIST_DELETE(LIST, INDEX)                      \
    do                                                \
    {                                                 \
        for (int i = INDEX; i < LIST->count - 1; i++) \
        {                                             \
            LIST->data[i] = LIST->data[i + 1];        \
        }                                             \
        LIST->count--;                                \
    } while (0)

// LIST_ITER
/* Iterates over a list.
 * Parameters:
 *  LIST - A reference to the list.
 *  TYPE - The type of the elements in the list.
 *  BLOCK - The code block to execute for each element.
 * Local Variables:
 *  _index - The index of the current element.
 *  _elem - A reference to the current element.
 *
 * Example:
 * LIST_ITER(&list, int, {
 *    printf("%d\n", *_elem);
 * });
 */
#define LIST_ITER(LIST, TYPE, BLOCK)                     \
    for (int _index = 0; _index < LIST->count; _index++) \
    {                                                    \
        TYPE *_elem = (TYPE *)&LIST->data[_index];       \
        BLOCK                                            \
    }

// LIST_ELEM
/* Gets an element from a list.
 * Parameters:
 *  LIST - A reference to the list.
 *  INDEX - The index of the element to get.
 * Returns:
 *  The element at the specified index.
 *
 * Example:
 * int elem = LIST_ELEM(list, 0);
 */
#define LIST_ELEM(LIST, INDEX) \
    LIST->data[INDEX]

// LIST_FIND
/* Finds the first element in a list that satisfies a predicate.
 * Parameters:
 *  LIST - A reference to the list.
 *  TYPE - The type of the elements in the list.
 *  PREDICATE - The predicate to test each element against.
 * Local Variables:
 *  _result - A reference to the first element that satisfies the predicate.
 * Returns:
 *  A reference to the first element that satisfies the predicate, or NULL if no element satisfies the predicate.
 *
 * Example:
 * int *result = LIST_FIND(&list, int, *_elem > 42);
 */
#define LIST_FIND(LIST, TYPE, PREDICATE) \
    ({                                   \
        TYPE *_result = NULL;            \
        LIST_ITER(LIST, TYPE, {          \
            if (PREDICATE)               \
            {                            \
                _result = _elem;         \
                break;                   \
            }                            \
        })                               \
        _result;                         \
    })

// LIST_FIND_INDEX
/* Finds the index of the first element in a list that satisfies a predicate.
 * Parameters:
 *  LIST - A reference to the list.
 *  TYPE - The type of the elements in the list.
 *  PREDICATE - The predicate to test each element against.
 * Local Variables:
 *  _result - The index of the first element that satisfies the predicate.
 *  _index - The index of the current element.
 *  _elem - A reference to the current element.
 * Returns:
 *  The index of the first element that satisfies the predicate, or -1 if no element satisfies the predicate.
 *
 * Example:
 * int index = LIST_FIND_INDEX(&list, int, *_elem == 42);
 */
#define LIST_FIND_INDEX(LIST, TYPE, PREDICATE) \
    ({                                         \
        int _result = -1;                      \
        LIST_ITER(LIST, TYPE, {                \
            if (PREDICATE)                     \
            {                                  \
                _result = _index;              \
                break;                         \
            }                                  \
        })                                     \
        _result;                               \
    })

#define LIST_ANY(LIST, TYPE, PREDICATE) \
    LIST_FIND(LIST, TYPE, PREDICATE) != NULL

#define LIST_NONE(LIST, TYPE, PREDICATE) \
    LIST_FIND(LIST, TYPE, PREDICATE) == NULL

#define LIST_ALL(LIST, TYPE, PREDICATE) \
    LIST_FIND(LIST, TYPE, !(PREDICATE)) == NULL

#define LIST_EMPTY(LIST) \
    ((LIST)->count == 0)