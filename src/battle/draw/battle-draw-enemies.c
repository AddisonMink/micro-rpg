#include "battle-draw-enemies.h"

#include "battle/battle-types.h"
#include "battle/draw/ui-enemy.h"
#include "ui/ui.h"

static void initializeDisplays(EnemyDisplay displays[NUM_COMBATANTS], bool active[NUM_COMBATANTS], const _Battle *battle)
{
    for (CombatantId id = FIRST_ENEMY_ID; id < NUM_COMBATANTS; id++)
    {
        const Combatant *combatant = &battle->combatants[id];
        const CombatantData *data = CombatantGetData(combatant->type);
        if (combatant->state != COMBATANT_STATE_ALIVE)
        {
            active[id] = false;
            continue;
        }

        EnemyDisplay *display = &displays[id];
        display->spriteTag = data->sprite;
        display->row = combatant->row;
        display->option = ENEMY_DISPLAY_NONE;
        active[id] = true;
    }
}

static void showEnemyEvent(EnemyDisplay display[NUM_COMBATANTS], const _Battle *battle)
{
    const int eventIndex = battle->data.showEvents.eventIndex;
    const Event *event = &battle->data.showEvents.events[eventIndex];

    switch (event->type)
    {
    case EVENT_ENEMY_TINT:
    {
        const CombatantId id = event->data.enemyTint.id;
        const Color color = event->data.enemyTint.color;
        display[id].option = ENEMY_DISPLAY_TINT;
        display[id].optionData.tint.color = color;
        break;
    }
    case EVENT_ENEMY_ANIMATION:
    {
        const CombatantId id = event->data.enemyAnimation.id;
        const AnimationTag tag = event->data.enemyAnimation.tag;
        display[id].option = ENEMY_DISPLAY_ANIMATION;
        display[id].optionData.animation.tag = tag;
        display[id].optionData.animation.time = event->elapsed;
        break;
    }
    case EVENT_ENEMY_STATUS:
    {
        const CombatantId id = event->data.enemyStatus.id;
        const Combatant *combatant = &battle->combatants[id];
        const CombatantData *data = CombatantGetData(combatant->type);
        display[id].option = ENEMY_DISPLAY_STATUS;
        display[id].optionData.status = (EnemyDisplayStatus){
            .name = data->name,
            .hp = combatant->hp,
            .maxHp = data->maxHp,
        };
        break;
    }
    default:
        break;
    }
}

static void setDisplayOptions(EnemyDisplay display[NUM_COMBATANTS], const _Battle *battle)
{
    switch (battle->state)
    {
    case BATTLE_SELECT_ACTION:
    {
        if (IsKeyDown(KEY_TAB))
        {
            for (CombatantId id = FIRST_ENEMY_ID; id < NUM_COMBATANTS; id++)
            {
                const Combatant *combatant = &battle->combatants[id];
                const CombatantData *data = CombatantGetData(combatant->type);

                display[id].option = ENEMY_DISPLAY_STATUS;
                display[id].optionData.status = (EnemyDisplayStatus){
                    .name = data->name,
                    .hp = combatant->hp,
                    .maxHp = data->maxHp,
                };
            }
        }
        break;
    }
    case BATTLE_SELECT_TARGET:
    {
        const int index = battle->data.selectTarget.targetIndex;
        const CombatantId id = battle->data.selectTarget.targets[index];
        display[id].option = ENEMY_DISPLAY_SELECTED;
        break;
    }
    case BATTLE_SHOW_EVENTS:
        showEnemyEvent(display, battle);
        break;
    default:
        break;
    }
}

void BattleDrawEnemies(UI *ui, const _Battle *battle)
{
    static bool active[NUM_COMBATANTS];
    static EnemyDisplay displays[NUM_COMBATANTS];
    initializeDisplays(displays, active, battle);
    setDisplayOptions(displays, battle);

    UIReset(ui);
    {
        UIShim(ui, SCREEN_WIDTH, SCREEN_HEIGHT);
        UIAlign(ui, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UIRow(ui, 20);
        for (CombatantId id = FIRST_ENEMY_ID; id < NUM_COMBATANTS; id++)
        {
            if (active[id])
                UIEnemy(ui, &displays[id]);
        }
        UIRowEnd(ui);
    }
    UIDraw(ui, (Vector2){0, 0});
}