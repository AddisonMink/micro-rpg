#pragma once

#include "combatant.h"

typedef const char *Name;

typedef int Index;

#define NUM_COMBATANTS 6
#define FIRST_ENEMY_ID 3

typedef struct _Battle
{
    Combatant combatants[NUM_COMBATANTS];
} _Battle;