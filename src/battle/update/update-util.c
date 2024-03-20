#include "update-util.h"

void ActionSetTarget(Effect effects[MAX_EFFECTS], int *effectCount, const Action *action, CombatantId source, CombatantId targetOpt)
{
    *effectCount = action->effectCount;

    switch (action->range)
    {
    case RANGE_SELF:
    {
        for (int i = 0; i < *effectCount; i++)
        {
            effects[i] = action->effects[i];
            effects[i].source = source;
            effects[i].target = source;
        }
        break;
    }
    case RANGE_MELEE:
        TraceLog(LOG_INFO, "ACTION-SET-TARGET: Melee range not implemented");
        break;
    }
}