#include "ui.h"

#include "ui-types.h"
#include "ui-compile.h"
#include "ui-render.h"

typedef struct UI
{
    Element *elements;
    int capacity;
    int count;
} UI;

UI *UIAlloc(int capacity)
{
    UI *ui = MemAlloc(sizeof(UI));
    ui->elements = MemAlloc(sizeof(Element) * capacity);
    ui->capacity = capacity;
    ui->count = 0;
    return ui;
}

void UIFree(UI *ui)
{
    MemFree(ui->elements);
    MemFree(ui);
}

void UIReset(UI *ui)
{
    ui->count = 0;
}

void UIDraw(UI *ui, Vector2 origin)
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

void UIRect(UI *ui, float width, float height, Color color)
{
    Element element = {
        .type = E_RECT,
        .rect = {
            .width = width,
            .height = height,
            .color = color}};
    pushElement(ui, element);
}

void UIRectOutline(UI *ui, float width, float height, Color color, float thickness)
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

void UISpriteSlice(UI *ui, Texture2D texture, Rectangle source, float width, float height, Color tint)
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

void UISprite(UI *ui, Texture2D texture, float width, float height, Color tint)
{
    UISpriteSlice(ui, texture, (Rectangle){0, 0, texture.width, texture.height}, width, height, tint);
}

void UINineSlice(UI *ui, Texture2D texture, int scale, int tileSize, float width, float height, Color tint)
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

void UILabel(UI *ui, Font font, const char *text, float fontSize, Color color)
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

void UIRowEnd(UI *ui)
{
    Element element = {
        .type = E_ROW_END};
    pushElement(ui, element);
}

void UIColEnd(UI *ui)
{
    Element element = {
        .type = E_COL_END};
    pushElement(ui, element);
}

void UIOverlayEnd(UI *ui)
{
    Element element = {
        .type = E_OVERLAY_END};
    pushElement(ui, element);
}

void UIPad(UI *ui, float top, float right, float bottom, float left)
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

void UIPadSimple(UI *ui, float pad)
{
    UIPad(ui, pad, pad, pad, pad);
}

void UIShim(UI *ui, float width, float height)
{
    Element element = {
        .type = E_SHIM,
        .shim = {
            .width = width,
            .height = height}};
    pushElement(ui, element);
}

void UIOffset(UI *ui, Vector2 offset)
{
    Element element = {
        .type = E_OFFSET,
        .offset = {
            .offset = offset}};
    pushElement(ui, element);
}

void UIRow(UI *ui, float spacing)
{
    Element element = {
        .type = E_ROW,
        .row = {
            .spacing = spacing}};
    pushElement(ui, element);
}

void UICol(UI *ui, float spacing)
{
    Element element = {
        .type = E_COL,
        .col = {
            .spacing = spacing}};
    pushElement(ui, element);
}

void UIOverlay(UI *ui)
{
    Element element = {
        .type = E_OVERLAY};
    pushElement(ui, element);
}

void UIAlign(UI *ui, AlignH alignH, AlignV alignV)
{
    Element element = {
        .type = E_ALIGN,
        .align = {
            .alignH = alignH,
            .alignV = alignV}};
    pushElement(ui, element);
}

void UIAlignH(UI *ui, AlignH alignH)
{
    UIAlign(ui, alignH, ALIGN_V_TOP);
}

void UIAlignV(UI *ui, AlignV alignV)
{
    UIAlign(ui, ALIGN_H_LEFT, alignV);
}

void UIAlignShim(UI *ui, float width, float height, AlignH alignH, AlignV alignV)
{
    UIShim(ui, width, height);
    UIAlign(ui, alignH, alignV);
}

void UIAlignShimH(UI *ui, float width, float height, AlignH alignH)
{
    UIAlignShim(ui, width, height, alignH, ALIGN_V_TOP);
}

void UIAlignShimV(UI *ui, float width, float height, AlignV alignV)
{
    UIAlignShim(ui, width, height, ALIGN_H_LEFT, alignV);
}