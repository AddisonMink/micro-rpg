#include "battle-execute-effect.h"

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

void BattleExecuteEffect(_Battle *battle, Event events[MAX_EVENTS], int *eventCount, const Effect *effect)
{
    switch (effect->type)
    {
    case EFFECT_DAMAGE:
    {
        const CombatantId id = effect->target;
        Combatant *combatant = &battle->combatants[id];
        combatant->hp -= effect->data.damage.amount;
        events[(*eventCount)++] = enemySlashAnimation(id);
        events[(*eventCount)++] = wait(0.1f);
        events[(*eventCount)++] = enemyDamageFlash(id);
        events[(*eventCount)++] = wait(0.2f);
        events[(*eventCount)++] = enemyStatus(id);
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