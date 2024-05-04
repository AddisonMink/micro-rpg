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

#define STATIC_LIST_TYPE(NAME, TYPE, CAPACITY) \
    typedef struct NAME                        \
    {                                          \
        int count;                             \
        int capacity;                          \
        TYPE data[CAPACITY];                   \
    } NAME

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

#define LIST_APPEND(LIST, VALUE)               \
    if ((LIST)->count < (LIST)->capacity)      \
    {                                          \
        (LIST)->data[(LIST)->count++] = VALUE; \
    }                                          \
    else                                       \
    {                                          \
        TraceLog(LOG_INFO, "LIST IS FULL");    \
    }

#define LIST_CONCAT(LIST1, LIST2)          \
    for (int i = 0; i < LIST2->count; i++) \
    LIST_APPEND(LIST1, LIST2->data[i])

#define LIST_DELETE(LIST, INDEX)              \
    for (int i = INDEX; i < LIST->count; i++) \
        LIST->data[i] = LIST->data[i + 1];    \
    LIST->count--

#define LIST_ITERATE_DEPRECATED(LIST) \
    for (int i = 0; i < LIST->count; i++)

#define LIST_ITERATE2_DEPRECATED(LIST) \
    for (int j = 0; j < LIST->count; j++)

#define LIST_FOREACH_PTR_DEPRECATED(LIST, TYPE, BLOCK) \
    {                                                  \
        for (int i = 0; i < LIST->count; i++)          \
        {                                              \
            TYPE *elem = LIST->data[i];                \
            do                                         \
            {                                          \
                BLOCK                                  \
            } while (0);                               \
        }                                              \
    }

#define LIST_FIND_PTR_DEPRECATED(LIST, TYPE, BLOCK) \
    ({                                              \
        TYPE *LIST_FIND_RESULT = NULL;              \
        bool success = false;                       \
        for (int i = 0; i < (LIST)->count; i++)     \
        {                                           \
            TYPE *elem = (LIST)->data[i];           \
            do                                      \
            {                                       \
                BLOCK                               \
            } while (0);                            \
            if (success)                            \
            {                                       \
                LIST_FIND_RESULT = elem;            \
                break;                              \
            }                                       \
        }                                           \
        LIST_FIND_RESULT;                           \
    })

#define LIST_EXISTS_PTR_DEPRECATED(LIST, TYPE, BLOCK) \
    (LIST_FIND_PTR_DEPRECATED(LIST, TYPE, BLOCK) != NULL)

#define LIST_ELEM_DEPRECATED(LIST, INDEX) LIST->data[INDEX]

#define LIST_EMPTY_DEPRECATED(LIST) (LIST->count <= 0)

#define LIST_FOREACH(LIST, TYPE, ELEM)                                                   \
    for (int _index = 0, _done = 0; !_done; _done = (++_index == (LIST)->count) ? 1 : 0) \
        for (TYPE *ELEM = &(LIST)->data[_index]; !_done; _done = 1)

#define LIST_FOREACH_IF(LIST, TYPE, ELEM, COND)                                          \
    for (int _index = 0, _done = 0; !_done; _done = (++_index == (LIST)->count) ? 1 : 0) \
        for (TYPE *ELEM = &(LIST)->data[_index]; !_done && (COND); _done = 1)

#define LIST_FIND(LIST, TYPE, ELEM, COND)                                                    \
    ({                                                                                       \
        TYPE *LIST_FIND_RESULT = NULL;                                                       \
        for (int _index = 0, _done = 0; !_done; _done = (++_index == (LIST)->count) ? 1 : 0) \
            for (TYPE *ELEM = &(LIST)->data[_index]; !_done; _done = 1)                      \
                if (COND)                                                                    \
                {                                                                            \
                    LIST_FIND_RESULT = ELEM;                                                 \
                    break;                                                                   \
                }                                                                            \
        LIST_FIND_RESULT;                                                                    \
    })

#define LIST_ANY(LIST, TYPE, ELEM, COND) \
    (LIST_FIND(LIST, TYPE, ELEM, COND) != NULL)
