#pragma once

#include "battle/action.h"
#include "common/list-macros-new.h"

#define MAX_ITEMS 3

typedef enum ItemType
{
    ITEM_LODESTONE,
    ITEM_WAND,
} ItemType;

typedef struct Item
{
    ItemType type;
    const char *name;
    int usesMax;
    int uses;
    ActionRefList actions;
} Item;

STATIC_LIST_TYPE(ItemList, Item, MAX_ITEMS);

Item Item_Create(ItemType type);
