#pragma once

#include "raylib.h"

#include "ui/align.h"

typedef struct UI UI;

UI *UIAlloc(int capacity);

void UIFree(UI *ui);

void UIReset(UI *ui);

void UIDraw(UI *ui, Vector2 origin);

void UIRect(UI *ui, float width, float height, Color color);

void UIRectOutline(UI *ui, float width, float height, Color color, float thickness);

void UISpriteSlice(UI *ui, Texture2D texture, Rectangle source, float width, float height, Color tint);

void UISprite(UI *ui, Texture2D texture, float width, float height, Color tint);

void UINineSlice(UI *ui, Texture2D texture, int scale, int tileSize, float width, float height, Color tint);

void UILabel(UI *ui, Font font, const char *text, float fontSize, Color color);

void UIRowEnd(UI *ui);

void UIColEnd(UI *ui);

void UIOverlayEnd(UI *ui);

void UIPad(UI *ui, float top, float right, float bottom, float left);

void UIPadSimple(UI *ui, float pad);

void UIShim(UI *ui, float width, float height);

void UIOffset(UI *ui, Vector2 offset);

void UIRow(UI *ui, float spacing);

void UICol(UI *ui, float spacing);

void UIOverlay(UI *ui);

void UIAlign(UI *ui, AlignH alignH, AlignV alignV);

void UIAlignH(UI *ui, AlignH alignH);

void UIAlignV(UI *ui, AlignV alignV);

void UIAlignShim(UI *ui, float width, float height, AlignH alignH, AlignV alignV);

void UIAlignShimH(UI *ui, float width, float height, AlignH alignH);

void UIAlignShimV(UI *ui, float width, float height, AlignV alignV);