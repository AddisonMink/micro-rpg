#pragma once

#include "battle/action.h"
#include "battle/combatant.h"
#include "battle/item.h"
#include "common/common-ui.h"

typedef struct ActionMenuResult
{
    bool confirmed;
    const Action *action;
    int itemIndexOpt;
} ActionMenuResult;

void ActionMenu_Init(const ItemList *items, const Combatant *player);

void ActionMenu_Draw(UI *ui);

ActionMenuResult ActionMenu_Update(float delta);
