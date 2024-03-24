#pragma once

#include "ui/ui.h"

typedef struct Battle Battle;

Battle *BattleAlloc();

void BattleFree(Battle *battle);

void BattleUpdate(Battle *battle, float delta);

void BattleDraw(Battle *battle, UI *ui);