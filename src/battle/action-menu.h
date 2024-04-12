#pragma once

#include "battle/combatant.h"
#include "battle/item.h"
#include "common/common-ui.h"

void ActionMenu_Init(const ItemList *items, const Combatant *player);

void ActionMenu_Draw(UI *ui);

void ActionMenu_Update(float delta);