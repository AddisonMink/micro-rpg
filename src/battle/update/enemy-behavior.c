#include "enemy-behavior.h"

void BattleEnemyBehavior(ActionType *actionType, CombatantId *target, const _Battle *battle)
{
    *actionType = ACTION_ATTACK;
    *target = battle->queue[0];
}