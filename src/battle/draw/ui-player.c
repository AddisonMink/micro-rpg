#include "ui-player.h"

#include "raymath.h"

#include "battle/draw/util.h"

static const float spriteScale = 4.0f;
static const float rowMargin = 40.0f;
static const float panelHeight = 50.0f;
static const int fontSize = 16;

static void uiPortrait(UI *ui, Vector2 size, const Texture2D sprite)
{
    UISprite(ui, sprite, size.x, size.y, WHITE);
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
        uiPortrait(ui, spriteSize, sprite);
        uiStatusPane(ui, spriteSize, player);
    }
    UIColEnd(ui);
}