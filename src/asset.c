#include "asset.h"

#define NUM_FONTS 1
#define NUM_SPRITES 4

static const char *fontPaths[NUM_FONTS] = {
    [FONT_TAG_KONGTEXT] = "assets/kongtext.ttf",
};

static const char *spritePaths[NUM_SPRITES] = {
    [SPRITE_SCAMP] = "assets/goblin_32.png",
    [SPRITE_MAGICIAN] = "assets/magician_32_38.png",
    [SPRITE_NINESLICE] = "assets/nineslice.png",
    [SPRITE_POINTER] = "assets/pointer_12.png",
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