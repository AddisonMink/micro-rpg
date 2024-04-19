#pragma once

#include "battle/combatant.h"

typedef struct Queue
{
    Id data[MAX_COMBATANTS];
    int capacity;
    int count;
    int index;
} Queue;

Queue Queue_Create(const Combatant combatants[MAX_COMBATANTS]);

Id Queue_Peek(const Queue *queue);

Id Queue_PeekNext(const Queue *queue);

void Queue_Next(Queue *queue);

void Queue_Delete(Queue *queue, Id id);