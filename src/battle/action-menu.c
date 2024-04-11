#include "action-menu.h"

#include "common/list-macros.h"

#define MAX_ACTION_MENU_ENTRIES (MAX_ITEMS * MAX_ACTIONS)

typedef struct ActionMenuEntry
{
    int itemIndex;
    const char *itemName;
    int uses;
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
                .uses = item->uses,
                .action = action,
            };

            LIST_APPEND((&menu), entry);
        }
    }
}

#define SPACING 10
#define ACTION_TAB_WIDTH 140
#define ACTION_TAB_HEIGHT 30
#define ACTION_MENU_WIDTH (ACTION_TAB_WIDTH * 3 + SPACING * 2)
#define ACTION_MENU_HEIGHT 120
#define ACTION_LIST_WIDTH ((ACTION_MENU_WIDTH - SPACING) / 2)
#define ACTION_LIST_NAME_WIDTH 120
#define ACTION_LIST_COL_SPACING 10
#define ACTION_LIST_ROW_SPACING 8

static void ui_action_tab(UI *ui, const char *name)
{
    const Vector2 size = UI_Panel(ui, ACTION_TAB_WIDTH, ACTION_TAB_HEIGHT);
    {
        UI_AlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UI_BodyLabel(ui, name);
    }
    UI_PanelEnd(ui);
}

static void ui_action_list(UI *ui)
{
    const Texture2D pointer = Asset_Sprite(SPRITE_POINTER);

    const char *oldItemName = "";

    const Vector2 size = UI_Panel(ui, ACTION_LIST_WIDTH, ACTION_MENU_HEIGHT);
    {
        UI_Col(ui, ACTION_LIST_ROW_SPACING);
        LIST_ITERATE((&menu))
        {
            const ActionMenuEntry *entry = &LIST_ELEM((&menu), i);

            if (!TextIsEqual(oldItemName, entry->itemName))
            {

                UI_BodyLabel(ui, entry->itemName);
                oldItemName = entry->itemName;
            }

            UI_Row(ui, ACTION_LIST_COL_SPACING);
            {
                if (i == menu.index)
                    UI_Sprite(ui, pointer, pointer.width, pointer.height, WHITE);
                else
                    UI_Rect(ui, pointer.width, pointer.height, BLANK);

                UI_Shim(ui, ACTION_LIST_NAME_WIDTH, 12);
                UI_BodyLabel(ui, entry->action->name);

                UI_BodyLabel(ui, TextFormat("x%d", entry->uses));
            }
            UI_RowEnd(ui);
        }
        UI_ColEnd(ui);
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
    case EFFECT_TEMPLATE_USE_ITEM:
    {
        const int amount = effect->data.useItem.amount;
        UI_BodyLabel(ui, TextFormat("Use %d", amount));
        break;
    }
    case EFFECT_TEMPLATE_BREAK_ITEM:
        UI_BodyLabel(ui, "Break");
        break;
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

static void ui_action_desc(UI *ui)
{
    const Action *action = LIST_ELEM((&menu), menu.index).action;

    const Vector2 size = UI_Panel(ui, ACTION_LIST_WIDTH, ACTION_MENU_HEIGHT);
    {
        UI_Col(ui, ACTION_LIST_ROW_SPACING);
        {
            UI_AlignShimH(ui, size.x, 12, ALIGN_H_CENTER);
            UI_BodyLabel(ui, action->name);

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
                ui_action_list(ui);
                ui_action_desc(ui);
            }
            UI_RowEnd(ui);
        }
        UI_ColEnd(ui);
    }
    UI_Draw(ui, (Vector2){0, 0});
}

static const float scrollCooldown = 0.1;

ActionMenuResult ActionMenu_Update(float delta)
{
    ActionMenuResult result = {.confirmed = false};

    if (menu.scrollCooldown > 0)
        menu.scrollCooldown -= delta;

    if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE))
    {
        result.confirmed = true;
        result.itemIndexOpt = -1;
        result.action = Action_Load(ACTION_WAIT);
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        const ActionMenuEntry *entry = &LIST_ELEM((&menu), menu.index);
        result.confirmed = true;
        result.itemIndexOpt = entry->itemIndex;
        result.action = entry->action;
    }
    else if (IsKeyPressed(KEY_ONE))
    {
        result.confirmed = true;
        result.itemIndexOpt = -1;
        result.action = Action_Load(ACTION_ATTACK);
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        result.confirmed = true;
        result.itemIndexOpt = -1;
        result.action = Action_Load(ACTION_MOVE);
    }
    else if (IsKeyPressed(KEY_THREE))
    {
    }
    else if (IsKeyDown(KEY_S) && menu.scrollCooldown <= 0)
    {
        menu.index = (menu.index + 1) % menu.count;
        menu.scrollCooldown = scrollCooldown;
    }
    else if (IsKeyDown(KEY_W) && menu.scrollCooldown <= 0)
    {
        menu.index = (menu.index - 1 + menu.count) % menu.count;
        menu.scrollCooldown = scrollCooldown;
    }

    return result;
}
