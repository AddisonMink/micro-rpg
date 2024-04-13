#include "common-ui.h"

const float panelPadding = 10;
const FontId font = FONT_KONGTEXT;
const SpriteId panelSprite = SPRITE_NINESLICE;
const float meterSpacing = 2;

void UI_BodyLabel(UI *ui, const char *text)
{
    UI_Label(ui, Asset_Font(font), text, BODY_FONT_SIZE, RAYWHITE);
}

void UI_HeaderLabel(UI *ui, const char *text)
{
    UI_Label(ui, Asset_Font(font), text, HEADER_FONT_SIZE, RAYWHITE);
}

void UI_ScreenCenter(UI *ui)
{
    UI_AlignShim(ui, SCREEN_WIDTH, SCREEN_HEIGHT, ALIGN_H_CENTER, ALIGN_V_CENTER);
}

Vector2 UI_Panel(UI *ui, float width, float height)
{
    UI_Overlay(ui);
    UI_NineSlice(ui, Asset_Sprite(SPRITE_NINESLICE), 1, 4, width, height, WHITE);
    UI_PadSimple(ui, panelPadding);
    return (Vector2){width - panelPadding * 2, height - panelPadding * 2};
}

void UI_PanelEnd(UI *ui)
{
    UI_OverlayEnd(ui);
}

void UI_Meter(UI *ui, float width, float value, float max, Color color, Color bgColor)
{
    const float totalSpacing = (max - 1) * meterSpacing;
    const float pipWidth = (width - totalSpacing) / max;

    UI_Row(ui, meterSpacing);
    for (int i = 0; i < max; i++)
    {
        const Color c = i < value ? color : bgColor;
        UI_Rect(ui, pipWidth, BODY_FONT_SIZE, c);
    }
    UI_RowEnd(ui);
}

void UI_BodyMessage(UI *ui, const char *text)
{
    const Vector2 textSize = MeasureTextEx(Asset_Font(font), text, BODY_FONT_SIZE, 2);
    const float width = textSize.x + panelPadding * 2;
    const float height = textSize.y + panelPadding * 2;

    const Vector2 size = UI_Panel(ui, width, height);
    {
        UI_AlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UI_BodyLabel(ui, text);
    }
    UI_PanelEnd(ui);
}