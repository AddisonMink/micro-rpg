#include "action-menu.h"

#include "stddef.h"

#include "common/list-macros.h"

typedef enum ActionMenuState
{
    ACTION_MENU_SELECT_ITEM,
    ACTION_MENU_SELECT_ACTION,
} ActionMenuState;

typedef struct ItemActionList
{
    int itemIndex;
    const Action *data[MAX_ACTIONS];
    int capacity;
    int count;
    int index;
} ItemActionList;

typedef struct ActionMenuEntry
{
    const Item *item;
    ItemActionList actions;
    int index;
} ActionMenuEntry;

typedef struct ActionMenu
{
    const char *playerName;
    ActionMenuEntry data[MAX_ITEMS];
    int capacity;
    int count;
    int index;
    ActionMenuState state;
    float scrollCooldown;
} ActionMenu;

static ActionMenu menu;
static ActionMenuResult result;

void ActionMenu_Init(const ItemList *items, const Combatant *player)
{
    menu.playerName = player->name;
    menu.capacity = items->count;
    menu.count = 0;
    menu.index = 0;
    menu.state = ACTION_MENU_SELECT_ITEM;
    menu.scrollCooldown = 0;

    LIST_ITERATE(items)
    {
        const Item *item = &LIST_ELEM(items, i);

        ActionMenuEntry entry;
        entry.index = 0;
        entry.item = item;

        ItemActionList *actions = &entry.actions;
        actions->capacity = item->actions.capacity;
        actions->count = 0;
        actions->index = 0;

        LIST_ITERATE2((&item->actions))
        {
            const Action *action = LIST_ELEM((&item->actions), j);

            if (player->magic >= action->magicLevel)
            {
                LIST_APPEND(actions, action);
            }
        }

        LIST_APPEND((&menu), entry);
    }
}

#define SPACING 10
#define ACTION_TAB_WIDTH 150
#define ACTION_TAB_HEIGHT 30
#define ACTION_MENU_WIDTH (ACTION_TAB_WIDTH * 3 + SPACING * 2)
#define ACTION_MENU_HEIGHT 120
#define ACTION_LIST_WIDTH ((ACTION_MENU_WIDTH - SPACING) / 2)

static void ui_action_tab(UI *ui, const char *name)
{
    const Vector2 size = UI_Panel(ui, ACTION_TAB_WIDTH, ACTION_TAB_HEIGHT);
    {
        UI_AlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UI_BodyLabel(ui, name);
    }
    UI_PanelEnd(ui);
}

static void ui_effect_template(UI *ui, const EffectTemplate *effect)
{
    switch (effect->type)
    {
    case EFFECT_TEMPLATE_DAMAGE:
    {
        const int amount = effect->data.damage.amount;
        const DamageType type = effect->data.damage.damageType;
        const char *typeName = type == DAMAGE_PHYSICAL ? "" : "MAG ";
        UI_BodyLabel(ui, TextFormat("%d %sDamage", amount, typeName));
        break;
    }
    case EFFECT_TEMPLATE_KILL:
        UI_BodyLabel(ui, "Kill");
        break;
    case EFFECT_TEMPLATE_MOVE:
    {
        const Direction dir = effect->data.move.direction;
        const char *dirName = dir == DIRECTION_FORWARD ? "Forward" : "Backward";
        UI_BodyLabel(ui, TextFormat("Move %s", dirName));
        break;
    }
    case EFFECT_TEMPLATE_ATTACK:
    {
        const int type = effect->data.attack.damageType;
        const char *typeName = type == DAMAGE_PHYSICAL ? "" : "MAG ";
        UI_BodyLabel(ui, TextFormat("%sAttack", typeName));
        break;
    }
    case EFFECT_TEMPLATE_AUTO_MOVE:
        UI_BodyLabel(ui, "Move");
        break;
    }
}

static void ui_range(UI *ui, Range range)
{
    switch (range)
    {
    case RANGE_SELF:
        UI_BodyLabel(ui, "Self");
        break;
    case RANGE_MELEE:
        UI_BodyLabel(ui, "Melee");
        break;
    case RANGE_REACH:
        UI_BodyLabel(ui, "Reach");
        break;
    case RANGE_PROJECTILE:
        UI_BodyLabel(ui, "Projectile");
        break;
    }
}

