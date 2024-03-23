#pragma once

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef enum FontTag
{
    FONT_KONGTEXT,
} FontTag;

typedef enum SpriteTag
{
    SPRITE_SCAMP,
    SPRITE_MAGICIAN,
    SPRITE_NINESLICE,
    SPRITE_POINTER,
    SPRITE_POINTER_DOWN,
    SPRITE_SLASH
} SpriteTag;

typedef struct Animation Animation;

typedef enum AnimationTag
{
    ANIMATION_SLASH,
} AnimationTag;

void AssetLoadFont(FontTag tag);

void AssetLoadSprite(SpriteTag tag);

Font AssetFont(FontTag tag);

Texture2D AssetSprite(SpriteTag tag);

const Animation *AssetAnimation(AnimationTag tag);

Texture2D AssetAnimationSprite(const Animation *animation);

Rectangle AssetAnimationFrame(const Animation *animation, float time);

bool AssetAnimationFinished(const Animation *animation, float time);

void AssetUnloadAll();