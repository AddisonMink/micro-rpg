#include "battle-draw-enemies.h"

#include "raymath.h"
#include "../../asset.h"
#include "draw-util.h"

static const float SPRITE_SCALE = 4;

static const float FONT_SIZE = 16;

static const float HP_HEIGHT = 16;
static const float HP_WIDTH = 80;

static const float PANEL_PAD = 4;
static const float PANEL_WIDTH = 112;
static const float PANEL_HEIGHT = 52;

static const float ENEMY_WIDTH = 128;
static const float ENEMY_HEIGHT = 128;

static void uiEnemySprite(UI *ui, const Combatant *enemy, Texture2D sprite, Vector2 size)
{
    const Vector2 spriteSize = enemy->row == ROW_FRONT ? size : Vector2Scale(size, 0.5);

    UIAlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
    UISprite(ui, sprite, spriteSize.x, spriteSize.y, WHITE);
}

static void uiStatusPanel(UI *ui, const Combatant *enemy, const CombatantData *data, Vector2 size)
{
    const Font font = AssetFont(FONT_TAG_KONGTEXT);

    UIAlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
    const Vector2 innerSize = UIPanel(ui, PANEL_WIDTH, PANEL_HEIGHT);
    {
        UIAlignShimH(ui, innerSize.x, FONT_SIZE, ALIGN_H_CENTER);
        UILabel(ui, font, data->name, FONT_SIZE, RAYWHITE);

        UIAlignShimH(ui, innerSize.x, HP_HEIGHT, ALIGN_H_CENTER);
        UIMeter(ui, HP_WIDTH, HP_HEIGHT, enemy->hp, data->maxHp, MAROON);
    }
    UIPanelEnd(ui);
}

static void uiEnemy(UI *ui, const Combatant *enemy, DrawEnemyOptions options)
{
    const CombatantData *data = CombatantGetData(enemy->type);
    const Texture2D sprite = AssetSprite(data->sprite);
    const Vector2 size = Vector2Scale((Vector2){sprite.width, sprite.height}, SPRITE_SCALE);

    UIOverlay(ui);
    {
        uiEnemySprite(ui, enemy, sprite, size);
        if (options.showStatusPane)
            uiStatusPanel(ui, enemy, data, size);
    }
    UIOverlayEnd(ui);
}

void BattleDrawEnemies(UI *ui, const _Battle *battle, DrawEnemyOptions options)
{
    UIReset(ui);
    {
        UIShim(ui, SCREEN_WIDTH, SCREEN_HEIGHT);
        UIAlign(ui, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UIRow(ui, 20);
        for (CombatantId id = FIRST_ENEMY_ID; id < NUM_COMBATANTS; id++)
        {
            const Combatant *enemy = &battle->combatants[id];
            if (enemy->state == COMBATANT_STATE_ALIVE)
                uiEnemy(ui, enemy, options);
        }
        UIRowEnd(ui);
    }
    UIDraw(ui, (Vector2){0, 0});
}