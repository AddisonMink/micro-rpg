#include "execute-effect.h"

static void pushEvent(_Battle *battle, int *eventCount, Event event)
{
    if (*eventCount < MAX_EVENTS)
        battle->events[(*eventCount)++] = event;
    else
        TraceLog(LOG_INFO, "BATTLE-EXECUTE-EFFECT: Event buffer is full!");
}

static void pushEffect(_Battle *battle, int *effectCount, Effect effect)
{
    if (*effectCount < MAX_EFFECTS)
        battle->effects[(*effectCount)++] = effect;
    else
        TraceLog(LOG_INFO, "BATTLE-EXECUTE-EFFECT: Effect buffer is full!");
}

void BattleExecuteEffect(_Battle *battle, int *eventCount)
{
    int *effectCount = &battle->data.executeEffect.effectCount;
    int *effectIndex = &battle->data.executeEffect.effectIndex;
    const Effect *effect = &battle->effects[*effectIndex];

    switch (effect->type)
    {
    case EFFECT_DAMAGE:
    {
        const CombatantId id = effect->target;
        Combatant *combatant = &battle->combatants[id];
        combatant->hp -= effect->data.damage.amount;

        pushEvent(battle, eventCount, EventSlashAnimation(id));
        pushEvent(battle, eventCount, EventWait(0.1f));
        pushEvent(battle, eventCount, EventDamageFlash(id));
        pushEvent(battle, eventCount, EventWait(0.2f));
        if (id >= FIRST_ENEMY_ID)
            pushEvent(battle, eventCount, EventEnemyStatus(id));

        if (combatant->hp <= 0)
        {
            Effect deathEffect = {.type = EFFECT_DEATH, .target = id};
            pushEvent(battle, eventCount, EventDeathFade(id));
            pushEffect(battle, effectCount, deathEffect);
        }
        break;
    }
    case EFFECT_DEATH:
    {
        const CombatantId id = effect->target;
        Combatant *combatant = &battle->combatants[id];
        combatant->hp = 0;
        combatant->state = COMBATANT_STATE_DEAD;
        break;
    }
    case EFFECT_MOVE:
    {
        const CombatantId id = effect->target;
        Combatant *combatant = &battle->combatants[id];
        switch (effect->data.move.moveType)
        {
        case MOVE_FORWARD:
            combatant->row = ROW_FRONT;
            break;
        case MOVE_BACK:
            combatant->row = ROW_BACK;
            break;
        case MOVE_TOGGLE:
            combatant->row = (combatant->row == ROW_FRONT) ? ROW_BACK : ROW_FRONT;
            break;
        }
        break;
    }
    }
}