#include "battle.h"

#include "raylib.h"

#include "battle/types.h"
#include "battle/draw/draw.h"
#include "battle/update/update.h"

Battle *BattleAlloc()
{
    _Battle *battle = MemAlloc(sizeof(_Battle));

    for (CombatantId id = 0; id < NUM_COMBATANTS; id++)
        battle->combatants[id].state = COMBATANT_STATE_INVALID;
    CombatantInit(&battle->combatants[0], 0, COMBATANT_PLAYER, ROW_FRONT);
    CombatantInit(&battle->combatants[FIRST_ENEMY_ID], FIRST_ENEMY_ID, COMBATANT_SCAMP, ROW_FRONT);
    CombatantInit(&battle->combatants[FIRST_ENEMY_ID + 1], FIRST_ENEMY_ID + 1, COMBATANT_SCAMP, ROW_BACK);

    ItemInit(&battle->items[0], ITEM_LODESTONE);
    battle->itemCount = 1;

    for (CombatantId id = 0; id < NUM_COMBATANTS; id++)
        battle->queue[id] = id;

    battle->state = BATTLE_SELECT_ACTION;
    battle->data.selectAction.queueIndex = 0;
    battle->data.selectAction.actionIndex = 0;

    return (Battle *)battle;
}

void BattleFree(Battle *battle)
{
    MemFree(battle);
}

void BattleUpdate(Battle *battle, float delta)
{
    BattleUpdateMain((_Battle *)battle, delta);
}

void BattleDraw(Battle *battle, UI *ui)
{
    BattleDrawMain(ui, (_Battle *)battle);
}