static void ui_item_list(UI *ui)
{
    const Texture2D pointer = Asset_Sprite(SPRITE_POINTER);

    const Vector2 size = UI_Panel(ui, ACTION_LIST_WIDTH, ACTION_MENU_HEIGHT);
    {
        if (menu.count > 0)
        {
            UI_Col(ui, SPACING);

            UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
            UI_BodyLabel(ui, "ITEMS");

            LIST_ITERATE((&menu))
            {
                const ActionMenuEntry *entry = &LIST_ELEM((&menu), i);
                const Item *item = entry->item;

                UI_Row(ui, SPACING);
                {
                    if (entry->index == menu.index)
                    {
                        UI_Sprite(ui, pointer, pointer.width, pointer.height, WHITE);
                    }
                    else
                    {
                        UI_Rect(ui, pointer.width, pointer.height, BLANK);
                    }

                    UI_BodyLabel(ui, TextFormat("%s x%d", item->name, item->uses));
                }
                UI_RowEnd(ui);
            }
            UI_ColEnd(ui);
        }
        else
        {
            UI_AlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
            UI_BodyLabel(ui, "NO ITEMS");
        }
    }
    UI_PanelEnd(ui);
}

static void ui_action_list(UI *ui, bool showPointer)
{
    const Texture2D pointer = Asset_Sprite(SPRITE_POINTER);
    const ActionMenuEntry *entry = &LIST_ELEM((&menu), menu.index);

    const Vector2 size = UI_Panel(ui, ACTION_LIST_WIDTH, ACTION_MENU_HEIGHT);
    {
        if (menu.count > 0 && entry->actions.count > 0)
        {
            UI_Col(ui, SPACING);

            UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
            UI_BodyLabel(ui, "ACTIONS");

            LIST_ITERATE((&entry->actions))
            {
                const Action *action = LIST_ELEM((&entry->actions), i);

                UI_Row(ui, SPACING);
                {
                    if (entry->index == i && showPointer)
                    {
                        UI_Sprite(ui, pointer, pointer.width, pointer.height, WHITE);
                    }
                    else
                    {
                        UI_Rect(ui, pointer.width, pointer.height, BLANK);
                    }

                    UI_BodyLabel(ui, action->name);
                }
                UI_RowEnd(ui);
            }
            UI_ColEnd(ui);
        }
        else
        {
            UI_AlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
            UI_BodyLabel(ui, "NO ACTIONS");
        }
    }
    UI_PanelEnd(ui);
}

static void ui_action_desc(UI *ui)
{
    const ActionMenuEntry *entry = &LIST_ELEM((&menu), menu.index);
    const Action *action = LIST_ELEM((&entry->actions), entry->index);

    const Vector2 size = UI_Panel(ui, ACTION_LIST_WIDTH, ACTION_MENU_HEIGHT);
    {
        UI_Col(ui, SPACING);
        {
            UI_AlignShimH(ui, size.x, 12, ALIGN_H_CENTER);
            UI_BodyLabel(ui, action->name);

            if (action->cost > 0)
            {
                UI_AlignShimH(ui, size.x, 12, ALIGN_H_CENTER);
                UI_BodyLabel(ui, TextFormat("Cost %d", action->cost));
            }

            UI_AlignShimH(ui, size.x, 12, ALIGN_H_CENTER);
            ui_range(ui, action->range);

            LIST_ITERATE((&action->effects))
            {
                const EffectTemplate *effect = &LIST_ELEM((&action->effects), i);
                UI_AlignShimH(ui, size.x, 12, ALIGN_H_CENTER);
                ui_effect_template(ui, effect);
            }
        }
        UI_ColEnd(ui);
    }
    UI_PanelEnd(ui);
}

