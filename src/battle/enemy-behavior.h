#pragma once

#include "battle/action.h"
#include "battle/combatant.h"

typedef struct EnemyBehavior
{
    const Action *action;
    Id targetOpt;
} EnemyBehavior;

EnemyBehavior EnemyBehavior_Get(const Combatant combatants[MAX_COMBATANTS], Id enemyId);