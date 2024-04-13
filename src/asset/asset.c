#include "asset.h"

static const int numFonts = 1;
static const int numSprites = 8;

typedef struct Animation
{
    SpriteId spriteTag;
    int rows;
    int cols;
    int frameCount;
    Vector2 frameSize;
    float duration;
    bool loop;
} Animation;

static const char *fontPaths[numFonts] = {
    [FONT_KONGTEXT] = "assets/kongtext.ttf",
};

static const char *spritePaths[numSprites] = {
    [SPRITE_NINESLICE] = "assets/nineslice.png",
    [SPRITE_POINTER] = "assets/pointer_12.png",
    [SPRITE_POINTER_DOWN] = "assets/pointer_down_12.png",
    [SPRITE_SCAMP] = "assets/goblin_32.png",
    [SPRITE_MAGICIAN] = "assets/magician_32_38.png",
    [SPRITE_GALOOT] = "assets/galoot_32_38.png",
    [SPRITE_SLASH] = "assets/slash_32.png",
    [SPRITE_ZAP] = "assets/lightning_32.png",
};

const Animation animations[] = {
    [ANIMATION_SLASH] = {
        .spriteTag = SPRITE_SLASH,
        .rows = 2,
        .cols = 2,
        .frameCount = 4,
        .frameSize = {32, 32},
        .duration = 0.2,
        .loop = false,
    },
    [ANIMATION_ZAP] = {
        .spriteTag = SPRITE_ZAP,
        .rows = 2,
        .cols = 2,
        .frameCount = 4,
        .frameSize = {32, 32},
        .duration = 0.2,
        .loop = false,
    },
};

static Font fonts[numFonts];
static Texture2D sprites[numSprites];
static bool fontsLoaded[numFonts] = {false};
static bool spritesLoaded[numSprites] = {false};

void Asset_LoadFont(FontId tag)
{
    if (fontsLoaded[tag])
        return;
    fonts[tag] = LoadFont(fontPaths[tag]);
    fontsLoaded[tag] = true;
}

void Asset_LoadSprite(SpriteId tag)
{
    if (spritesLoaded[tag])
        return;
    sprites[tag] = LoadTexture(spritePaths[tag]);
    spritesLoaded[tag] = true;
}

Font Asset_Font(FontId tag)
{
    if (!fontsLoaded[tag])
        Asset_LoadFont(tag);
    return fonts[tag];
}

Texture2D Asset_Sprite(SpriteId tag)
{
    if (!spritesLoaded[tag])
        Asset_LoadSprite(tag);
    return sprites[tag];
}

const Animation *Asset_Animation(AnimationId tag)
{
    return &animations[tag];
}

float Asset_AnimationDuration(const Animation *animation)
{
    return animation->duration;
}

Texture2D Asset_AnimationSprite(const Animation *animation)
{
    return Asset_Sprite(animation->spriteTag);
}

Rectangle Asset_AnimationFrame(const Animation *animation, float time)
{
    const int frame = (int)(time / (animation->duration / animation->frameCount));
    const int row = frame / animation->cols;
    const int col = frame % animation->cols;
    return (Rectangle){
        col * animation->frameSize.x,
        row * animation->frameSize.y,
        animation->frameSize.x,
        animation->frameSize.y,
    };
}

bool Asset_AnimationFinished(const Animation *animation, float time)
{
    return !animation->loop && time >= animation->duration;
}

void Asset_UnloadAll()
{
    for (int i = 0; i < numFonts; i++)
    {
        if (fontsLoaded[i])
        {
            UnloadFont(fonts[i]);
            fontsLoaded[i] = false;
        }
    }
    for (int i = 0; i < numSprites; i++)
    {
        if (spritesLoaded[i])
        {
            UnloadTexture(sprites[i]);
            spritesLoaded[i] = false;
        }
    }
}