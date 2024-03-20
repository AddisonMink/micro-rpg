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
        BATTLE_SELECT_TARGET,
        BATTLE_EXECUTE_ACTION,
    } state;

    union
    {
        struct
        {
            int queueIndex;
            int actionIndex;
        } selectAction;

        struct
        {
            int queueIndex;
            ActionType actionType;
            CombatantId targets[NUM_COMBATANTS];
            int targetCount;
            int targetIndex;
        } selectTarget;

        struct
        {
            int queueIndex;
            ActionType actionType;
            CombatantId targetIdOpt;
        } executeAction;

    } data;
} _Battle;