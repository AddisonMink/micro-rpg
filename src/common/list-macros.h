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

// Initialize

#define STATIC_LIST_TYPE(TYPE, CAPACITY) \
    typedef struct TYPE##List            \
    {                                    \
        int count;                       \
        int capacity;                    \
        TYPE data[CAPACITY];             \
    } TYPE##List

#define STATIC_LIST_TYPE_PTR(TYPE, CAPACITY) \
    typedef struct TYPE##List                \
    {                                        \
        int count;                           \
        int capacity;                        \
        TYPE *data[CAPACITY];                \
    } TYPE##List

#define LIST_INIT(CAPACITY)  \
    {                        \
        .count = 0,          \
        .capacity = CAPACITY \
    }

// List Mutators

#define LIST_APPEND(LIST, VALUE)           \
    if (LIST->count < LIST->capacity)      \
        LIST->data[LIST->count++] = VALUE; \
    else                                   \
        TraceLog(LOG_WARNING, "List is full!")

#define LIST_CONCAT(LIST1, LIST2)          \
    for (int i = 0; i < LIST2->count; i++) \
    LIST_APPEND(LIST1, LIST2->data[i])

#define LIST_DELETE(LIST, INDEX)              \
    for (int i = INDEX; i < LIST->count; i++) \
        LIST->data[i] = LIST->data[i + 1];    \
    LIST->count--

// List Iterators

#define LIST_ITERATE(LIST) \
    for (int i = 0; i < LIST->count; i++)

#define LIST_ITERATE2(LIST) \
    for (int j = 0; j < LIST->count; j++)

#define LIST_FOREACH(LIST, TYPE, BLOCK)       \
    {                                         \
        for (int i = 0; i < LIST->count; i++) \
        {                                     \
            const TYPE elem = LIST->data[i];  \
            do                                \
            {                                 \
                BLOCK                         \
            } while (0);                      \
        }                                     \
    }

#define LIST_FOREACH_PTR(LIST, TYPE, BLOCK)   \
    {                                         \
        for (int i = 0; i < LIST->count; i++) \
        {                                     \
            TYPE *elem = LIST->data[i];       \
            do                                \
            {                                 \
                BLOCK                         \
            } while (0);                      \
        }                                     \
    }

// List Queries

/* LIST_FIND
 * Apply a test to each element in the list and return a pointer to the first element that passes the test.
 * If a test is successful, set local variable "success" to true.
 *
 * LIST: List A
 * TYPE: typeof(A)
 * LOCAL VARIABLES:
 * - elem: A,           The current element in the list.
 * - success: bool,     The result of the test.
 * RETURN: A* | NULL,   The first element that passes the test.
 */
#define LIST_FIND(LIST, TYPE, BLOCK)            \
    ({                                          \
        TYPE *LIST_FIND_RESULT = NULL;          \
        bool success = false;                   \
        for (int i = 0; i < (LIST)->count; i++) \
        {                                       \
            TYPE *elem = &(LIST)->data[i];      \
            do                                  \
            {                                   \
                BLOCK                           \
            } while (0);                        \
            if (success)                        \
            {                                   \
                LIST_FIND_RESULT = elem;        \
                break;                          \
            }                                   \
        }                                       \
        LIST_FIND_RESULT;                       \
    })

#define LIST_FIND_PTR(LIST, TYPE, BLOCK)        \
    ({                                          \
        TYPE *LIST_FIND_RESULT = NULL;          \
        bool success = false;                   \
        for (int i = 0; i < (LIST)->count; i++) \
        {                                       \
            TYPE *elem = (LIST)->data[i];       \
            do                                  \
            {                                   \
                BLOCK                           \
            } while (0);                        \
            if (success)                        \
            {                                   \
                LIST_FIND_RESULT = elem;        \
                break;                          \
            }                                   \
        }                                       \
        LIST_FIND_RESULT;                       \
    })

/* LIST_EXISTS
 * Apply a test to each element in the list and return true if an an element satisfies the test.
 * If a test is successful, set local variable "success" to true.
 *
 * LIST: List A
 * TYPE: typeof(A)
 * LOCAL VARIABLES:
 * - elem: A,       The current element in the list.
 * - success: bool, The result of the test.
 */
#define LIST_EXISTS(LIST, TYPE, BLOCK) \
    (LIST_FIND(LIST, TYPE, BLOCK) != NULL)

#define LIST_EXISTS_PTR(LIST, TYPE, BLOCK) \
    (LIST_FIND_PTR(LIST, TYPE, BLOCK) != NULL)

#define LIST_ELEM(LIST, INDEX) LIST->data[INDEX]

#define LIST_EMPTY(LIST) (LIST->count <= 0)