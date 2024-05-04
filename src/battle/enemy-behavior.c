#include "enemy-behavior.h"

#include "battle/target.h"
#include "common/list-macros.h"

static CombatantRefList allyList(Combatant combatants[MAX_COMBATANTS], Combatant *enemy)
{
    CombatantRefList result = LIST_INIT(MAX_ENEMIES);
    for (Id id = FIRST_ENEMY_ID; id < MAX_COMBATANTS; id++)
    {
        Combatant *combatant = &combatants[id];
        if (combatant->state == COMBATANT_STATE_ALIVE && combatant->id != enemy->id)
        {
            LIST_APPEND(&result, combatant);
        }
    }

    return result;
}

static Id chooseTargetWithLowestArmor(const Combatant combatants[MAX_COMBATANTS], TargetList targets)
{
    int lowestArmor = __INT_MAX__;
    Id result;
    LIST_ITERATE_DEPRECATED((&targets))
    {
        const Id candidateId = LIST_ELEM_DEPRECATED((&targets), i);
        const Combatant *target = &combatants[candidateId];
        if (target->armor < lowestArmor)
        {
            lowestArmor = target->armor;
            result = target->id;
        }
    }

    return result;
}

static bool tryAction(EnemyBehavior *result, const Combatant combatants[MAX_COMBATANTS], const Combatant *enemy, ActionId actionId)
{
    const Action *action = Action_Load(actionId);
    const TargetList targets = TargetList_Create(action, combatants, enemy->id);
    bool success = false;

    if (!LIST_EMPTY_DEPRECATED((&targets)))
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

EnemyBehavior EnemyBehavior_Get(Combatant combatants[MAX_COMBATANTS], Id enemyId)
{
    Combatant *enemy = &combatants[enemyId];

    EnemyBehavior result = {
        .action = Action_Load(ACTION_WAIT),
        .targetOpt = -1,
    };

    switch (enemy->type)
    {
    case COMBATANT_TYPE_OGRE:
    {
        if (tryAction(&result, combatants, enemy, ACTION_OGRE_SMASH))
            break;
        break;
    }
    case COMBATANT_TYPE_SCAMP:
    {
        if (tryAction(&result, combatants, enemy, ACTION_SCAMP_BLOWGUN))
            break;

        CombatantRefList allies = allyList(combatants, enemy);
        const bool allyInFront = LIST_ANY(&allies, Combatant *, ally, (*ally)->row == ROW_FRONT);
        if (allyInFront && tryAction(&result, combatants, enemy, ACTION_SCAMP_HOOK))
            break;

        if (tryAction(&result, combatants, enemy, ACTION_SCAMP_KNIFE))
            break;
        break;
    }
    default:
        break;
    }

    return result;
}