#include "draw-players.h"

#include "asset/asset.h"
#include "battle/draw/ui-player.h"
#include "battle/draw/util.h"

const float playerSpacing = 20;

static void initializeDisplays(PlayerDisplay displays[NUM_PLAYERS], bool active[NUM_PLAYERS], const _Battle *battle)
{
    for (CombatantId id = 0; id < FIRST_ENEMY_ID; id++)
    {
        const Combatant *combatant = &battle->combatants[id];
        const CombatantData *data = CombatantGetData(combatant->type);
        if (combatant->state == COMBATANT_STATE_INVALID)
        {
            active[id] = false;
            continue;
        }

        PlayerDisplay *display = &displays[id];
        display->spriteTag = data->sprite;
        display->name = data->name;
        display->row = combatant->row;
        display->hp = combatant->hp;
        display->maxHp = data->maxHp;
        display->option = combatant->state == COMBATANT_STATE_DEAD ? PLAYER_DISPLAY_DEAD : PLAYER_DISPLAY_NONE;
        active[id] = true;
    }
}

void BattleDrawPlayers(UI *ui, const _Battle *battle)
{
    static bool active[NUM_PLAYERS];
    static PlayerDisplay displays[NUM_PLAYERS];
    initializeDisplays(displays, active, battle);

    UIReset(ui);
    {
        UIShim(ui, SCREEN_WIDTH, SCREEN_HEIGHT);
        UIAlign(ui, ALIGN_H_CENTER, ALIGN_V_BOTTOM);
        UIRow(ui, playerSpacing);
        for (CombatantId id = 0; id < FIRST_ENEMY_ID; id++)
        {
            if (active[id])
                UIPlayer(ui, &displays[id]);
        }
        UIRowEnd(ui);
    }
    UIDraw(ui, (Vector2){0, 0});
}