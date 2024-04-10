#include "compile.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static const char *ELEMENT_NAMES[] = {
    [E_RECT] = "E_RECT",
    [E_RECT_OUTLINE] = "E_RECT_OUTLINE",
    [E_SPRITE] = "E_SPRITE",
    [E_ROW_END] = "E_ROW_END",
    [E_COL_END] = "E_COL_END",
    [E_OVERLAY_END] = "E_OVERLAY_END",
    [E_PAD] = "E_PAD",
    [E_SHIM] = "E_SHIM",
    [E_OFFSET] = "E_OFFSET",
    [E_ROW] = "E_ROW",
    [E_COL] = "E_COL",
    [E_OVERLAY] = "E_OVERLAY",
    [E_ALIGN] = "E_ALIGN",
};

static void setAlignment(Element *elements, int elementsCount, int index)
{
    if (index == 0 || index == elementsCount - 1)
    {
        TraceLog(LOG_INFO, "UI: COMPILE: %s must have a parent and a child", ELEMENT_NAMES[E_ALIGN]);
        return;
    }

    Element *align = &elements[index];
    const Element *parent = &elements[index - 1];
    Element *child = &elements[index + 1];

    align->width = parent->width;
    align->height = parent->height;

    switch (align->align.alignH)
    {
    case ALIGN_H_LEFT:
        child->offsetFromParent.x = 0;
        break;
    case ALIGN_H_CENTER:
        child->offsetFromParent.x = (align->width - child->width) / 2;
        break;
    case ALIGN_H_RIGHT:
        child->offsetFromParent.x = align->width - child->width;
        break;
    }

    switch (align->align.alignV)
    {
    case ALIGN_V_TOP:
        child->offsetFromParent.y = 0;
        break;
    case ALIGN_V_CENTER:
        child->offsetFromParent.y = (align->height - child->height) / 2;
        break;
    case ALIGN_V_BOTTOM:
        child->offsetFromParent.y = align->height - child->height;
        break;
    }
}

static bool isContainerComplete(ElementType parent, ElementType child)
{
    switch (parent)
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

static void setContainerSizeAndChildOffset(Element *container, Element *child)
{
    switch (container->type)
    {
    case E_PAD:
    {
        child->offsetFromParent.x = container->pad.left;
        child->offsetFromParent.y = container->pad.top;
        container->width = child->width + container->pad.left + container->pad.right;
        container->height = child->height + container->pad.top + container->pad.bottom;
        break;
    }
    case E_SHIM:
    {
        container->width = MAX(container->shim.width, child->width);
        container->height = MAX(container->shim.height, child->height);
        break;
    }
    case E_OFFSET:
    {
        child->offsetFromParent.x = container->offset.offset.x;
        child->offsetFromParent.y = container->offset.offset.y;
        container->width = child->width;
        container->height = child->height;
        break;
    }
    case E_ROW:
    {
        if (child->type == E_ROW_END)
        {
            container->width -= container->col.spacing;
        }
        else
        {
            child->offsetFromParent.x = container->width;
            container->width += child->width + container->col.spacing;
            container->height = MAX(container->height, child->height);
        }
        break;
    }
    case E_COL:
    {
        if (child->type == E_COL_END)
        {
            container->height -= container->row.spacing;
        }
        else
        {
            child->offsetFromParent.y = container->height;
            container->height += child->height + container->row.spacing;
            container->width = MAX(container->width, child->width);
        }
        break;
    }
    case E_OVERLAY:
    {
        container->width = MAX(container->width, child->width);
        container->height = MAX(container->height, child->height);
        break;
    }
    // Align is special because it needs to know the size of both it's child and it's parent.
    case E_ALIGN:
        break;
    default:
    {
        TraceLog(LOG_INFO, "UI: COMPILE: Expected container type, got %s", ELEMENT_NAMES[container->type]);
        break;
    }
    }
}

static void unwindContainerStack(Element *stack[], int *top, Element *child)
{
    while (*top >= 0)
    {
        Element *parent = stack[*top];
        setContainerSizeAndChildOffset(parent, child);
        if (isContainerComplete(parent->type, child->type) && *top > 0)
        {
            (*top)--;
            child = parent;
            parent = stack[*top];
        }
        else
            break;
    }
}

static void setPrimitiveSize(Element *element)
{
    switch (element->type)
    {
    case E_RECT:
    {
        element->width = element->rect.width;
        element->height = element->rect.height;
        break;
    }
    case E_RECT_OUTLINE:
    {
        element->width = element->rectOutline.width;
        element->height = element->rectOutline.height;
        break;
    }
    case E_SPRITE:
    {
        element->width = element->sprite.width;
        element->height = element->sprite.height;
        break;
    }
    case E_NINE_SLICE:
    {
        element->width = element->nineSlice.width;
        element->height = element->nineSlice.height;
        break;
    }
    case E_LABEL:
    {
        Vector2 size = MeasureTextEx(element->label.font, element->label.text, element->label.fontSize, 2.0);
        element->width = size.x;
        element->height = size.y;
        break;
    }
    case E_ROW_END:
    case E_COL_END:
    case E_OVERLAY_END:
    {
        element->width = 0;
        element->height = 0;
        break;
    }
    default:
    {
        TraceLog(LOG_INFO, "UI: COMPILE: Expected primitive type, got %s", ELEMENT_NAMES[element->type]);
        break;
    }
    }
}

void UICompile(Element *elements, int count)
{
    Element *stack[count];
    int top = -1;

    int alignments[count];
    int alignmentCount = 0;

    for (int i = 0; i < count; i++)
    {
        Element *element = &elements[i];
        if (element->type < FIRST_CONTAINER)
        {
            setPrimitiveSize(element);
            unwindContainerStack(stack, &top, element);
        }
        else
        {
            stack[++top] = element;
            if (element->type == E_ALIGN)
            {
                alignments[alignmentCount++] = i;
            }
        }
    }

    for (int i = 0; i < alignmentCount; i++)
    {
        setAlignment(elements, count, alignments[i]);
    }

    // DEBUG
    /*
    for (int i = 0; i < count; i++)
    {
        Element *element = &elements[i];
        TraceLog(LOG_INFO, "UI: COMPILE: %s (%f, %f) %f %f", ELEMENT_NAMES[element->type], element->offsetFromParent.x, element->offsetFromParent.y, element->width, element->height);
    }
    */
}