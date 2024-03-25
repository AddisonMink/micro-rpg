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

static void showPlayerEvent(PlayerDisplay displays[NUM_PLAYERS], const Event *event)
{
    switch (event->type)
    {
    case EVENT_PLAYER_TINT:
    {
        const CombatantId id = event->data.tint.id;
        const Color color = event->data.tint.color;
        displays[id].option = PLAYER_DISPLAY_TINT;
        displays[id].optionData.tint.color = color;
        break;
    }
    case EVENT_PLAYER_ANIMATION:
    {
        const CombatantId id = event->data.animation.id;
        const AnimationTag tag = event->data.animation.tag;
        const float time = event->elapsed;

        displays[id].option = PLAYER_DISPLAY_ANIMATION;
        displays[id].optionData.animation.tag = tag;
        displays[id].optionData.animation.time = time;
        break;
    }
    case EVENT_PLAYER_FADE:
    {
        const CombatantId id = event->data.fade.id;
        const float time = event->elapsed;
        const float duration = event->duration;
        const int alpha = 255 - (int)(255 * time / duration);

        displays[id].option = PLAYER_DISPLAY_TINT;
        displays[id].optionData.tint.color = (Color){255, 255, 255, alpha};
        break;
    }
    default:
        break;
    }
}

static void setDisplayOptions(PlayerDisplay displays[NUM_PLAYERS], const _Battle *battle)
{
    switch (battle->state)
    {
    case BATTLE_SHOW_EVENTS:
    {
        const int eventIndex = battle->data.showEvents.eventIndex;
        const Event *event = &battle->events[eventIndex];
        showPlayerEvent(displays, event);
        break;
    }
    default:
        break;
    }
}

void BattleDrawPlayers(UI *ui, const _Battle *battle)
{
    static bool active[NUM_PLAYERS];
    static PlayerDisplay displays[NUM_PLAYERS];
    initializeDisplays(displays, active, battle);
    setDisplayOptions(displays, battle);

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