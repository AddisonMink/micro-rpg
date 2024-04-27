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

// LIST_ITER
/* Apply a block of code to each element in the list.
 *
 * LIST: Pointer to the list.
 * TYPE: Type of the elements in the list.
 * ADDR: & | EMPTY, depending on whether the elements should be taken by reference.
 * BLOCK: The block of code to apply to each element.
 * LOCAL VARIABLES:
 * - _elem: A, The current element in the list.
 * - _continue: bool, Whether to continue iterating.
 *
 * Example: Print the first 10 elements of an list of integers.
 * LIST_ITER(list, int, , {
 *    printf("%d\n", _elem);
 *    _continue = _elem < 10;
 * });
 */
#define LIST_ITER(LIST, TYPE, ADDR, BLOCK)                 \
    {                                                      \
        bool _continue = true;                             \
        for (int i = 0; _continue && i < LIST->count; i++) \
        {                                                  \
            TYPE _elem = ADDR##LIST->data[i];              \
            do                                             \
            {                                              \
                BLOCK                                      \
            } while (0);                                   \
        }                                                  \
    }

// LIST_ACC
/* Apply a block of code to each element in the list and accumulate the result.
 *
 * LIST: Pointer to the list.
 * TYPE: Type of the elements in the list.
 * ACC_TYPE: Type of the accumulator.
 * ACC: Initial value of the accumulator.
 * ADDR: & | EMPTY, depending on whether the elements should be taken by reference.
 * BLOCK: The block of code to apply to each element.
 * LOCAL VARIABLES:
 * - _elem: A, The current element in the list.
 * - _continue: bool, Whether to continue iterating.
 * - _acc: ACC_TYPE, The accumulator.
 *
 * Example: Sum the first 10 elements of an list of integers.
 * int sum = LIST_ACC(list, int, int, 0, , {
 *    _acc += _elem;
 *    _continue = _elem < 10;
 * });
 */
#define LIST_ACC(LIST, TYPE, ACC_TYPE, ACC, ADDR, BLOCK) \
    ({                                                   \
        ACC_TYPE _acc = ACC;                             \
        LIST_ITER(LIST, TYPE, ADDR, {BLOCK});            \
        _acc;                                            \
    })

// LIST_FIND
/* Apply a block of code to each element in the list and return the first element that satisfies the condition.
 *
 * LIST: Pointer to the list.
 * TYPE: Type of the elements in the list.
 * ADDR: & | EMPTY, depending on whether the elements should be taken by reference.
 * BLOCK: The block of code to apply to each element.
 * LOCAL VARIABLES:
 * - _elem: A, The current element in the list.
 * - _continue: bool, Whether to continue iterating.
 * - _acc: A*, The accumulator.
 * - _success: bool, Whether an element satisfies the condition.
 *
 * Example: Find the first element in a list of integers that is greater than 10.
 * int *result = LIST_FIND(list, int, , {
 *    _success = _elem > 10;
 * });
 */
#define LIST_FIND(LIST, TYPE, ADDR, BLOCK)         \
    ({                                             \
        int _success = 0;                          \
        LIST_ACC(LIST, TYPE, TYPE *, NULL, ADDR, { \
            BLOCK                                  \
            if (_success)                          \
            {                                      \
                _continue = 0;                     \
                _acc = &_elem;                     \
            }                                      \
        });                                        \
    })

// LIST_CONTAINS
/* Apply a block of code to each element in the list and return true if an element satisfies the condition.
 *
 * LIST: Pointer to the list.
 * TYPE: Type of the elements in the list.
 * ADDR: & | EMPTY, depending on whether the elements should be taken by reference.
 * BLOCK: The block of code to apply to each element.
 * LOCAL VARIABLES:
 * - _elem: A, The current element in the list.
 * - _continue: bool, Whether to continue iterating.
 * - _acc: A*, The accumulator.
 * - _success: bool, Whether an element satisfies the condition.
 *
 * Example: Check if a list of integers contains an element greater than 10.
 * bool contains = LIST_CONTAINS(list, int, , {
 *    _success = _elem > 10;
 * });
 */
#define LIST_CONTAINS(LIST, TYPE, ADDR, BLOCK) \
    (LIST_FIND(LIST, TYPE, ADDR, BLOCK) != NULL)

#define LIST_ITERATE(LIST) \
    for (int i = 0; i < LIST->count; i++)

#define LIST_ITERATE2(LIST) \
    for (int j = 0; j < LIST->count; j++)

#define LIST_ELEM(LIST, INDEX) LIST->data[INDEX]

#define LIST_EMPTY(LIST) (LIST->count <= 0)
