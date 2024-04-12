#pragma once

#include "battle/combatant.h"
#include "battle/event.h"
#include "common/common-ui.h"

void PlayerDisplay_Draw(UI *ui, const Combatant combatants[MAX_COMBATANTS], Id selected, const Event *eventOpt);