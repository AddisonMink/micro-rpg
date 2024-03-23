#pragma once

#include "effect.h"

typedef enum Range
{
    RANGE_SELF,
    RANGE_MELEE,
} Range;

#define NUM_RANGE_TYPES 2

typedef enum ActionType
{
    ACTION_ATTACK,
    ACTION_MOVE,
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