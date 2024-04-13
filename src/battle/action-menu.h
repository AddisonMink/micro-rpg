#pragma once

#include "battle/combatant.h"
#include "battle/item.h"
#include "common/common-ui.h"

typedef struct ActionMenuResult
{
    int itemIndexOpt;
    const Action *action;
} ActionMenuResult;

void ActionMenu_Init(const ItemList *items, const Combatant *player);

void ActionMenu_Draw(UI *ui);

const ActionMenuResult *ActionMenu_Update(float delta);