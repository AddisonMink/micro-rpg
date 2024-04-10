#include "action-menu.h"

#include "common/list-macros.h"

#define MAX_ACTION_MENU_ENTRIES (MAX_ITEMS * MAX_ACTIONS)

typedef struct ActionMenuEntry
{
    int itemIndex;
    const char *itemName;
    const Action *action;
} ActionMenuEntry;

typedef struct ActionMenu
{
    const char *playerName;
    ActionMenuEntry data[MAX_ACTION_MENU_ENTRIES];
    int capacity;
    int count;
    int index;
    float scrollCooldown;
} ActionMenu;

static ActionMenu menu;
static bool initialized = false;

void ActionMenu_Init(const ItemList *items, const Combatant *player)
{
    menu.playerName = player->name;
    menu.capacity = MAX_ACTION_MENU_ENTRIES;
    menu.count = 0;
    menu.index = 0;
    menu.scrollCooldown = 0;

    LIST_ITERATE((items))
    {
        const Item *item = &LIST_ELEM(items, i);
        LIST_ITERATE2((&item->actions))
        {
            const Action *action = LIST_ELEM((&item->actions), j);
            if (action->magicLevel > player->magic)
                continue;

            ActionMenuEntry entry = {
                .itemIndex = i,
                .itemName = item->name,
                .action = action,
            };

            LIST_APPEND((&menu), entry);
        }
    }
}

void ActionMenu_Draw(UI *ui)
{
    UI_Reset(ui);
    {
        UI_BodyLabel(ui, "Choose an action.");
    }
    UI_Draw(ui, (Vector2){0, 0});
}

ActionMenuResult ActionMenu_Update(float delta)
{
    return (ActionMenuResult){0};
}
