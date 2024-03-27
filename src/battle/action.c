#include "action.h"

static const Action actions[] = {
    // General
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

    [ACTION_ITEM] = {
        .type = ACTION_ITEM,
        .name = "Item",
        .range = RANGE_SELF,
        .effectCount = 0,
        .effects = {},
    },

    // Lodestone
    [ACTION_LODESTONE_HOLD] = {
        .type = ACTION_LODESTONE_HOLD,
        .name = "Hold",
        .range = RANGE_SELF,
        .effectCount = 0,
        .effects = {},
    },

    [ACTION_LODESTONE_PULL] = {
        .type = ACTION_LODESTONE_PULL,
        .name = "Pull",
        .range = RANGE_PROJECTILE,
        .effectCount = 0,
        .effects = {},
    },

    // Scamp
    [ACTION_SCAMP_CLEAVER] = {
        .type = ACTION_SCAMP_CLEAVER,
        .name = "Cleaver",
        .range = RANGE_PROJECTILE,
        .effectCount = 1,
        .effects = {
            MAKE_EFFECT_DAMAGE(false, 2, DAMAGE_SOURCE_STRENGTH, DAMAGE_TYPE_PHYSICAL),
        },
    },
};

static const PlayerActions playerActions[NUM_PLAYER_TYPES] = {
    [COMBATANT_PLAYER] = {
        .count = 4,
        .actions = {ACTION_ATTACK, ACTION_MOVE, ACTION_ITEM, ACTION_WAIT},
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