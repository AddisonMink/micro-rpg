#include "battle-execute-effect.h"

void BattleExecuteEffect(_Battle *battle, const Effect *effect)
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