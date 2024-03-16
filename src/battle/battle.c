#include "battle.h"

#include "raylib.h"

#include "battle-types.h"
#include "combatant-data.h"
#include "draw/battle-draw-enemies.h"

Battle *BattleAlloc()
{
    _Battle *battle = MemAlloc(sizeof(_Battle));
    for (CombatantId id = 0; id < NUM_COMBATANTS; id++)
        battle->combatants[id].state = COMBATANT_STATE_INVALID;
    CombatantInit(&battle->combatants[0], 0, COMBATANT_PLAYER, ROW_FRONT);
    CombatantInit(&battle->combatants[FIRST_ENEMY_ID], FIRST_ENEMY_ID, COMBATANT_SCAMP, ROW_FRONT);
    CombatantInit(&battle->combatants[FIRST_ENEMY_ID + 1], FIRST_ENEMY_ID + 1, COMBATANT_SCAMP, ROW_BACK);
    return (Battle *)battle;
}

void BattleFree(Battle *battle)
{
    MemFree(battle);
}

void BattleUpdate(Battle *battle, float delta) {}

void BattleDraw(Battle *battle, UI *ui)
{
    BattleDrawEnemies(ui, (_Battle *)battle);
}