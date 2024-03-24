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

    // This will eventually be used to implement initiative order.
    CombatantId queue[NUM_COMBATANTS];
    int queueCount;

    // Only valid during BATTLE_SELECT_TARGET.
    CombatantId targets[NUM_COMBATANTS];

    // Only valid during BATTLE_EXECUTE_EFFECTS and BATTLE_SHOW_EVENTS.
    Effect effects[MAX_EFFECTS];

    // Only valid during BATTLE_SHOW_EVENTS.
    Event events[MAX_EVENTS];

    enum
    {
        BATTLE_SELECT_ACTION,
        BATTLE_SELECT_TARGET,
        BATTLE_EXECUTE_ACTION,
        BATTLE_EXECUTE_EFFECTS,
        BATTLE_SHOW_EVENTS,
        BATTLE_END_TURN,
        BATTLE_ENEMY_TURN,
        BATTLE_WIN,
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

        struct
        {
            int queueIndex;
        } endTurn;

        struct
        {
            int queueIndex;
            ActionType actionType;
            CombatantId targetIdOpt;
            float elapsed;
        } enemyTurn;

    } data;
} _Battle;