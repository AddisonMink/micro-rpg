#pragma once

#include "action.h"
#include "combatant.h"

#define NUM_COMBATANTS 6
#define FIRST_ENEMY_ID 3

typedef struct _Battle
{
    Combatant combatants[NUM_COMBATANTS];
    CombatantId queue[NUM_COMBATANTS]; // This will eventually be used to implement initiative order.

    enum
    {
        BATTLE_SELECT_ACTION,
    } state;

    union
    {
        struct
        {
            int queueIndex;
            int actionIndex;
        } selectAction;
    } data;
} _Battle;