#pragma once

#include "battle/combatant.h"

typedef enum DamageSource
{
    DAMAGE_SOURCE_STRENGTH,
    DAMAGE_SOURCE_MAGIC
} DamageSource;

typedef enum DamageType
{
    DAMAGE_TYPE_PHYSICAL,
    DAMAGE_TYPE_MAGICAL
} DamageType;

typedef enum MoveType
{
    MOVE_FORWARD,
    MOVE_BACK,
    MOVE_TOGGLE,
} MoveType;

typedef struct Effect
{
    CombatantId source;
    CombatantId target;
    bool selfTarget;

    enum
    {
        EFFECT_DAMAGE,
        EFFECT_DEATH,
        EFFECT_MOVE,
    } type;

    union
    {
        struct
        {
            int amount;
            DamageSource source;
            DamageType type;
        } damage;

        struct
        {
            MoveType moveType;
        } move;
    } data;
} Effect;

#define MAKE_EFFECT_DAMAGE(SELF_TARGET, AMOUNT, SOURCE, DAMAGE) \
    (Effect)                                                    \
    {                                                           \
        .selfTarget = SELF_TARGET,                              \
        .type = EFFECT_DAMAGE,                                  \
        .data.damage = {                                        \
            .amount = AMOUNT,                                   \
            .source = SOURCE,                                   \
            .type = DAMAGE,                                     \
        }                                                       \
    }

#define MAKE_EFFECT_MOVE(SELF_TARGET, MOVE_TYPE) \
    (Effect)                                     \
    {                                            \
        .selfTarget = SELF_TARGET,               \
        .type = EFFECT_MOVE,                     \
        .data.move = {                           \
            .moveType = MOVE_TYPE,               \
        }                                        \
    }
