#pragma once

#include "action.h"
#include "combatant.h"

#define NUM_COMBATANTS 6
#define FIRST_ENEMY_ID 3
#define MAX_EVENTS 10

typedef struct Event
{
    float duration;
    float elapsed;

    enum
    {
        EVENT_WAIT,
        EVENT_ENEMY_TINT,
        EVENT_ENEMY_ANIMATION,
        EVENT_ENEMY_STATUS,
        EVENT_ENEMY_FADE,
    } type;

    union
    {
        struct
        {
            CombatantId id;
            Color color;
        } enemyTint;

        struct
        {
            CombatantId id;
            AnimationTag tag;
        } enemyAnimation;

        struct
        {
            CombatantId id;
        } enemyStatus;

        struct
        {
            CombatantId id;
        } enemyFade;

    } data;
} Event;

typedef struct _Battle
{
    Combatant combatants[NUM_COMBATANTS];
    CombatantId queue[NUM_COMBATANTS]; // This will eventually be used to implement initiative order.

    CombatantId targets[NUM_COMBATANTS];
    Effect effects[MAX_EFFECTS];
    Event events[MAX_EVENTS];

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
            int effectCount;
            int effectIndex;
        } executeEffect;

        struct
        {
            int queueIndex;
            CombatantId targetIdOpt;
            int effectCount;
            int effectIndex;
            int eventCount;
            int eventIndex;
        } showEvents;

    } data;
} _Battle;