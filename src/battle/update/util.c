#include "util.h"

static const bool inRange[NUM_RANGE_TYPES][NUM_ROWS][NUM_ROWS] = {
    [RANGE_SELF] = {
        [ROW_FRONT] = {[ROW_FRONT] = false, [ROW_BACK] = false},
        [ROW_BACK] = {[ROW_FRONT] = false, [ROW_BACK] = false},
    },
    [RANGE_MELEE] = {
        [ROW_FRONT] = {[ROW_FRONT] = true, [ROW_BACK] = false},
        [ROW_BACK] = {[ROW_FRONT] = false, [ROW_BACK] = false},
    },
    [RANGE_PROJECTILE] = {
        [ROW_FRONT] = {[ROW_FRONT] = false, [ROW_BACK] = true},
        [ROW_BACK] = {[ROW_FRONT] = true, [ROW_BACK] = true},
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
    case RANGE_PROJECTILE:
    {
        for (int i = 0; i < *effectCount; i++)
        {
            effects[i] = action->effects[i];
            effects[i].source = source;
            effects[i].target = targetOpt;
        }
        break;
    }
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

// Assumes that the active combatant is alive.
void BuildQueue(_Battle *battle, int *queueIndex)
{
    CombatantId activeId = battle->queue[(*queueIndex)];

    battle->queueCount = 0;
    for (CombatantId id = 0; id < NUM_COMBATANTS; id++)
    {
        if (battle->combatants[id].state == COMBATANT_STATE_ALIVE)
        {
            battle->queue[battle->queueCount] = id;
            if (id == activeId)
            {
                *queueIndex = battle->queueCount;
            }
            battle->queueCount++;
        }
    }
}

static void resetRowSide(_Battle *battle, int start, int stop)
{
    bool allInBack = true;
    for (int i = start; i < stop; i++)
    {
        const bool alive = battle->combatants[i].state == COMBATANT_STATE_ALIVE;
        const bool inFront = battle->combatants[i].row == ROW_FRONT;
        if (alive && inFront)
        {
            allInBack = false;
            break;
        }
    }

    if (allInBack)
    {
        for (int i = start; i < stop; i++)
        {
            battle->combatants[i].row = ROW_FRONT;
        }
    }
}

void ResetRows(_Battle *battle)
{
    resetRowSide(battle, 0, FIRST_ENEMY_ID);
    resetRowSide(battle, FIRST_ENEMY_ID, NUM_COMBATANTS);
}