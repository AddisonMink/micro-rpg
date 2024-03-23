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
            FindTargets(battle->data.selectTarget.targets, &battle->data.selectTarget.targetCount, battle, id, action->range);
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

    if (IsKeyPressed(KEY_D))
    {
        battle->data.selectTarget.targetIndex = (targetIndex + 1) % targetCount;
    }
    else if (IsKeyPressed(KEY_A))
    {
        battle->data.selectTarget.targetIndex = (targetIndex - 1 + targetCount) % targetCount;
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        battle->state = BATTLE_EXECUTE_ACTION;
        battle->data.executeAction.queueIndex = queueIndex;
        battle->data.executeAction.actionType = battle->data.selectTarget.actionType;
        battle->data.executeAction.targetIdOpt = battle->data.selectTarget.targets[targetIndex];
    }
    else if (IsKeyPressed(KEY_DELETE))
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

    Effect effects[MAX_EFFECTS];
    int effectCount;
    ActionSetTarget(effects, &effectCount, action, id, targetOpt);

    battle->state = BATTLE_EXECUTE_EFFECTS;
    battle->data.executeEffect.queueIndex = queueIndex;
    battle->data.executeEffect.targetIdOpt = targetOpt;
    battle->data.executeEffect.effectCount = effectCount;
    battle->data.executeEffect.effectIndex = 0;
    for (int i = 0; i < effectCount; i++)
    {
        battle->data.executeEffect.effects[i] = effects[i];
    }
}

static void executeEffects(_Battle *battle)
{
    const int effectCount = battle->data.executeEffect.effectCount;
    int effectIndex = battle->data.executeEffect.effectIndex;
    const int queueIndex = battle->data.executeEffect.queueIndex;
    const CombatantId targetOpt = battle->data.executeEffect.targetIdOpt;
    const Effect *effect = &battle->data.executeEffect.effects[effectIndex];

    if (effectIndex < effectCount)
    {
        Event events[MAX_EVENTS];
        int eventCount = 0;
        BattleExecuteEffect(battle, events, &eventCount, effect);
        effectIndex++;

        if (eventCount > 0)
        {
            battle->state = BATTLE_SHOW_EVENTS;
            battle->data.showEvents.queueIndex = queueIndex;
            battle->data.showEvents.targetIdOpt = targetOpt;
            battle->data.showEvents.effectCount = effectCount;
            battle->data.showEvents.effectIndex = effectIndex;
            for (int i = effectIndex; i < effectCount; i++)
            {
                battle->data.showEvents.effects[i] = battle->data.executeEffect.effects[i];
            }
            battle->data.showEvents.eventCount = eventCount;
            battle->data.showEvents.eventIndex = 0;
            for (int i = 0; i < eventCount; i++)
            {
                battle->data.showEvents.events[i] = events[i];
            }
        }
        else
        {
            battle->data.executeEffect.effectIndex = effectIndex;
        }
    }
    else
    {
        battle->state = BATTLE_SELECT_ACTION;
        battle->data.selectAction.queueIndex = queueIndex;
        battle->data.selectAction.actionIndex = 0;
    }
}

static void showEvents(_Battle *battle, float delta)
{
    const int eventCount = battle->data.showEvents.eventCount;
    const int eventIndex = battle->data.showEvents.eventIndex;

    if (eventIndex >= eventCount)
    {
        const int queueIndex = battle->data.showEvents.queueIndex;
        const CombatantId targetOpt = battle->data.showEvents.targetIdOpt;
        const int effectCount = battle->data.showEvents.effectCount;
        const int effectIndex = battle->data.showEvents.effectIndex;
        Effect effects[MAX_EFFECTS];
        for (int i = effectIndex; i < effectCount; i++)
        {
            effects[i] = battle->data.showEvents.effects[i];
        }

        battle->state = BATTLE_EXECUTE_EFFECTS;
        battle->data.executeEffect.queueIndex = queueIndex;
        battle->data.executeEffect.targetIdOpt = targetOpt;
        battle->data.executeEffect.effectCount = effectCount;
        battle->data.executeEffect.effectIndex = effectIndex;
        for (int i = effectIndex; i < effectCount; i++)
        {
            battle->data.executeEffect.effects[i] = effects[i];
        }
    }
    else
    {
        Event *event = &battle->data.showEvents.events[eventIndex];
        event->elapsed += delta;
        if (event->elapsed >= event->duration)
        {
            battle->data.showEvents.eventIndex++;
        }
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
    }
}