#pragma once

#include "raylib.h"

typedef enum FontId
{
    FONT_KONGTEXT,
} FontId;

typedef enum SpriteId
{
    // UI Sprites
    SPRITE_NINESLICE,
    SPRITE_POINTER,
    SPRITE_POINTER_DOWN,

    // Enemy Sprites
    SPRITE_SCAMP,

    // Player Sprites
    SPRITE_MAGICIAN,
    SPRITE_GALOOT,

    // Animation Sprites
    SPRITE_SLASH,
    SPRITE_ZAP,

    // Background Sprites
    SPRITE_WALL,
    SPRITE_FLOOR,
} SpriteId;

typedef struct Animation Animation;

typedef enum AnimationId
{
    ANIMATION_SLASH,
    ANIMATION_ZAP,
} AnimationId;

void Asset_LoadFont(FontId tag);

void Asset_LoadSprite(SpriteId tag);

Font Asset_Font(FontId tag);

Texture2D Asset_Sprite(SpriteId tag);

const Animation *Asset_Animation(AnimationId tag);

float Asset_AnimationDuration(const Animation *animation);

Texture2D Asset_AnimationSprite(const Animation *animation);

Rectangle Asset_AnimationFrame(const Animation *animation, float time);

bool Asset_AnimationFinished(const Animation *animation, float time);

void Asset_UnloadAll();