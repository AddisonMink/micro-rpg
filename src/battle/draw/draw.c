#include "draw.h"

#include "battle/draw/draw-enemies.h"
#include "battle/draw/draw-menu.h"
#include "battle/draw/draw-players.h"

void BattleDrawMain(UI *ui, _Battle *battle)
{
    BattleDrawEnemies(ui, battle);
    BattleDrawPlayers(ui, battle);
    BattleDrawMenu(ui, battle);
}