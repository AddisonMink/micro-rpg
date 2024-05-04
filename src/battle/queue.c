#include "queue.h"

#include "common/list-macros.h"

Queue Queue_Create(const Combatant combatants[MAX_COMBATANTS])
{
    Queue queue = {
        .capacity = MAX_COMBATANTS,
        .count = 0,
        .index = 0,
    };

    for (int i = 0; i < MAX_COMBATANTS; i++)
    {
        if (combatants[i].state == COMBATANT_STATE_ALIVE)
        {
            queue.data[queue.count++] = combatants[i].id;
        }
    }

    return queue;
}

Id Queue_Peek(const Queue *queue)
{
    return queue->data[queue->index];
}

Id Queue_PeekNext(const Queue *queue)
{
    return queue->data[(queue->index + 1) % queue->count];
}

void Queue_Next(Queue *queue)
{
    queue->index = (queue->index + 1) % queue->count;
}

void Queue_Delete(Queue *queue, Id id)
{

    int index = -1;
    LIST_ITERATE_DEPRECATED(queue)
    {
        if (LIST_ELEM_DEPRECATED(queue, i) == id)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        LIST_DELETE(queue, index);
        if (queue->index > index)
        {
            queue->index--;
        }
    }
}