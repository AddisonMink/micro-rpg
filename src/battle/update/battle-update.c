#include "battle-update.h"

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
}

void BattleUpdateMain(_Battle *battle, float delta)
{
    switch (battle->state)
    {
    case BATTLE_SELECT_ACTION:
        return selectAction(battle);
    }
}