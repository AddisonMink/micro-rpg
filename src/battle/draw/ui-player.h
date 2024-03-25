#pragma once

#include "asset/asset.h"
#include "battle/types.h"
#include "ui/ui.h"

typedef struct PlayerDisplay
{
    SpriteTag spriteTag;
    const char *name;
    Row row;
    int hp;
    int maxHp;

    enum
    {
        PLAYER_DISPLAY_NONE,
        PLAYER_DISPLAY_DEAD,
        PLAYER_DISPLAY_TINT,
        PLAYER_DISPLAY_ANIMATION,
    } option;

    union
    {
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
} PlayerDisplay;

void UIPlayer(UI *ui, const PlayerDisplay *player);