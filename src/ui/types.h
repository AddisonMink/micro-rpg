#pragma once

#include "raylib.h"


#include "ui/align.h"

typedef enum ElementType
{
    // Primitives
    E_RECT,
    E_RECT_OUTLINE,
    E_SPRITE,
    E_NINE_SLICE,
    E_LABEL,
    E_ROW_END,
    E_COL_END,
    E_OVERLAY_END,

    // Containers
    E_PAD,
    E_SHIM,
    E_OFFSET,
    E_ROW,
    E_COL,
    E_OVERLAY,
    E_ALIGN,
} ElementType;

typedef struct Element
{
    float width;
    float height;
    Vector2 offsetFromParent;
    ElementType type;

    union
    {
        struct
        {
            float width;
            float height;
            Color color;
        } rect;

        struct
        {
            float width;
            float height;
            Color color;
            float thickness;
        } rectOutline;

        struct
        {
            Texture2D texture;
            Rectangle source;
            float width;
            float height;
            Color tint;
        } sprite;

        struct
        {
            Texture2D texture;
            int scale;
            int tileSize;
            float width;
            float height;
            Color tint;
        } nineSlice;

        struct
        {
            Font font;
            char text[256];
            float fontSize;
            Color color;
        } label;

        struct
        {
            float top;
            float bottom;
            float left;
            float right;
        } pad;

        struct
        {
            float width;
            float height;
        } shim;

        struct
        {
            Vector2 offset;
        } offset;

        struct
        {
            float spacing;
        } row;

        struct
        {
            float spacing;
        } col;

        struct
        {
            AlignH alignH;
            AlignV alignV;
        } align;
    };
} Element;

#define FIRST_CONTAINER E_PAD