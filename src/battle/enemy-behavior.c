#include "enemy-behavior.h"

EnemyBehavior EnemyBehavior_Get(const Combatant combatants[MAX_COMBATANTS], Id enemy)
{
    return (EnemyBehavior){
        .action = Action_Load(ACTION_WAIT),
        .targetOpt = -1,
    };
}