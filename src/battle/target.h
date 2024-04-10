#pragma once

#include "battle/combatant.h"

typedef struct TargetList
{
    Id data[MAX_COMBATANTS];
    int capacity;
    int count;
    int index;
} TargetList;