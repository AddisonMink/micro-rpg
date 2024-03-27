#include "update.h"

#include "battle/update/enemy-behavior.h"
#include "battle/update/execute-effect.h"
#include "battle/update/util.h"

static float enemyTurnDuration = 0.5f;

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
        case RANGE_PROJECTILE:
        {
            battle->state = BATTLE_SELECT_TARGET;
            battle->data.selectAction.queueIndex = queueIndex;
            battle->data.selectTarget.actionType = actionType;
            FindTargets(battle->targets, &battle->data.selectTarget.targetCount, battle, id, action->range);
            battle->data.selectTarget.targetIndex = 0;
            break;
        }
        }
    }
}

static void selectTarget(_Battle *battle)
{
    const int queueIndex = battle->data.selectAction.queueIndex;
    const int targetIndex = battle->data.selectTarget.targetIndex;
    const int targetCount = battle->data.selectTarget.targetCount;

    if (targetCount > 0 && IsKeyPressed(KEY_D))
    {
        battle->data.selectTarget.targetIndex = (targetIndex + 1) % targetCount;
    }
    else if (targetCount > 0 && IsKeyPressed(KEY_A))
    {
        battle->data.selectTarget.targetIndex = (targetIndex - 1 + targetCount) % targetCount;
    }
    else if (targetCount > 0 && IsKeyPressed(KEY_ENTER))
    {
        battle->state = BATTLE_EXECUTE_ACTION;
        battle->data.executeAction.queueIndex = queueIndex;
        battle->data.executeAction.actionType = battle->data.selectTarget.actionType;
        battle->data.executeAction.targetIdOpt = battle->targets[targetIndex];
    }
    else if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE))
    {
        battle->state = BATTLE_SELECT_ACTION;
        battle->data.selectAction.queueIndex = queueIndex;
        battle->data.selectAction.actionIndex = 0;
    }
}

static void executeAction(_Battle *battle)
{
    const int queueIndex = battle->data.executeAction.queueIndex;
    const ActionType actionType = battle->data.executeAction.actionType;
    const CombatantId targetOpt = battle->data.executeAction.targetIdOpt;
    const CombatantId id = battle->queue[queueIndex];
    const Action *action = ActionGet(actionType);

    int effectCount;
    ActionSetTarget(battle->effects, &effectCount, action, id, targetOpt);

    battle->state = BATTLE_EXECUTE_EFFECTS;
    battle->data.executeEffect.queueIndex = queueIndex;
    battle->data.executeEffect.targetIdOpt = targetOpt;
    battle->data.executeEffect.effectCount = effectCount;
    battle->data.executeEffect.effectIndex = 0;
}

static void executeEffects(_Battle *battle)
{
    const int effectCount = battle->data.executeEffect.effectCount;
    int effectIndex = battle->data.executeEffect.effectIndex;
    const int queueIndex = battle->data.executeEffect.queueIndex;
    const CombatantId targetOpt = battle->data.executeEffect.targetIdOpt;

    if (effectIndex < effectCount)
    {
        int eventCount = 0;
        BattleExecuteEffect(battle, &eventCount);
        const int newEffectCount = battle->data.executeEffect.effectCount;
        effectIndex++;

        if (eventCount > 0)
        {
            battle->state = BATTLE_SHOW_EVENTS;
            battle->data.showEvents.queueIndex = queueIndex;
            battle->data.showEvents.targetIdOpt = targetOpt;
            battle->data.showEvents.effectCount = newEffectCount;
            battle->data.showEvents.effectIndex = effectIndex;
            battle->data.showEvents.eventCount = eventCount;
            battle->data.showEvents.eventIndex = 0;
        }
        else
        {
            battle->data.executeEffect.effectIndex = effectIndex;
        }
    }
    else
    {
        battle->state = BATTLE_END_TURN;
        battle->data.endTurn.queueIndex = queueIndex;
    }
}

