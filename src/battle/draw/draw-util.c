#include "draw-util.h"

Vector2 UIPanel(UI *ui, float width, float height)
{
    const float innerWidth = width - UI_PANEL_PAD * 2;
    const float innerHeight = height - UI_PANEL_PAD * 2;

    Texture2D nineSlice = AssetSprite(SPRITE_NINESLICE);
    UIOverlay(ui);

    UINineSlice(ui, nineSlice, 1, 4, width, height, WHITE);

    UIPadSimple(ui, UI_PANEL_PAD);
    UIAlignShimV(ui, innerWidth, innerHeight, ALIGN_V_CENTER);
    UICol(ui, UI_PANEL_SPACING);

    return (Vector2){innerWidth, innerHeight};
}

void UIPanelEnd(UI *ui)
{
    UIColEnd(ui);
    UIOverlayEnd(ui);
}

void UIMeter(UI *ui, float width, float height, int current, int max, Color color)
{
    const float totalSpacing = (max - 1) * 2;
    const float pipWidth = (width - totalSpacing) / max;

    UIRow(ui, 2);
    for (int i = 0; i < max; i++)
    {
        const Color c = (i < current) ? color : GRAY;
        UIRect(ui, pipWidth, height, c);
    }
    UIRowEnd(ui);
}