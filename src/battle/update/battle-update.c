#include "battle-update.h"

#include "battle-execute-effect.h"
#include "update-util.h"

static void selectAction(_Battle *battle)
{
    const int queueIndex = battle->data.selectAction.queueIndex;
    const int actionIndex = battle->data.selectAction.actionIndex;
    const CombatantId id = battle->queue[queueIndex];
    const CombatantType type = battle->combatants[id].type;
    const PlayerActions *actions = PlayerActionsGet(type);
    const int count = actions->count;

    if (IsKeyPressed(KEY_W))
    {
        battle->data.selectAction.actionIndex = (actionIndex + 1) % count;
    }
    else if (IsKeyPressed(KEY_S))
    {
        battle->data.selectAction.actionIndex = (actionIndex - 1 + count) % count;
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        ActionType actionType = actions->actions[actionIndex];
        const Action *action = ActionGet(actionType);

        switch (action->range)
        {
        case RANGE_SELF:
        {
            battle->state = BATTLE_EXECUTE_ACTION;
            battle->data.executeAction.actionType = actionType;
            battle->data.executeAction.queueIndex = queueIndex;
            break;
        }
        case RANGE_MELEE:
        {
            battle->state = BATTLE_SELECT_TARGET;
            battle->data.selectAction.queueIndex = queueIndex;
            battle->data.selectTarget.actionType = actionType;
            battle->data.selectTarget.targetCount = 0;
            break;
        }
        }
    }
}

static void executeAction(_Battle *battle)
{
    const int queueIndex = battle->data.executeAction.queueIndex;
    const ActionType actionType = battle->data.executeAction.actionType;
    const CombatantId targetOpt = battle->data.executeAction.targetIdOpt;
    const CombatantId id = battle->queue[queueIndex];
    const Action *action = ActionGet(actionType);

    Effect effects[MAX_EFFECTS];
    int effectCount;
    ActionSetTarget(effects, &effectCount, action, id, targetOpt);
    for (int i = 0; i < effectCount; i++)
    {
        BattleExecuteEffect(battle, &effects[i]);
    }

    battle->state = BATTLE_SELECT_ACTION;
    battle->data.selectAction.queueIndex = queueIndex;
}

void BattleUpdateMain(_Battle *battle, float delta)
{
    switch (battle->state)
    {
    case BATTLE_SELECT_ACTION:
        return selectAction(battle);
    case BATTLE_SELECT_TARGET:
        return;
    case BATTLE_EXECUTE_ACTION:
        return executeAction(battle);
    }
}