#pragma once

#include "battle/combatant.h"
#include "battle/event.h"
#include "common/common-ui.h"

void EnemyDisplay_Draw(UI *ui, const Combatant combatants[MAX_ENEMIES], Id selected, const Event *event, bool showStatus);