static void showEvents(_Battle *battle, float delta)
{
    const int eventCount = battle->data.showEvents.eventCount;

    if (battle->data.showEvents.effectIndex < eventCount)
    {
        Event *event = &battle->events[battle->data.showEvents.eventIndex];
        event->elapsed += delta;
        if (event->elapsed >= event->duration)
        {
            battle->data.showEvents.eventIndex++;
        }
    }

    if (battle->data.showEvents.eventIndex >= eventCount)
    {
        const int queueIndex = battle->data.showEvents.queueIndex;
        const CombatantId targetOpt = battle->data.showEvents.targetIdOpt;
        const int effectCount = battle->data.showEvents.effectCount;
        const int effectIndex = battle->data.showEvents.effectIndex;

        battle->state = BATTLE_EXECUTE_EFFECTS;
        battle->data.executeEffect.queueIndex = queueIndex;
        battle->data.executeEffect.targetIdOpt = targetOpt;
        battle->data.executeEffect.effectCount = effectCount;
        battle->data.executeEffect.effectIndex = effectIndex;
    }
}

static bool playersDead(_Battle *battle)
{
    for (CombatantId id = 0; id < FIRST_ENEMY_ID; id++)
        if (battle->combatants[id].state == COMBATANT_STATE_ALIVE)
            return false;
    return true;
}

static bool enemiesDead(_Battle *battle)
{
    for (CombatantId id = FIRST_ENEMY_ID; id < NUM_COMBATANTS; id++)
        if (battle->combatants[id].state == COMBATANT_STATE_ALIVE)
            return false;
    return true;
}

static void endTurn(_Battle *battle)
{
    BuildQueue(battle, &battle->data.endTurn.queueIndex);
    ResetRows(battle);
    
    const int queueIndex = battle->data.endTurn.queueIndex;
    const int nextIndex = (queueIndex + 1) % battle->queueCount;
    const CombatantId nextId = battle->queue[nextIndex];

    if (playersDead(battle))
    {
        battle->state = BATTLE_LOSE;
    }
    else if (enemiesDead(battle))
    {
        battle->state = BATTLE_WIN;
    }
    else if (nextId < FIRST_ENEMY_ID)
    {
        battle->state = BATTLE_SELECT_ACTION;
        battle->data.selectAction.queueIndex = nextIndex;
        battle->data.selectAction.actionIndex = 0;
    }
    else
    {
        const CombatantId enemy = battle->queue[nextIndex];
        ActionType actionType;
        CombatantId targetOpt;
        BattleEnemyBehavior(&actionType, &targetOpt, battle, enemy);

        battle->state = BATTLE_ENEMY_TURN;
        battle->data.enemyTurn.queueIndex = nextIndex;
        battle->data.enemyTurn.actionType = actionType;
        battle->data.enemyTurn.targetIdOpt = targetOpt;
        battle->data.enemyTurn.elapsed = 0;
    }
}

static void enemyTurn(_Battle *battle, float delta)
{
    battle->data.enemyTurn.elapsed += delta;
    if (battle->data.enemyTurn.elapsed >= enemyTurnDuration)
    {
        const int queueIndex = battle->data.enemyTurn.queueIndex;
        const ActionType actionType = battle->data.enemyTurn.actionType;
        const CombatantId targetIdOpt = battle->data.enemyTurn.targetIdOpt;

        battle->state = BATTLE_EXECUTE_ACTION;
        battle->data.executeAction.queueIndex = queueIndex;
        battle->data.executeAction.actionType = actionType;
        battle->data.executeAction.targetIdOpt = targetIdOpt;
    }
}

void BattleUpdateMain(_Battle *battle, float delta)
{
    switch (battle->state)
    {
    case BATTLE_SELECT_ACTION:
        return selectAction(battle);
    case BATTLE_SELECT_TARGET:
        return selectTarget(battle);
    case BATTLE_EXECUTE_ACTION:
        return executeAction(battle);
    case BATTLE_EXECUTE_EFFECTS:
        return executeEffects(battle);
    case BATTLE_SHOW_EVENTS:
        return showEvents(battle, delta);
    case BATTLE_END_TURN:
        return endTurn(battle);
    case BATTLE_ENEMY_TURN:
        return enemyTurn(battle, delta);
    case BATTLE_WIN:
        return;
    case BATTLE_LOSE:
        return;
    }
}