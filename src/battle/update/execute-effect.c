#include "execute-effect.h"

static const Event wait(float duration)
{
    return (Event){
        .duration = duration,
        .elapsed = 0,
        .type = EVENT_WAIT,
    };
}

static const Event enemyDamageFlash(CombatantId id)
{
    return (Event){
        .duration = 0.05f,
        .elapsed = 0,
        .type = EVENT_ENEMY_TINT,
        .data.enemyTint = {
            .id = id,
            .color = RED,
        },
    };
}

static const Event enemySlashAnimation(CombatantId id)
{
    return (Event){
        .duration = AssetAnimationDuration(AssetAnimation(ANIMATION_SLASH)),
        .elapsed = 0,
        .type = EVENT_ENEMY_ANIMATION,
        .data.enemyAnimation = {
            .id = id,
            .tag = ANIMATION_SLASH,
        },
    };
}

static const Event enemyStatus(CombatantId id)
{
    return (Event){
        .duration = 0.5f,
        .elapsed = 0,
        .type = EVENT_ENEMY_STATUS,
        .data.enemyStatus = {
            .id = id,
        },
    };
}

static const Event enemyDeathFade(CombatantId id)
{
    return (Event){
        .duration = 0.5f,
        .elapsed = 0,
        .type = EVENT_ENEMY_FADE,
        .data.enemyFade = {
            .id = id,
        },
    };
}

static void pushEvent(_Battle *battle, int *eventCount, Event event)
{
    if (*eventCount < MAX_EVENTS)
        battle->events[(*eventCount)++] = event;
    else
        TraceLog(LOG_INFO, "BATTLE-EXECUTE-EFFECT: Event buffer is full!");
}

static void pushEffect(_Battle *battle, int *effectCount, Effect effect)
{
    if (*effectCount < MAX_EFFECTS)
        battle->effects[(*effectCount)++] = effect;
    else
        TraceLog(LOG_INFO, "BATTLE-EXECUTE-EFFECT: Effect buffer is full!");
}

void BattleExecuteEffect(_Battle *battle, int *eventCount)
{
    int *effectCount = &battle->data.executeEffect.effectCount;
    int *effectIndex = &battle->data.executeEffect.effectIndex;
    const Effect *effect = &battle->effects[*effectIndex];

    switch (effect->type)
    {
    case EFFECT_DAMAGE:
    {
        const CombatantId id = effect->target;
        Combatant *combatant = &battle->combatants[id];
        combatant->hp -= effect->data.damage.amount;

        pushEvent(battle, eventCount, enemySlashAnimation(id));
        pushEvent(battle, eventCount, wait(0.1f));
        pushEvent(battle, eventCount, enemyDamageFlash(id));
        pushEvent(battle, eventCount, wait(0.2f));
        pushEvent(battle, eventCount, enemyStatus(id));

        if (combatant->hp <= 0)
        {
            Effect deathEffect = {.type = EFFECT_DEATH, .target = id};
            pushEvent(battle, eventCount, enemyDeathFade(id));
            pushEffect(battle, effectCount, deathEffect);
        }
        break;
    }
    case EFFECT_DEATH:
    {
        const CombatantId id = effect->target;
        Combatant *combatant = &battle->combatants[id];
        combatant->hp = 0;
        combatant->state = COMBATANT_STATE_DEAD;
        break;
    }
    case EFFECT_MOVE:
    {
        const CombatantId id = effect->target;
        Combatant *combatant = &battle->combatants[id];
        switch (effect->data.move.moveType)
        {
        case MOVE_FORWARD:
            combatant->row = ROW_FRONT;
            break;
        case MOVE_BACK:
            combatant->row = ROW_BACK;
            break;
        case MOVE_TOGGLE:
            combatant->row = (combatant->row == ROW_FRONT) ? ROW_BACK : ROW_FRONT;
            break;
        }
        break;
    }
    }
}