#pragma once

#include "battle/types.h"
#include "ui/ui.h"

typedef struct EnemyDisplayStatus
{
    const char *name;
    int hp;
    int maxHp;
} EnemyDisplayStatus;

typedef struct EnemyDisplay
{
    SpriteTag spriteTag;
    Row row;

    enum
    {
        ENEMY_DISPLAY_NONE,
        ENEMY_DISPLAY_DEAD,
        ENEMY_DISPLAY_SELECTED,
        ENEMY_DISPLAY_STATUS,
        ENEMY_DISPLAY_TINT,
        ENEMY_DISPLAY_ANIMATION,
    } option;

    union
    {
        EnemyDisplayStatus status;

        struct
        {
            Color color;
        } tint;

        struct
        {
            AnimationTag tag;
            float time;
        } animation;
    } optionData;
} EnemyDisplay;

void UIEnemy(UI *ui, const EnemyDisplay *display);