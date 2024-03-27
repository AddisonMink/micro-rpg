#pragma once

#include "battle/action.h"

#define MAX_ITEM_ACTIONS 3

typedef enum ItemType
{
    ITEM_LODESTONE,
} ItemType;

typedef struct ItemData
{
    const char *name;
    int maxUses;
    int actionCount;
    ActionType actions[MAX_ITEM_ACTIONS];
} ItemData;

typedef struct Item
{
    // Metadata
    ItemType type;
    // State
    int uses;
} Item;

void ItemInit(Item *item, ItemType type);

const ItemData *ItemGetData(ItemType type);