#pragma once

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef enum FontTag
{
    FONT_TAG_KONGTEXT,
} FontTag;

typedef enum SpriteTag
{
    SPRITE_SCAMP,
    SPRITE_NINESLICE,
} SpriteTag;

void AssetLoadFont(FontTag tag);

void AssetLoadSprite(SpriteTag tag);

Font AssetFont(FontTag tag);

void AssetUnloadAll();

Texture2D AssetSprite(SpriteTag tag);
