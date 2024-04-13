#include "queue.h"

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

Id Queue_Next(Queue *queue)
{
    queue->index = (queue->index + 1) % queue->count;
    return Queue_Peek(queue);
}