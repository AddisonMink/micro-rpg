#include "ui-player.h"

#include "raymath.h"

#include "battle/draw/util.h"

static const float spriteScale = 4.0f;
static const float rowMargin = 40.0f;
static const float panelHeight = 50.0f;
static const int fontSize = 16;

static void uiAnimation(UI *ui, Vector2 size, AnimationTag tag, float time)
{
    const Animation *animation = AssetAnimation(tag);
    const Texture2D sprite = AssetAnimationSprite(animation);
    const Rectangle frame = AssetAnimationFrame(animation, time);
    const float width = frame.width * spriteScale;
    const float height = frame.height * spriteScale;

    UIAlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
    UISpriteSlice(ui, sprite, frame, width, height, WHITE);
}

static void uiPortrait(UI *ui, Vector2 size, const Texture2D sprite, const PlayerDisplay *player)
{
    Color tint;
    if (player->option == PLAYER_DISPLAY_TINT)
        tint = player->optionData.tint.color;
    else if (player->option == PLAYER_DISPLAY_DEAD)
        tint = (Color){255, 255, 255, 0};
    else
        tint = WHITE;

    UIOverlay(ui);
    {
        UISprite(ui, sprite, size.x, size.y, tint);

        if (player->option == PLAYER_DISPLAY_ANIMATION)
        {
            const AnimationTag tag = player->optionData.animation.tag;
            const float time = player->optionData.animation.time;
            uiAnimation(ui, size, tag, time);
        }
    }
    UIOverlayEnd(ui);
}

static void uiStatusPane(UI *ui, Vector2 spriteSize, const PlayerDisplay *player)
{
    const Font font = AssetFont(FONT_KONGTEXT);

    const Vector2 innerSize = UIPanel(ui, spriteSize.x, panelHeight);
    {
        UIAlignShimH(ui, innerSize.x, fontSize, ALIGN_H_CENTER);
        UILabel(ui, font, player->name, fontSize, RAYWHITE);

        UIAlignShimH(ui, innerSize.x, fontSize, ALIGN_H_CENTER);
        UIMeter(ui, innerSize.x, fontSize, player->hp, player->maxHp, MAROON);
    }
    UIPanelEnd(ui);
}

void UIPlayer(UI *ui, const PlayerDisplay *player)
{
    const Texture2D sprite = AssetSprite(player->spriteTag);
    const Vector2 spriteSize = Vector2Scale((Vector2){sprite.width, sprite.height}, spriteScale);
    const float height = spriteSize.y + panelHeight + rowMargin;
    const AlignV alignV = player->row == ROW_FRONT ? ALIGN_V_TOP : ALIGN_V_BOTTOM;

    UIAlignShimV(ui, spriteSize.x, height, alignV);
    UICol(ui, 0);
    {
        uiPortrait(ui, spriteSize, sprite, player);
        uiStatusPane(ui, spriteSize, player);
    }
    UIColEnd(ui);
}