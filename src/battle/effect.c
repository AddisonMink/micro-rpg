#include "effect.h"

#include "common/list-macros.h"

static const char *itemBreakMessage = "Item broke!";

Effect EffectDamage_Create(int amount, DamageType type, Id id)
{
    return (Effect){
        .type = EFFECT_DAMAGE,
        .damage = {
            .amount = amount,
            .type = type,
            .id = id,
        },
    };
}

Effect EffectKill_Create(Id id)
{
    return (Effect){
        .type = EFFECT_KILL,
        .kill = {
            .id = id,
        },
    };
}

Effect EffectMove_Create(Direction direction, Id id)
{
    return (Effect){
        .type = EFFECT_MOVE,
        .move = {
            .direction = direction,
            .id = id,
        },
    };
}

Effect EffectUseItem_Create(int amount, int itemIndex)
{
    return (Effect){
        .type = EFFECT_USE_ITEM,
        .useItem = {
            .amount = amount,
            .itemIndex = itemIndex,
        },
    };
}

Effect EffectBreakItem_Create(int itemIndex)
{
    return (Effect){
        .type = EFFECT_BREAK_ITEM,
        .breakItem = {
            .itemIndex = itemIndex,
        },
    };
}

EffectList Effect_Compile(const Action *action, const Combatant *actor, Id targetOpt, int itemIndexOpt)
{
    const Id target = targetOpt == -1 ? actor->id : targetOpt;
    EffectList effects = LIST_INIT(MAX_EFFECTS);

    LIST_ITERATE((&action->effects))
    {
        const EffectTemplate template = LIST_ELEM((&action->effects), i);

        switch (template.type)
        {
        case EFFECT_TEMPLATE_DAMAGE:
        {
            const int amount = template.data.damage.amount;
            const DamageType type = template.data.damage.damageType;
            LIST_APPEND((&effects), EffectDamage_Create(amount, type, target));
        }
        case EFFECT_TEMPLATE_KILL:
            LIST_APPEND((&effects), EffectKill_Create(target));
            break;
        case EFFECT_TEMPLATE_MOVE:
        {
            const Direction direction = template.data.move.direction;
            LIST_APPEND((&effects), EffectMove_Create(direction, target));
            break;
        }
        case EFFECT_TEMPLATE_USE_ITEM:
        {
            const int amount = template.data.useItem.amount;
            LIST_APPEND((&effects), EffectUseItem_Create(amount, itemIndexOpt));
            break;
        }
        case EFFECT_TEMPLATE_BREAK_ITEM:
            LIST_APPEND((&effects), EffectBreakItem_Create(itemIndexOpt));
            break;
        case EFFECT_TEMPLATE_ATTACK:
        {
            const int amount = actor->strength;
            const DamageType type = template.data.attack.damageType;
            LIST_APPEND((&effects), EffectDamage_Create(amount, type, target));
            break;
        }
        case EFFECT_TEMPLATE_AUTO_MOVE:
        {
            const Direction direction = actor->row == ROW_FRONT ? DIRECTION_BACK : DIRECTION_FORWARD;
            LIST_APPEND((&effects), EffectMove_Create(direction, actor->id));
            break;
        }
        }
    }

    return effects;
}

EffectResult Effect_Execute(Combatant combatants[MAX_COMBATANTS], ItemList *items, Queue *queue, Effect effect)
{
    EffectResult result = {
        .effects = LIST_INIT(MAX_EFFECTS),
        .events = LIST_INIT(MAX_EVENTS),
    };
    EffectList *effects = &result.effects;
    EventList *events = &result.events;

    switch (effect.type)
    {
    case EFFECT_DAMAGE:
    {
        const int amount = effect.damage.amount;
        const DamageType type = effect.damage.type;
        const Id id = effect.damage.id;
        Combatant *combatant = &combatants[id];

        int trueAmount = amount - combatant->armor;
        if (trueAmount < 0)
            trueAmount = 0;

        combatant->hp -= trueAmount;
        if (combatant->hp <= 0)
        {
            LIST_APPEND(effects, EffectKill_Create(id));
        }

        LIST_APPEND(events, Event_Animate(id, ANIMATION_SLASH));
        LIST_APPEND(events, Event_Flash(id, RED, 0.1));
        if (combatant->id >= FIRST_ENEMY_ID && combatant->hp > 0)
        {
            LIST_APPEND(events, Event_Wait(0.2));
            LIST_APPEND(events, Event_Status(id, 0.2));
        }
        break;
    }
    case EFFECT_KILL:
    {
        const Id id = effect.kill.id;
        Combatant *combatant = &combatants[id];

        combatant->hp = 0;
        combatant->state = COMBATANT_STATE_DEAD;
        Queue_Delete(queue, id);

        LIST_APPEND(events, Event_Fade(id, 0.5));
        break;
    }
    case EFFECT_MOVE:
    {
        const Id id = effect.move.id;
        const Direction direction = effect.move.direction;
        Combatant *combatant = &combatants[id];

        const Row row = direction == DIRECTION_FORWARD ? ROW_FRONT : ROW_BACK;
        const bool showMove = combatant->row != row;

        combatant->row = row;

        if (showMove)
        {
            LIST_APPEND(events, Event_Move(id, 0.2));
        }
        break;
    }
    case EFFECT_USE_ITEM:
    {
        const int amount = effect.useItem.amount;
        const int itemIndex = effect.useItem.itemIndex;
        Item *item = &LIST_ELEM(items, itemIndex);

        item->uses -= amount;
        if (item->uses <= 0)
        {
            LIST_APPEND(effects, EffectBreakItem_Create(itemIndex));
        }
        break;
    }
    case EFFECT_BREAK_ITEM:
    {
        const int itemIndex = effect.breakItem.itemIndex;
        LIST_DELETE(items, itemIndex);

        LIST_APPEND(events, Event_GlobalMessage(itemBreakMessage, 0.2));
        break;
    }
    }

    return result;
}