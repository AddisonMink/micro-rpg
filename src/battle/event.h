#pragma once

#include "battle/combatant.h"

typedef enum EventType
{
    EVENT_WAIT,
    EVENT_ENEMY_TINT,
    EVENT_ENEMY_ANIMATION,
    EVENT_ENEMY_STATUS,
    EVENT_ENEMY_FADE,
} EventType;

typedef struct Event
{
    float duration;
    float elapsed;
    EventType type;

    union
    {
        struct
        {
            CombatantId id;
            Color color;
        } enemyTint;

        struct
        {
            CombatantId id;
            AnimationTag tag;
        } enemyAnimation;

        struct
        {
            CombatantId id;
        } enemyStatus;

        struct
        {
            CombatantId id;
        } enemyFade;

    } data;
} Event;

Event EventWait(float duration);

Event EventDamageFlash(CombatantId id);

Event EventSlashAnimation(CombatantId id);

Event EventDeathFade(CombatantId id);

Event EventEnemyStatus(CombatantId id);