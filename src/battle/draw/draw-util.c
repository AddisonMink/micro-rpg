#include "draw-util.h"

Vector2 UIPanel(UI *ui, float width, float height)
{
    Texture2D nineSlice = AssetSprite(SPRITE_NINESLICE);
    UIOverlay(ui);

    UINineSlice(ui, nineSlice, 1, 4, width, height, WHITE);

    UIPadSimple(ui, UI_PANEL_PAD);
    UICol(ui, 0);

    const float innerWidth = width - UI_PANEL_PAD * 2;
    const float innerHeight = height - UI_PANEL_PAD * 2;
    return (Vector2){innerWidth, innerHeight};
}

void UIPanelEnd(UI *ui)
{
    UIColEnd(ui);
    UIOverlayEnd(ui);
}