#pragma once

#include "battle/combatant.h"

typedef enum EventType
{
    EVENT_WAIT,
    EVENT_ENEMY_TINT,
    EVENT_PLAYER_TINT,
    EVENT_ENEMY_ANIMATION,
    EVENT_PLAYER_ANIMATION,
    EVENT_ENEMY_STATUS,
    EVENT_ENEMY_FADE,
    EVENT_PLAYER_FADE,
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
        } tint;

        struct
        {
            CombatantId id;
            AnimationTag tag;
        } animation;

        struct
        {
            CombatantId id;
        } enemyStatus;

        struct
        {
            CombatantId id;
        } fade;

    } data;
} Event;

Event EventWait(float duration);

Event EventDamageFlash(CombatantId id);

Event EventSlashAnimation(CombatantId id);

Event EventDeathFade(CombatantId id);

Event EventEnemyStatus(CombatantId id);