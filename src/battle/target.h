#pragma once

#include "battle/action.h"
#include "battle/combatant.h"

typedef struct TargetList
{
    Id data[MAX_COMBATANTS];
    int capacity;
    int count;
    int index;
} TargetList;

TargetList TargetList_Create(const Action *action, const Combatant combatants[MAX_COMBATANTS], Id id);