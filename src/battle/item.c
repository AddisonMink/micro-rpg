#include "item.h"

static const ItemData items[] = {
    [ITEM_LODESTONE] = {
        .name = "Lodestone",
        .maxUses = 3,
        .actionCount = 2,
        .actions = {ACTION_LODESTONE_HOLD, ACTION_LODESTONE_PULL},
    },
};

void ItemInit(Item *item, ItemType type)
{
    item->type = type;
    item->uses = ItemGetData(type)->maxUses;
}

const ItemData *ItemGetData(ItemType type)
{
    return &items[type];
}