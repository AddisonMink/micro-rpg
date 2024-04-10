#include "item.h"

typedef struct ItemData
{
    ItemType type;
    const char *name;
    int usesMax;
    ActionId actionIds[MAX_ACTIONS];
    int actionCount;
} ItemData;

static const ItemData itemData[] = {
    [ITEM_LODESTONE] = {
        .type = ITEM_LODESTONE,
        .name = "Lodestone",
        .usesMax = 3,
        .actionIds = {ACTION_LODESTONE_ZAP, ACTION_LODESTONE_PULL},
        .actionCount = 2,
    },
};

Item Item_Create(ItemType type)
{
    const ItemData *data = &itemData[type];

    Item item = {
        .type = type,
        .name = data->name,
        .uses = data->usesMax,
        .usesMax = data->usesMax,
    };

    item.actions.capacity = MAX_ACTIONS;
    item.actions.count = 0;
    for (int i = 0; i < data->actionCount; i++)
    {
        item.actions.data[i] = Action_Load(data->actionIds[i]);
        item.actions.count++;
    }

    return item;
}