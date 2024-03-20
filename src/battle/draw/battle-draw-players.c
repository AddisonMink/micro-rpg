#include "battle-draw-players.h"

#include "../../asset.h"
#include "draw-util.h"

const int FONT_SIZE = 16;

const float HP_WIDTH = 90;
const float HP_HEIGHT = 16;

const float PORTRAIT_WIDTH = 128;
const float PORTAIT_HEIGHT = 152;

const float PANE_HEIGHT = 52;

const float PLAYER_SPACING = 20;
const float PLAYER_WIDTH = 118;
const float PLAYER_HEIGHT = 244;

static void uiStatusPane(UI *ui, const Combatant *player, const CombatantData *data)
{
    const Font font = AssetFont(FONT_TAG_KONGTEXT);
    UIShim(ui, PLAYER_WIDTH, PANE_HEIGHT + PLAYER_SPACING);
    const Vector2 innerSize = UIPanel(ui, PLAYER_WIDTH, PANE_HEIGHT);
    {
        UIAlignShimH(ui, innerSize.x, FONT_SIZE, ALIGN_H_CENTER);
        UILabel(ui, font, data->name, FONT_SIZE, RAYWHITE);

        UIAlignShimH(ui, innerSize.x, HP_HEIGHT, ALIGN_H_CENTER);
        UIMeter(ui, HP_WIDTH, HP_HEIGHT, player->hp, data->maxHp, MAROON);
    }
    UIPanelEnd(ui);
}

static void uiPlayer(UI *ui, const Combatant *player)
{
    const CombatantData *data = CombatantGetData(player->type);
    const Texture2D sprite = AssetSprite(data->sprite);
    const AlignV alignV = player->row == ROW_FRONT ? ALIGN_V_TOP : ALIGN_V_BOTTOM;

    UIAlignShimV(ui, PLAYER_WIDTH, PLAYER_HEIGHT, alignV);
    UICol(ui, 0);
    {
        UISprite(ui, sprite, PORTRAIT_WIDTH, PORTAIT_HEIGHT, WHITE);
        uiStatusPane(ui, player, data);
    }
    UIColEnd(ui);
}

void BattleDrawPlayers(UI *ui, const _Battle *battle)
{
    UIReset(ui);
    {
        UIShim(ui, SCREEN_WIDTH, SCREEN_HEIGHT);
        UIAlign(ui, ALIGN_H_CENTER, ALIGN_V_BOTTOM);
        UIRow(ui, PLAYER_SPACING);
        for (CombatantId id = 0; id < FIRST_ENEMY_ID; id++)
        {
            const Combatant *player = &battle->combatants[id];
            if (player->state != COMBATANT_STATE_INVALID)
                uiPlayer(ui, player);
        }
        UIRowEnd(ui);
    }
    UIDraw(ui, (Vector2){0, 0});
}