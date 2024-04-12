#include "battle/battle.h"

#include "stddef.h"

#include "battle/action.h"
#include "battle/action-menu.h"
#include "battle/effect.h"
#include "battle/enemy-display.h"
#include "battle/player-display.h"
#include "battle/target.h"
#include "battle/queue.h"
#include "common/list-macros.h"

typedef enum BattleState
{
    BATTLE_SELECT_ACTION,
    BATTLE_SELECT_TARGET,
    BATTLE_COMPILE_EFFECTS,
    BATTLE_EXECUTE_EFFECTS,
    BATTLE_SHOW_EVENTS,
    BATTLE_END_TURN,
    BATTLE_ENEMY_TURN,
    BATTLE_WIN,
    BATTLE_LOSE,
} BattleState;

typedef struct SelectTarget
{
    const Action *action;
    int itemIndexOpt;
    TargetList targets;
    float scrollCooldown;
} SelectTarget;

typedef struct CompileEffects
{
    const Action *action;
    int itemIndexOpot;
    Id targetOpt;
} CompileEffects;

typedef struct ExecuteEffects
{
    EffectList effects;
} ExecuteEffects;

typedef struct ShowEvents
{
    EffectList effects;
    EventList events;
} ShowEvents;

typedef struct Battle
{
    Combatant combatants[MAX_COMBATANTS];
    Queue queue;
    ItemList items;
    BattleState state;
    union
    {
        // selectAction;
        SelectTarget selectTarget;
        CompileEffects compileEffects;
        ExecuteEffects executeEffects;
        ShowEvents showEvents;
        // endTurn;
        // enemyTurn;
        // win;
        // lose;
    } data;
} Battle;

static Battle battle;

void Battle_Init()
{
    battle.combatants[0] = Combatant_Create(0, COMBATANT_TYPE_MAGICIAN, ROW_BACK);
    battle.combatants[1] = Combatant_Create(1, COMBATANT_TYPE_GALOOT, ROW_FRONT);
    battle.combatants[FIRST_ENEMY_ID] = Combatant_Create(FIRST_ENEMY_ID, COMBATANT_TYPE_SCAMP, ROW_FRONT);
    battle.combatants[FIRST_ENEMY_ID + 1] = Combatant_Create(FIRST_ENEMY_ID + 1, COMBATANT_TYPE_SCAMP, ROW_BACK);

    battle.queue = Queue_Create(battle.combatants);

    battle.items = (ItemList)LIST_INIT(MAX_ITEMS);
    LIST_APPEND((&battle.items), Item_Create(ITEM_LODESTONE));

    battle.state = BATTLE_SELECT_ACTION;
    const Id id = battle.queue.data[battle.queue.index];
    ActionMenu_Init(&battle.items, &battle.combatants[id]);
}

void Battle_Update(float delta)
{
    switch (battle.state)
    {
    case BATTLE_SELECT_ACTION:
    {
        ActionMenu_Update(delta);
        break;
    }

    default:
        break;
    }
}

void Battle_Draw(UI *ui)
{
    switch (battle.state)
    {
    case BATTLE_SELECT_ACTION:
    {
        const bool showEnemyStatus = IsKeyDown(KEY_TAB);
        EnemyDisplay_Draw(ui, battle.combatants, -1, NULL, showEnemyStatus);
        PlayerDisplay_Draw(ui, battle.combatants, -1, NULL);
        ActionMenu_Draw(ui);
        break;
    }

    default:
        break;
    }
}