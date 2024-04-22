#include "enemy-behavior.h"

#include "battle/target.h"
#include "common/list-macros-new.h"

static Id chooseTargetWithLowestArmor(const Combatant combatants[MAX_COMBATANTS], TargetList targets)
{
    int lowestArmor = __INT_MAX__;
    Id result;
    LIST_ITER((&targets), Id, {
        const Combatant *target = &combatants[*_elem];
        if (target->armor < lowestArmor)
        {
            lowestArmor = target->armor;
            result = target->id;
        }
    })

    return result;
}

static bool tryAction(EnemyBehavior *result, const Combatant combatants[MAX_COMBATANTS], const Combatant *enemy, ActionId actionId)
{
    const Action *action = Action_Load(actionId);
    const TargetList targets = TargetList_Create(action, combatants, enemy->id);
    bool success = false;

    if (!LIST_EMPTY((&targets)))
    {
        const Id targetId = chooseTargetWithLowestArmor(combatants, targets);
        *result = (EnemyBehavior){
            .action = action,
            .targetOpt = targetId,
        };
        success = true;
    }

    return success;
}

EnemyBehavior EnemyBehavior_Get(const Combatant combatants[MAX_COMBATANTS], Id enemyId)
{
    const Combatant *enemy = &combatants[enemyId];

    EnemyBehavior result = {
        .action = Action_Load(ACTION_WAIT),
        .targetOpt = -1,
    };

    switch (enemy->type)
    {
    case COMBATANT_TYPE_SCAMP:
    {
        if (tryAction(&result, combatants, enemy, ACTION_SCAMP_CLEAVER))
            break;
        if (tryAction(&result, combatants, enemy, ACTION_ATTACK))
            break;
        break;
    }
    default:
        break;
    }

    return result;
}