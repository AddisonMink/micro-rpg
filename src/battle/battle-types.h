#pragma once

#include "action.h"
#include "combatant.h"

#define NUM_COMBATANTS 6
#define FIRST_ENEMY_ID 3
#define MAX_EVENTS 2

typedef struct Event
{
    float duration;
    float elapsed;

    enum
    {
        EVENT_ENEMY_TINT,
    } type;

    union
    {
        struct
        {
            CombatantId id;
            Color color;
        } enemyTint;
    } data;
} Event;

typedef struct _Battle
{
    Combatant combatants[NUM_COMBATANTS];
    CombatantId queue[NUM_COMBATANTS]; // This will eventually be used to implement initiative order.

    enum
    {
        BATTLE_SELECT_ACTION,
        BATTLE_SELECT_TARGET,
        BATTLE_EXECUTE_ACTION,
        BATTLE_EXECUTE_EFFECTS,
        BATTLE_SHOW_EVENTS,
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

        struct
        {
            int queueIndex;
            CombatantId targetIdOpt;
            Effect effects[MAX_EFFECTS];
            int effectCount;
            int effectIndex;
        } executeEffect;

        struct
        {
            int queueIndex;
            CombatantId targetIdOpt;
            Effect effects[MAX_EFFECTS];
            int effectCount;
            int effectIndex;
            Event events[MAX_EVENTS];
            int eventCount;
            int eventIndex;
        } showEvents;

    } data;
} _Battle;