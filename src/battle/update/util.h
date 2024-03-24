#pragma once

#include "battle/types.h"

void ActionSetTarget(Effect effects[MAX_EFFECTS], int *effectCount, const Action *action, CombatantId source, CombatantId targetOpt);

void FindTargets(CombatantId targets[NUM_COMBATANTS], int *targetCount, const _Battle *battle, CombatantId source, Range range);

void BuildQueue(_Battle *battle, int *queueIndex);