#pragma once

#include "battle/event.h"
#include "battle/combatant.h"
#include "common/common-ui.h"

void EnemyDisplay3D_Draw(const Camera3D *camera, const Combatant combatants[MAX_COMBATANTS], const Event *event);