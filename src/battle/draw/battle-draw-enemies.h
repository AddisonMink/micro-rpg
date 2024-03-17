#pragma once

#include "../../ui/ui.h"
#include "../battle-types.h"

typedef struct DrawEnemyOptions
{
    bool showStatusPane;
} DrawEnemyOptions;

void BattleDrawEnemies(UI *ui, const _Battle *battle, DrawEnemyOptions options);
