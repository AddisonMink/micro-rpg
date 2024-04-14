#pragma once

#include "action.h"

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
    ActionList actions;
} Item;

typedef struct ItemList
{
    Item data[MAX_ITEMS];
    int capacity;
    int count;
} ItemList;

Item Item_Create(ItemType type);
