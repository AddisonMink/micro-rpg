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
    const EventType type = id < FIRST_ENEMY_ID ? EVENT_PLAYER_TINT : EVENT_ENEMY_TINT;

    return (Event){
        .duration = 0.05f,
        .elapsed = 0,
        .type = type,
        .data.tint = {
            .id = id,
            .color = RED,
        },
    };
}

Event EventSlashAnimation(CombatantId id)
{
    const EventType type = id < FIRST_ENEMY_ID ? EVENT_PLAYER_ANIMATION : EVENT_ENEMY_ANIMATION;
    const Animation *animation = AssetAnimation(ANIMATION_SLASH);
    const float duration = AssetAnimationDuration(animation);

    return (Event){
        .duration = duration,
        .elapsed = 0,
        .type = type,
        .data.animation = {
            .id = id,
            .tag = ANIMATION_SLASH,
        },
    };
}

Event EventDeathFade(CombatantId id)
{
    const EventType type = id < FIRST_ENEMY_ID ? EVENT_PLAYER_FADE : EVENT_ENEMY_FADE;

    return (Event){
        .duration = 0.5f,
        .elapsed = 0,
        .type = type,
        .data.fade = {
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