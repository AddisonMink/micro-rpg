#include "common-ui.h"

const int bodyFontSize = 12;
const int headerFontSize = 24;
const float panelPadding = 10;
const FontId font = FONT_KONGTEXT;
const SpriteId panelSprite = SPRITE_NINESLICE;

void UI_BodyLabel(UI *ui, const char *text)
{
    UI_Label(ui, Asset_Font(font), text, bodyFontSize, RAYWHITE);
}

void UI_HeaderLabel(UI *ui, const char *text)
{
    UI_Label(ui, Asset_Font(font), text, headerFontSize, RAYWHITE);
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