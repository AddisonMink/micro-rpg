#include "battle.h"

#include "raylib.h"

#include "battle-types.h"
#include "combatant.h"
#include "draw/battle-draw-enemies.h"
#include "draw/battle-draw-menu.h"
#include "draw/battle-draw-players.h"

Battle *BattleAlloc()
{
    _Battle *battle = MemAlloc(sizeof(_Battle));
    for (CombatantId id = 0; id < NUM_COMBATANTS; id++)
        battle->combatants[id].state = COMBATANT_STATE_INVALID;
    CombatantInit(&battle->combatants[0], 0, COMBATANT_PLAYER, ROW_FRONT);
    CombatantInit(&battle->combatants[FIRST_ENEMY_ID], FIRST_ENEMY_ID, COMBATANT_SCAMP, ROW_FRONT);
    CombatantInit(&battle->combatants[FIRST_ENEMY_ID + 1], FIRST_ENEMY_ID + 1, COMBATANT_SCAMP, ROW_BACK);

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

void BattleUpdate(Battle *battle, float delta) {}

void BattleDraw(Battle *battle, UI *ui)
{
    DrawEnemyOptions options = {.showStatusPane = IsKeyDown(KEY_TAB)};
    BattleDrawEnemies(ui, (_Battle *)battle, options);
    BattleDrawPlayers(ui, (_Battle *)battle);
    BattleDrawMenu(ui, (_Battle *)battle);
}