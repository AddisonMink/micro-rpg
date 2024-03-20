#pragma once

#include "../battle-types.h"

void ActionSetTarget(Effect effects[MAX_EFFECTS], int *effectCount, const Action *action, CombatantId source, CombatantId targetOpt);