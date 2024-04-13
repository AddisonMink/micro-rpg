#include "target.h"

#include "common/list-macros.h"

static const bool rangeTable[NUM_RANGES][NUM_ROWS][NUM_ROWS] = {
    [RANGE_MELEE] = {
        [ROW_FRONT] = {[ROW_FRONT] = true, [ROW_BACK] = false},
        [ROW_BACK] = {[ROW_FRONT] = false, [ROW_BACK] = false},
    },
    [RANGE_REACH] = {
        [ROW_FRONT] = {[ROW_FRONT] = true, [ROW_BACK] = true},
        [ROW_BACK] = {[ROW_FRONT] = true, [ROW_BACK] = false},
    },
    [RANGE_PROJECTILE] = {
        [ROW_FRONT] = {[ROW_FRONT] = false, [ROW_BACK] = false},
        [ROW_BACK] = {[ROW_FRONT] = true, [ROW_BACK] = true},
    },
};

TargetList TargetList_Create(const Action *action, const Combatant combatants[MAX_COMBATANTS], Id id)
{
    TargetList targets = LIST_INIT(MAX_COMBATANTS);

    if (action->range == RANGE_SELF)
    {
        LIST_APPEND((&targets), id);
        return targets;
    };

    const Id start = id < FIRST_ENEMY_ID ? FIRST_ENEMY_ID : 0;
    const Id end = start == FIRST_ENEMY_ID ? MAX_COMBATANTS : FIRST_ENEMY_ID;

    for (Id i = start; i < end; ++i)
    {
        // TraceLog(LOG_INFO, "TargetList_Create Candidate: %d", i);
        const Combatant *combatant = &combatants[i];
        if (combatant->state != COMBATANT_STATE_ALIVE)
            continue;

        if (rangeTable[action->range][combatants[id].row][combatant->row])
        {
            // TraceLog(LOG_INFO, "TargetList_Create: %d", i);
            LIST_APPEND((&targets), i);
        }
    }

    return targets;
}