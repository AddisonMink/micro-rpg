#pragma once

#define LIST_INIT(CAPACITY)  \
    {                        \
        .count = 0,          \
        .capacity = CAPACITY \
    }

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

#define LIST_ITERATE(LIST) \
    for (int i = 0; i < LIST->count; i++)

#define LIST_ITERATE2(LIST) \
    for (int j = 0; j < LIST->count; j++)

#define LIST_ELEM(LIST, INDEX) LIST->data[INDEX]