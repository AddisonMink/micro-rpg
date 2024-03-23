#include "battle-execute-effect.h"

static const Event enemyDamageFlash(CombatantId id)
{
    return (Event){
        .duration = 0.1f,
        .elapsed = 0,
        .type = EVENT_ENEMY_TINT,
        .data.enemyTint = {
            .id = id,
            .color = RED,
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
        if (combatant->hp <= 0)
        {
            combatant->state = COMBATANT_STATE_DEAD;
        }
        events[(*eventCount)++] = enemyDamageFlash(id);
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