void ActionMenu_Draw(UI *ui)
{
    UI_Reset(ui);
    {
        UI_AlignShimH(ui, SCREEN_WIDTH, 0, ALIGN_H_CENTER);
        UI_Offset(ui, (Vector2){0, 20});
        UI_Col(ui, SPACING);
        {
            UI_AlignShimH(ui, ACTION_MENU_WIDTH, 24, ALIGN_H_CENTER);
            UI_HeaderLabel(ui, menu.playerName);

            UI_Row(ui, SPACING);
            {
                ui_action_tab(ui, "1.Attack");
                ui_action_tab(ui, "2.Move");
                ui_action_tab(ui, "3.Special");
            }
            UI_RowEnd(ui);

            UI_Row(ui, SPACING);
            {
                switch (menu.state)
                {
                case ACTION_MENU_SELECT_ITEM:
                {
                    ui_item_list(ui);
                    ui_action_list(ui, false);
                    break;
                }
                case ACTION_MENU_SELECT_ACTION:
                {
                    ui_action_list(ui, true);
                    ui_action_desc(ui);
                    break;
                }
                }
            }
            UI_RowEnd(ui);
        }
        UI_ColEnd(ui);
    }
    UI_Draw(ui, (Vector2){0, 0});
}

static const float scrollCooldown = 0.2f;

const ActionMenuResult *ActionMenu_Update(float delta)
{
    const bool itemsNonEmpty = !LIST_EMPTY((&menu));
    const bool actionsNonEmpty = itemsNonEmpty && !LIST_EMPTY((&LIST_ELEM((&menu), menu.index).actions));
    bool confirmed = false;

    if (menu.scrollCooldown > 0)
    {
        menu.scrollCooldown -= delta;
    }

    if (IsKeyDown(KEY_S) && menu.scrollCooldown <= 0)
    {
        if (menu.state == ACTION_MENU_SELECT_ITEM && itemsNonEmpty)
        {
            menu.index = (menu.index + 1) % menu.count;
            menu.scrollCooldown = scrollCooldown;
        }
        else if (menu.state == ACTION_MENU_SELECT_ACTION && actionsNonEmpty)
        {
            ActionMenuEntry *entry = &LIST_ELEM((&menu), menu.index);
            entry->index = (entry->index + 1) % entry->actions.count;
            menu.scrollCooldown = scrollCooldown;
        }
    }
    else if (IsKeyDown(KEY_W) && menu.scrollCooldown <= 0)
    {
        if (menu.state == ACTION_MENU_SELECT_ITEM && itemsNonEmpty)
        {
            menu.index = (menu.index - 1 + menu.count) % menu.count;
            menu.scrollCooldown = scrollCooldown;
        }
        else if (menu.state == ACTION_MENU_SELECT_ACTION && actionsNonEmpty)
        {
            ActionMenuEntry *entry = &LIST_ELEM((&menu), menu.index);
            entry->index = (entry->index - 1 + entry->actions.count) % entry->actions.count;
            menu.scrollCooldown = scrollCooldown;
        }
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        if (menu.state == ACTION_MENU_SELECT_ITEM && itemsNonEmpty && actionsNonEmpty)
        {
            menu.state = ACTION_MENU_SELECT_ACTION;
        }
        else if (menu.state == ACTION_MENU_SELECT_ACTION && actionsNonEmpty)
        {
            const ActionMenuEntry *entry = &menu.data[menu.index];
            const Action *action = entry->actions.data[entry->index];
            result = (ActionMenuResult){menu.index, action};
            confirmed = true;
        }
    }
    else if (IsKeyPressed(KEY_ONE))
    {
        const Action *action = Action_Load(ACTION_ATTACK);
        result = (ActionMenuResult){-1, action};
        confirmed = true;
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        const Action *action = Action_Load(ACTION_MOVE);
        result = (ActionMenuResult){-1, action};
        confirmed = true;
    }
    else if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE))
    {
        if (menu.state == ACTION_MENU_SELECT_ITEM)
        {
            const Action *action = Action_Load(ACTION_WAIT);
            result = (ActionMenuResult){-1, action};
            confirmed = true;
        }
        else
        {
            menu.state = ACTION_MENU_SELECT_ITEM;
        }
    }

    return confirmed ? &result : NULL;
}