#include "ui.h"

#include "ui/compile.h"
#include "ui/render.h"
#include "ui/types.h"

typedef struct UI
{
    Element *elements;
    int capacity;
    int count;
} UI;

UI *UI_Alloc(int capacity)
{
    UI *ui = MemAlloc(sizeof(UI));
    ui->elements = MemAlloc(sizeof(Element) * capacity);
    ui->capacity = capacity;
    ui->count = 0;
    return ui;
}

void UI_Free(UI *ui)
{
    MemFree(ui->elements);
    MemFree(ui);
}

void UI_Reset(UI *ui)
{
    ui->count = 0;
}

void UI_Draw(UI *ui, Vector2 origin)
{
    UICompile(ui->elements, ui->count);
    UIRender(ui->elements, ui->count, origin);
}

static void pushElement(UI *ui, Element element)
{
    element.width = 0;
    element.height = 0;
    element.offsetFromParent = (Vector2){0, 0};
    if (ui->count < ui->capacity)
    {
        ui->elements[ui->count++] = element;
    }
    else
    {
        TraceLog(LOG_INFO, "UI element overflow");
    }
}

void UI_Rect(UI *ui, float width, float height, Color color)
{
    Element element = {
        .type = E_RECT,
        .rect = {
            .width = width,
            .height = height,
            .color = color}};
    pushElement(ui, element);
}

void UI_RectOutline(UI *ui, float width, float height, Color color, float thickness)
{
    Element element = {
        .type = E_RECT_OUTLINE,
        .rectOutline = {
            .width = width,
            .height = height,
            .color = color,
            .thickness = thickness}};
    pushElement(ui, element);
}

void UI_SpriteSlice(UI *ui, Texture2D texture, Rectangle source, float width, float height, Color tint)
{
    Element element = {
        .type = E_SPRITE,
        .sprite = {
            .texture = texture,
            .source = source,
            .width = width,
            .height = height,
            .tint = tint,
        }};
    pushElement(ui, element);
}

void UI_Sprite(UI *ui, Texture2D texture, float width, float height, Color tint)
{
    UI_SpriteSlice(ui, texture, (Rectangle){0, 0, texture.width, texture.height}, width, height, tint);
}

void UI_NineSlice(UI *ui, Texture2D texture, int scale, int tileSize, float width, float height, Color tint)
{
    Element element = {
        .type = E_NINE_SLICE,
        .nineSlice = {
            .texture = texture,
            .scale = scale,
            .tileSize = tileSize,
            .width = width,
            .height = height,
            .tint = tint,
        }};
    pushElement(ui, element);
}

void UI_Label(UI *ui, Font font, const char *text, float fontSize, Color color)
{
    Element element = {
        .type = E_LABEL,
        .label = {
            .font = font,
            .fontSize = fontSize,
            .color = color,
        }};
    TextCopy(element.label.text, text);
    pushElement(ui, element);
}

void UI_RowEnd(UI *ui)
{
    Element element = {
        .type = E_ROW_END};
    pushElement(ui, element);
}

void UI_ColEnd(UI *ui)
{
    Element element = {
        .type = E_COL_END};
    pushElement(ui, element);
}

void UI_OverlayEnd(UI *ui)
{
    Element element = {
        .type = E_OVERLAY_END};
    pushElement(ui, element);
}

void UI_Pad(UI *ui, float top, float right, float bottom, float left)
{
    Element element = {
        .type = E_PAD,
        .pad = {
            .top = top,
            .right = right,
            .bottom = bottom,
            .left = left}};
    pushElement(ui, element);
}

void UI_PadSimple(UI *ui, float pad)
{
    UI_Pad(ui, pad, pad, pad, pad);
}

void UI_Shim(UI *ui, float width, float height)
{
    Element element = {
        .type = E_SHIM,
        .shim = {
            .width = width,
            .height = height}};
    pushElement(ui, element);
}

void UI_Offset(UI *ui, Vector2 offset)
{
    Element element = {
        .type = E_OFFSET,
        .offset = {
            .offset = offset}};
    pushElement(ui, element);
}

void UI_Row(UI *ui, float spacing)
{
    Element element = {
        .type = E_ROW,
        .row = {
            .spacing = spacing}};
    pushElement(ui, element);
}

void UI_Col(UI *ui, float spacing)
{
    Element element = {
        .type = E_COL,
        .col = {
            .spacing = spacing}};
    pushElement(ui, element);
}

void UI_Overlay(UI *ui)
{
    Element element = {
        .type = E_OVERLAY};
    pushElement(ui, element);
}

void UI_Align(UI *ui, AlignH alignH, AlignV alignV)
{
    Element element = {
        .type = E_ALIGN,
        .align = {
            .alignH = alignH,
            .alignV = alignV}};
    pushElement(ui, element);
}

void UI_AlignH(UI *ui, AlignH alignH)
{
    UI_Align(ui, alignH, ALIGN_V_TOP);
}

void UI_AlignV(UI *ui, AlignV alignV)
{
    UI_Align(ui, ALIGN_H_LEFT, alignV);
}

void UI_AlignShim(UI *ui, float width, float height, AlignH alignH, AlignV alignV)
{
    UI_Shim(ui, width, height);
    UI_Align(ui, alignH, alignV);
}

void UI_AlignShimH(UI *ui, float width, float height, AlignH alignH)
{
    UI_AlignShim(ui, width, height, alignH, ALIGN_V_TOP);
}

void UI_AlignShimV(UI *ui, float width, float height, AlignV alignV)
{
    UI_AlignShim(ui, width, height, ALIGN_H_LEFT, alignV);
}