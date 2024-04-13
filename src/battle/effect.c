#include "effect.h"

#include "common/list-macros.h"

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