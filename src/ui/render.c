#include "render.h"

#include "raymath.h"

typedef struct Origin
{
    ElementType type;
    Vector2 position;
} Origin;

static void drawNineSlice(const Element *element, Vector2 origin)
{
    origin = Vector2Add(origin, element->offsetFromParent);
    const int tileSize = element->nineSlice.tileSize;
    const int scale = element->nineSlice.scale;
    const Color tint = element->nineSlice.tint;
    const float destTileSize = tileSize * scale;
    const int middleSizeH = element->width - destTileSize * 2;
    const int middleSizeV = element->height - destTileSize * 2;
    const int middleX = origin.x + destTileSize;
    const int middleY = origin.y + destTileSize;
    const int rightX = origin.x + element->width - destTileSize;
    const int bottomY = origin.y + element->height - destTileSize;

    Rectangle topLeftSource = (Rectangle){0, 0, tileSize, tileSize};
    Rectangle topLeftDest = (Rectangle){origin.x, origin.y, destTileSize, destTileSize};
    DrawTexturePro(element->nineSlice.texture, topLeftSource, topLeftDest, (Vector2){0, 0}, 0, tint);

    Rectangle topMiddleSource = (Rectangle){tileSize, 0, tileSize, tileSize};
    Rectangle topMiddleDest = (Rectangle){middleX, origin.y, middleSizeH, destTileSize};
    DrawTexturePro(element->nineSlice.texture, topMiddleSource, topMiddleDest, (Vector2){0, 0}, 0, tint);

    Rectangle topRightSource = (Rectangle){tileSize * 2, 0, tileSize, tileSize};
    Rectangle topRightDest = (Rectangle){rightX, origin.y, destTileSize, destTileSize};
    DrawTexturePro(element->nineSlice.texture, topRightSource, topRightDest, (Vector2){0, 0}, 0, tint);

    Rectangle middleLeftSource = (Rectangle){0, tileSize, tileSize, tileSize};
    Rectangle middleLeftDest = (Rectangle){origin.x, middleY, destTileSize, middleSizeV};
    DrawTexturePro(element->nineSlice.texture, middleLeftSource, middleLeftDest, (Vector2){0, 0}, 0, tint);

    Rectangle middleMiddleSource = (Rectangle){tileSize, tileSize, tileSize, tileSize};
    Rectangle middleMiddleDest = (Rectangle){middleX, middleY, middleSizeH, middleSizeV};
    DrawTexturePro(element->nineSlice.texture, middleMiddleSource, middleMiddleDest, (Vector2){0, 0}, 0, tint);

    Rectangle middleRightSource = (Rectangle){tileSize * 2, tileSize, tileSize, tileSize};
    Rectangle middleRightDest = (Rectangle){rightX, middleY, destTileSize, middleSizeV};
    DrawTexturePro(element->nineSlice.texture, middleRightSource, middleRightDest, (Vector2){0, 0}, 0, tint);

    Rectangle bottomLeftSource = (Rectangle){0, tileSize * 2, tileSize, tileSize};
    Rectangle bottomLeftDest = (Rectangle){origin.x, bottomY, destTileSize, destTileSize};
    DrawTexturePro(element->nineSlice.texture, bottomLeftSource, bottomLeftDest, (Vector2){0, 0}, 0, tint);

    Rectangle bottomMiddleSource = (Rectangle){tileSize, tileSize * 2, tileSize, tileSize};
    Rectangle bottomMiddleDest = (Rectangle){middleX, bottomY, middleSizeH, destTileSize};
    DrawTexturePro(element->nineSlice.texture, bottomMiddleSource, bottomMiddleDest, (Vector2){0, 0}, 0, tint);

    Rectangle bottomRightSource = (Rectangle){tileSize * 2, tileSize * 2, tileSize, tileSize};
    Rectangle bottomRightDest = (Rectangle){rightX, bottomY, destTileSize, destTileSize};
    DrawTexturePro(element->nineSlice.texture, bottomRightSource, bottomRightDest, (Vector2){0, 0}, 0, tint);
}

static void drawElement(const Element *element, Vector2 origin)
{
    switch (element->type)
    {
    case E_RECT:
    {
        DrawRectangle(
            origin.x + element->offsetFromParent.x,
            origin.y + element->offsetFromParent.y,
            element->width,
            element->height,
            element->rect.color);
        break;
    }
    case E_RECT_OUTLINE:
    {
        DrawRectangleLinesEx(
            (Rectangle){
                origin.x + element->offsetFromParent.x,
                origin.y + element->offsetFromParent.y,
                element->width,
                element->height},
            element->rectOutline.thickness,
            element->rectOutline.color);
        break;
    }
    case E_SPRITE:
    {
        DrawTexturePro(
            element->sprite.texture,
            element->sprite.source,
            (Rectangle){
                origin.x + element->offsetFromParent.x,
                origin.y + element->offsetFromParent.y,
                element->width,
                element->height},
            (Vector2){0, 0},
            0,
            element->sprite.tint);
        break;
    }
    case E_NINE_SLICE:
    {
        drawNineSlice(element, origin);
        break;
    }
    case E_LABEL:
    {
        DrawTextEx(
            element->label.font,
            element->label.text,
            (Vector2){origin.x + element->offsetFromParent.x, origin.y + element->offsetFromParent.y},
            element->label.fontSize,
            2,
            element->label.color);
        break;
    }
    default:
        break;
    }
}

static bool isContainerComplete(ElementType container, ElementType child)
{
    switch (container)
    {
    case E_ROW:
        return child == E_ROW_END;
    case E_COL:
        return child == E_COL_END;
    case E_OVERLAY:
        return child == E_OVERLAY_END;
    default:
        return true;
    }
}

static void unwindStack(Origin *stack, int *top, ElementType type)
{
    while (*top > 0 && isContainerComplete(stack[*top].type, type))
    {
        (*top)--;
    }
}

void UIRender(const Element *elements, int count, Vector2 origin)
{
    Origin stack[count];
    int top = 0;
    stack[0] = (Origin){E_PAD, origin};

    for (int i = 0; i < count; i++)
    {
        const Element *element = &elements[i];
        drawElement(element, stack[top].position);
        if (element->type < FIRST_CONTAINER)
        {
            unwindStack(stack, &top, element->type);
        }
        else
        {
            Vector2 position = Vector2Add(stack[top].position, element->offsetFromParent);
            stack[++top] = (Origin){element->type, position};
        }
    }
}