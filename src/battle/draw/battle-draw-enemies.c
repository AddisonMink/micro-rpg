#include "battle-draw-enemies.h"

#include "../../asset.h"

static const float ENEMY_SPRITE_WIDTH = 128;
static const float ENEMY_SPRITE_HEIGHT = 128;
static const float HP_HEIGHT = 16;
static const float HP_SPACING = 2;
static const float ENEMY_WIDTH = 128;

static void uiHpBar(UI *ui, const Combatant *enemy)
{
    const float numSpaces = enemy->maxHp - 1;
    const float totalSpace = numSpaces * HP_SPACING;
    const float pipWidth = (ENEMY_WIDTH - totalSpace) / enemy->maxHp;

    UIShim(ui, ENEMY_WIDTH, HP_HEIGHT);
    UIRow(ui, HP_SPACING);
    for (int i = 0; i < enemy->hp; i++)
    {
        UIRect(ui, pipWidth, HP_HEIGHT, RED);
    }
    UIRowEnd(ui);
}

static void uiEnemy(UI *ui, const Combatant *enemy)
{
    const Font font = AssetFont(FONT_TAG_KONGTEXT);
    const Color color = RAYWHITE;

    UICol(ui, 0);
    {
        UISprite(ui, enemy->sprite, ENEMY_SPRITE_WIDTH, ENEMY_SPRITE_HEIGHT, WHITE);
        uiHpBar(ui, enemy);
    }
    UIColEnd(ui);
}

void BattleDrawEnemies(UI *ui, const _Battle *battle)
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
                uiEnemy(ui, enemy);
        }
        UIRowEnd(ui);
    }
    UIDraw(ui, (Vector2){0, 0});
}