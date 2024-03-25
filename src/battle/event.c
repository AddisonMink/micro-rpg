#include "event.h"

#include "asset/asset.h"

Event EventWait(float duration)
{
    return (Event){
        .duration = duration,
        .elapsed = 0,
        .type = EVENT_WAIT,
    };
}

Event EventDamageFlash(CombatantId id)
{
    return (Event){
        .duration = 0.05f,
        .elapsed = 0,
        .type = EVENT_ENEMY_TINT,
        .data.enemyTint = {
            .id = id,
            .color = RED,
        },
    };
}

Event EventSlashAnimation(CombatantId id)
{
    return (Event){
        .duration = AssetAnimationDuration(AssetAnimation(ANIMATION_SLASH)),
        .elapsed = 0,
        .type = EVENT_ENEMY_ANIMATION,
        .data.enemyAnimation = {
            .id = id,
            .tag = ANIMATION_SLASH,
        },
    };
}

Event EventDeathFade(CombatantId id)
{
    return (Event){
        .duration = 0.5f,
        .elapsed = 0,
        .type = EVENT_ENEMY_FADE,
        .data.enemyFade = {
            .id = id,
        },
    };
}

Event EventEnemyStatus(CombatantId id)
{
    return (Event){
        .duration = 0.5f,
        .elapsed = 0,
        .type = EVENT_ENEMY_STATUS,
        .data.enemyStatus = {
            .id = id,
        },
    };
}