#pragma once

#include "raylib.h"

#include "ui/align.h"

typedef struct UI UI;

UI *UI_Alloc(int capacity);

void UI_Free(UI *ui);

void UI_Reset(UI *ui);

void UI_Draw(UI *ui, Vector2 origin);

void UI_Rect(UI *ui, float width, float height, Color color);

void UI_RectOutline(UI *ui, float width, float height, Color color, float thickness);

void UI_SpriteSlice(UI *ui, Texture2D texture, Rectangle source, float width, float height, Color tint);

void UI_Sprite(UI *ui, Texture2D texture, float width, float height, Color tint);

void UI_NineSlice(UI *ui, Texture2D texture, int scale, int tileSize, float width, float height, Color tint);

void UI_Label(UI *ui, Font font, const char *text, float fontSize, Color color);

void UI_RowEnd(UI *ui);

void UI_ColEnd(UI *ui);

void UI_OverlayEnd(UI *ui);

void UI_Pad(UI *ui, float top, float right, float bottom, float left);

void UI_PadSimple(UI *ui, float pad);

void UI_Shim(UI *ui, float width, float height);

void UI_Offset(UI *ui, Vector2 offset);

void UI_Row(UI *ui, float spacing);

void UI_Col(UI *ui, float spacing);

void UI_Overlay(UI *ui);

void UI_Align(UI *ui, AlignH alignH, AlignV alignV);

void UI_AlignH(UI *ui, AlignH alignH);

void UI_AlignV(UI *ui, AlignV alignV);

void UI_AlignShim(UI *ui, float width, float height, AlignH alignH, AlignV alignV);

void UI_AlignShimH(UI *ui, float width, float height, AlignH alignH);

void UI_AlignShimV(UI *ui, float width, float height, AlignV alignV);