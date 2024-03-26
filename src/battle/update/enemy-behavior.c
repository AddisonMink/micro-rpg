#include "enemy-behavior.h"

#include "battle/update/util.h"

typedef struct State
{
    CombatantId enemy;
    ActionType actionType;
    CombatantId targets[NUM_COMBATANTS];
    int targetCount;
} State;

static bool attemptAction(State *state, const _Battle *battle, ActionType action)
{
    const Action *actionData = ActionGet(action);

    switch (actionData->range)
    {
    case RANGE_SELF:
    {
        state->targets[0] = state->enemy;
        state->targetCount = 1;
        state->actionType = action;
        return true;
    }
    case RANGE_MELEE:
    {
        FindTargets(state->targets, &state->targetCount, battle, state->enemy, actionData->range);
        state->actionType = action;
        return state->targetCount > 0;
    }
    }
}

void BattleEnemyBehavior(ActionType *actionType, CombatantId *targetOpt, const _Battle *battle, CombatantId enemy)
{
    State state = {.enemy = enemy};

    if (attemptAction(&state, battle, ACTION_ATTACK))
    {
        *actionType = state.actionType;
        *targetOpt = state.targets[0];
    }
    else
    {
        *actionType = ACTION_WAIT;
    }
}