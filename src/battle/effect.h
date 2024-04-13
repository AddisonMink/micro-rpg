#pragma once

#include "battle/action.h"
#include "battle/combatant.h"
#include "battle/event.h"
#include "battle/item.h"
#include "battle/queue.h"

#define MAX_EFFECTS 5

typedef enum EffectType
{
    EFFECT_DAMAGE,
    EFFECT_KILL,
    EFFECT_MOVE,
    EFFECT_USE_ITEM,
    EFFECT_BREAK_ITEM,
    EFFECT_PUSH_EVENT,
} EffectType;

typedef struct EffectDamage
{
    int amount;
    DamageType type;
    Id id;
} EffectDamage;

typedef struct EffectKill
{
    Id id;
} EffectKill;

typedef struct EffectMove
{
    Direction direction;
    Id id;
} EffectMove;

typedef struct EffectUseItem
{
    int amount;
    int itemIndex;
} EffectUseItem;

typedef struct EffectBreakItem
{
    int itemIndex;
} EffectBreakItem;

typedef struct EffectPushEvent
{
    Event event;
} EffectPushEvent;

typedef struct Effect
{
    EffectType type;
    union
    {
        EffectDamage damage;
        EffectKill kill;
        EffectMove move;
        EffectUseItem useItem;
        EffectBreakItem breakItem;
        EffectPushEvent pushEvent;
    };
} Effect;

Effect EffectDamage_Create(int amount, DamageType type, Id id);

Effect EffectKill_Create(Id id);

Effect EffectMove_Create(Direction direction, Id id);

Effect EffectUseItem_Create(int amount, int itemIndex);

Effect EffectBreakItem_Create(int itemIndex);

Effect EffectPushEvent_Create(Event event);

typedef struct EffectList
{
    Effect data[MAX_EFFECTS];
    int capacity;
    int count;
} EffectList;

typedef struct EffectResult
{
    EffectList effects;
    EventList events;
} EffectResult;

EffectList Effect_Compile(const Action *action, const Combatant *actor, Id targetOpt, int itemIndexOpt);

EffectResult Effect_Execute(Combatant combatants[MAX_COMBATANTS], ItemList *items, Queue *queue, Effect effect);