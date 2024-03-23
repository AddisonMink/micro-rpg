#include "asset.h"

#define NUM_FONTS 1
#define NUM_SPRITES 6

typedef struct Animation
{
    SpriteTag spriteTag;
    int rows;
    int cols;
    int frameCount;
    Vector2 frameSize;
    float duration;
    bool loop;
} Animation;

static const char *fontPaths[NUM_FONTS] = {
    [FONT_KONGTEXT] = "assets/kongtext.ttf",
};

static const char *spritePaths[NUM_SPRITES] = {
    [SPRITE_SCAMP] = "assets/goblin_32.png",
    [SPRITE_MAGICIAN] = "assets/magician_32_38.png",
    [SPRITE_NINESLICE] = "assets/nineslice.png",
    [SPRITE_POINTER] = "assets/pointer_12.png",
    [SPRITE_POINTER_DOWN] = "assets/pointer_down_12.png",
    [SPRITE_SLASH] = "assets/slash_32.png",
};

const Animation animations[] = {
    [ANIMATION_SLASH] = {
        .spriteTag = SPRITE_SLASH,
        .rows = 2,
        .cols = 2,
        .frameCount = 4,
        .frameSize = (Vector2){32, 32},
        .duration = 0.5f,
        .loop = false,
    },
};

static Font fonts[NUM_FONTS];
static Texture2D sprites[NUM_SPRITES];
static bool fontsLoaded[NUM_FONTS] = {false};
static bool spritesLoaded[NUM_SPRITES] = {false, false};

void AssetLoadFont(FontTag tag)
{
    if (fontsLoaded[tag])
        return;
    fonts[tag] = LoadFont(fontPaths[tag]);
    fontsLoaded[tag] = true;
}

void AssetLoadSprite(SpriteTag tag)
{
    if (spritesLoaded[tag])
        return;
    sprites[tag] = LoadTexture(spritePaths[tag]);
    spritesLoaded[tag] = true;
}

Font AssetFont(FontTag tag)
{
    if (!fontsLoaded[tag])
        AssetLoadFont(tag);
    return fonts[tag];
}

Texture2D AssetSprite(SpriteTag tag)
{
    if (!spritesLoaded[tag])
        AssetLoadSprite(tag);
    return sprites[tag];
}

const Animation *AssetAnimation(AnimationTag tag)
{
    return &animations[tag];
}

Texture2D AssetAnimationSprite(const Animation *animation)
{
    return AssetSprite(animation->spriteTag);
}

Rectangle AssetAnimationFrame(const Animation *animation, float time)
{
    const int frame = (int)(time / animation->duration * animation->frameCount - 1);
    const int row = frame / animation->cols;
    const int col = frame % animation->cols;
    return (Rectangle){
        col * animation->frameSize.x,
        row * animation->frameSize.y,
        animation->frameSize.x,
        animation->frameSize.y,
    };
}

bool AssetAnimationFinished(const Animation *animation, float time)
{
    return !animation->loop && time >= animation->duration;
}

void AssetUnloadAll()
{
    for (int i = 0; i < NUM_FONTS; i++)
    {
        if (fontsLoaded[i])
        {
            UnloadFont(fonts[i]);
            fontsLoaded[i] = false;
        }
    }
    for (int i = 0; i < NUM_SPRITES; i++)
    {
        if (spritesLoaded[i])
        {
            UnloadTexture(sprites[i]);
            spritesLoaded[i] = false;
        }
    }
}