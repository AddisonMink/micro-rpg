#pragma once

#include "battle/effect.h"

typedef enum Range
{
    RANGE_SELF,
    RANGE_MELEE,
    RANGE_PROJECTILE,
} Range;

#define NUM_RANGE_TYPES 3

typedef enum ActionType
{
    ACTION_WAIT,
    ACTION_ATTACK,
    ACTION_MOVE,

    ACTION_SCAMP_CLEAVER,
} ActionType;

#define MAX_EFFECTS 10

typedef struct Action
{
    ActionType type;
    const char *name;
    Range range;
    int effectCount;
    Effect effects[MAX_EFFECTS];
} Action;

#define MAX_PLAYER_ACTIONS 2

typedef struct PlayerActions
{
    int count;
    ActionType actions[MAX_PLAYER_ACTIONS];
} PlayerActions;

const Action *ActionGet(ActionType type);

const PlayerActions *PlayerActionsGet(CombatantType type);