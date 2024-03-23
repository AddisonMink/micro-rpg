#include "battle-draw-menu.h"

#include "../action.h"
#include "draw-util.h"

static const float MENU_MARGIN = 10;
static const int HEADER_SIZE = 16;
static const int FONT_SIZE = 12;

static const float SELECT_ACTION_PANEL_WIDTH = 200;
static const float SELECT_ACTION_PANEL_HEIGHT = 100;
static const float SELECT_ACTION_PANEL_SPACING = 20;
static const float SELECT_ACTION_POINTER_SPACING = 5;

static const float SELECT_TARGET_PANEL_WIDTH = 200;
static const float SELECT_TARGET_PANEL_HEIGHT = 50;

static void uiActionMenu(UI *ui, const _Battle *battle)
{
    const Font font = AssetFont(FONT_KONGTEXT);
    const Texture2D pointer = AssetSprite(SPRITE_POINTER);

    const int queueIndex = battle->data.selectAction.queueIndex;
    const int actionIndex = battle->data.selectAction.actionIndex;
    const CombatantId id = battle->queue[queueIndex];
    const CombatantType type = battle->combatants[id].type;
    const char *name = CombatantGetData(type)->name;
    const PlayerActions *actions = PlayerActionsGet(type);

    const Vector2 size = UIPanel(ui, SELECT_ACTION_PANEL_WIDTH, SELECT_ACTION_PANEL_HEIGHT);
    {
        UIAlignShimH(ui, size.x, HEADER_SIZE, ALIGN_H_CENTER);
        UILabel(ui, font, name, HEADER_SIZE, RAYWHITE);

        for (int i = 0; i < actions->count; i++)
        {
            const Action *action = ActionGet(actions->actions[i]);

            if (i == actionIndex)
            {
                UIRow(ui, SELECT_ACTION_POINTER_SPACING);
                UISprite(ui, pointer, pointer.width, pointer.height, WHITE);
                UILabel(ui, font, action->name, FONT_SIZE, RAYWHITE);
                UIRowEnd(ui);
            }
            else
            {
                UILabel(ui, font, action->name, FONT_SIZE, RAYWHITE);
            }
        }
    }
    UIPanelEnd(ui);
}

static void uiEffect(UI *ui, const Effect *effect, float maxWidth)
{
    const Font font = AssetFont(FONT_KONGTEXT);

    UIAlignShimH(ui, maxWidth, FONT_SIZE, ALIGN_H_CENTER);
    switch (effect->type)
    {
    case EFFECT_DAMAGE:
    {
        UILabel(ui, font, "Damage", FONT_SIZE, RAYWHITE);
        break;
    }
    case EFFECT_MOVE:
    {
        UILabel(ui, font, "Move", FONT_SIZE, RAYWHITE);
        break;
    }
    }
}

static void uiEffectList(UI *ui, const _Battle *battle)
{
    const Font font = AssetFont(FONT_KONGTEXT);

    const int queueIndex = battle->data.selectAction.queueIndex;
    const int actionIndex = battle->data.selectAction.actionIndex;
    const CombatantId id = battle->queue[queueIndex];
    const CombatantType type = battle->combatants[id].type;
    const PlayerActions *actions = PlayerActionsGet(type);
    const Action *action = ActionGet(actions->actions[actionIndex]);

    const Vector2 size = UIPanel(ui, SELECT_ACTION_PANEL_WIDTH, SELECT_ACTION_PANEL_HEIGHT);
    {
        UIAlignShimH(ui, size.x, HEADER_SIZE, ALIGN_H_CENTER);
        UILabel(ui, font, "Effects", HEADER_SIZE, RAYWHITE);

        for (int i = 0; i < action->effectCount; i++)
        {
            const Effect *effect = &action->effects[i];
            uiEffect(ui, effect, size.x);
        }
    }
    UIPanelEnd(ui);
}

static void drawSelectAction(UI *ui, const _Battle *battle)
{
    UIReset(ui);
    {
        UIAlignShim(ui, SCREEN_WIDTH, SELECT_ACTION_PANEL_HEIGHT + MENU_MARGIN, ALIGN_H_CENTER, ALIGN_V_BOTTOM);
        UIRow(ui, SELECT_ACTION_PANEL_SPACING);
        {
            uiActionMenu(ui, battle);
            uiEffectList(ui, battle);
        }
        UIRowEnd(ui);
    }
    UIDraw(ui, (Vector2){0, 0});
}

static void drawSelectTarget(UI *ui, const _Battle *battle)
{

    const Font font = AssetFont(FONT_KONGTEXT);

    UIReset(ui);
    {
        UIAlignShim(ui, SCREEN_WIDTH, SELECT_TARGET_PANEL_HEIGHT + MENU_MARGIN, ALIGN_H_CENTER, ALIGN_V_BOTTOM);
        const Vector2 size = UIPanel(ui, SELECT_TARGET_PANEL_WIDTH, SELECT_TARGET_PANEL_HEIGHT);
        {
            if (battle->data.selectTarget.targetCount > 0)
            {
                UIAlignShimH(ui, size.x, HEADER_SIZE, ALIGN_H_CENTER);
                UILabel(ui, font, "Select", FONT_SIZE, RAYWHITE);

                UIAlignShimH(ui, size.x, HEADER_SIZE, ALIGN_H_CENTER);
                UILabel(ui, font, "target", FONT_SIZE, RAYWHITE);
            }
            else
            {
                UIAlignShimH(ui, size.x, HEADER_SIZE, ALIGN_H_CENTER);
                UILabel(ui, font, "No targets!", FONT_SIZE, RAYWHITE);
            }
        }
        UIPanelEnd(ui);
    }
    UIDraw(ui, (Vector2){0, 0});
}

void BattleDrawMenu(UI *ui, const _Battle *battle)
{
    switch (battle->state)
    {
    case BATTLE_SELECT_ACTION:
        return drawSelectAction(ui, battle);
    case BATTLE_SELECT_TARGET:
        return drawSelectTarget(ui, battle);
    case BATTLE_EXECUTE_ACTION:
        return;
    case BATTLE_EXECUTE_EFFECTS:
        return;
    }
}