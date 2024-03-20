#include "update-util.h"

static const bool inRange[NUM_RANGE_TYPES][NUM_ROWS][NUM_ROWS] = {
    [RANGE_SELF] = {
        [ROW_FRONT] = {[ROW_FRONT] = false, [ROW_BACK] = false},
        [ROW_BACK] = {[ROW_FRONT] = false, [ROW_BACK] = false},
    },
    [RANGE_MELEE] = {
        [ROW_FRONT] = {[ROW_FRONT] = true, [ROW_BACK] = false},
        [ROW_BACK] = {[ROW_FRONT] = false, [ROW_BACK] = false},
    },
};

void ActionSetTarget(Effect effects[MAX_EFFECTS], int *effectCount, const Action *action, CombatantId source, CombatantId targetOpt)
{
    *effectCount = action->effectCount;

    switch (action->range)
    {
    case RANGE_SELF:
    {
        for (int i = 0; i < *effectCount; i++)
        {
            effects[i] = action->effects[i];
            effects[i].source = source;
            effects[i].target = source;
        }
        break;
    }
    case RANGE_MELEE:
        TraceLog(LOG_INFO, "ACTION-SET-TARGET: Melee range not implemented");
        break;
    }
}

void FindTargets(CombatantId targets[NUM_COMBATANTS], int *targetCount, const _Battle *battle, CombatantId source, Range range)
{
    const CombatantId start = source < FIRST_ENEMY_ID ? FIRST_ENEMY_ID : 0;
    const CombatantId stop = source < FIRST_ENEMY_ID ? NUM_COMBATANTS : FIRST_ENEMY_ID;
    const Row sourceRow = battle->combatants[source].row;
    *targetCount = 0;
    
    for (CombatantId id = start; id < stop; id++)
    {
        const Row targetRow = battle->combatants[id].row;
        if (battle->combatants[id].state == COMBATANT_STATE_ALIVE && inRange[range][sourceRow][targetRow])
        {
            targets[*targetCount] = id;
            (*targetCount)++;
        }
    }
}