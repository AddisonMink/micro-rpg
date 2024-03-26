#include "action.h"

static const Action actions[] = {
    [ACTION_WAIT] = {
        .type = ACTION_WAIT,
        .name = "Wait",
        .range = RANGE_SELF,
        .effectCount = 0,
    },

    [ACTION_ATTACK] = {
        .type = ACTION_ATTACK,
        .name = "Attack",
        .range = RANGE_MELEE,
        .effectCount = 1,
        .effects = {
            MAKE_EFFECT_DAMAGE(false, 1, DAMAGE_SOURCE_STRENGTH, DAMAGE_TYPE_PHYSICAL),
        },
    },

    [ACTION_MOVE] = {
        .type = ACTION_MOVE,
        .name = "Move",
        .range = RANGE_SELF,
        .effectCount = 1,
        .effects = {
            MAKE_EFFECT_MOVE(true, MOVE_TOGGLE),
        },
    },
};

static const PlayerActions playerActions[NUM_PLAYER_TYPES] = {
    [COMBATANT_PLAYER] = {
        .count = 2,
        .actions = {ACTION_ATTACK, ACTION_MOVE},
    },
};

const Action *ActionGet(ActionType type)
{
    return &actions[type];
}

const PlayerActions *PlayerActionsGet(CombatantType type)
{
    return &playerActions[type];
}