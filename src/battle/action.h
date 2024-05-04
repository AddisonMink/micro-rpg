#pragma once

#define MAX_EFFECT_TEMPLATES 3
#define MAX_ACTIONS 3

typedef enum Range
{
    RANGE_SELF,
    RANGE_MELEE,
    RANGE_REACH,
    RANGE_PROJECTILE,
} Range;

#define NUM_RANGES 4

typedef enum DamageType
{
    DAMAGE_PHYSICAL,
    DAMAGE_MAGICAL,
} DamageType;

typedef enum Direction
{
    DIRECTION_FORWARD,
    DIRECTION_BACK,
} Direction;

typedef struct EffectTemplate
{
    enum
    {
        EFFECT_TEMPLATE_DAMAGE,
        EFFECT_TEMPLATE_KILL,
        EFFECT_TEMPLATE_MOVE,
        EFFECT_TEMPLATE_ATTACK,
        EFFECT_TEMPLATE_AUTO_MOVE,
    } type;

    union
    {
        struct
        {
            DamageType damageType;
            int amount;
        } damage;

        struct
        {
            Direction direction;
        } move;

        struct
        {
            int amount;
        } useItem;

        struct
        {
            DamageType damageType;
        } attack;

    } data;
} EffectTemplate;

#define MAKE_EFFECT_TEMPLATE_DAMAGE(DAMAGE_TYPE, AMOUNT)                   \
    (EffectTemplate)                                                       \
    {                                                                      \
        .type = EFFECT_TEMPLATE_DAMAGE,                                    \
        .data = {.damage = {.damageType = DAMAGE_TYPE, .amount = AMOUNT} } \
    }

#define MAKE_EFFECT_TEMPLATE_KILL()   \
    (EffectTemplate)                  \
    {                                 \
        .type = EFFECT_TEMPLATE_KILL, \
    }

#define MAKE_EFFECT_TEMPLATE_MOVE(DIRECTION)        \
    (EffectTemplate)                                \
    {                                               \
        .type = EFFECT_TEMPLATE_MOVE,               \
        .data = {.move = {.direction = DIRECTION} } \
    }

#define MAKE_EFFECT_TEMPLATE_ATTACK(DAMAGE_TYPE)         \
    (EffectTemplate)                                     \
    {                                                    \
        .type = EFFECT_TEMPLATE_ATTACK,                  \
        .data = {.attack = {.damageType = DAMAGE_TYPE} } \
    }

#define MAKE_EFFECT_TEMPLATE_AUTO_MOVE()   \
    (EffectTemplate)                       \
    {                                      \
        .type = EFFECT_TEMPLATE_AUTO_MOVE, \
    }

typedef struct EffectTemplateList
{
    EffectTemplate data[MAX_EFFECT_TEMPLATES];
    int capacity;
    int count;
} EffectTemplateList;

typedef enum ActionId
{
    // Basic
    ACTION_ATTACK,
    ACTION_MOVE,
    ACTION_WAIT,

    // Enemu
    ACTION_OGRE_SMASH,

    ACTION_SCAMP_BLOWGUN,

    // Item
    ACTION_LODESTONE_ZAP,
    ACTION_LODESTONE_PULL,

    ACTION_WAND_BONK,
    ACTION_WAND_SHOOT,
} ActionId;

typedef struct Action
{
    ActionId id;
    const char *name;
    Range range;
    int magicLevel;
    int cost;
    EffectTemplateList effects;
} Action;

typedef struct ActionList
{
    const Action *data[MAX_ACTIONS];
    int capacity;
    int count;
} ActionList;

const Action *Action_Load(ActionId id);