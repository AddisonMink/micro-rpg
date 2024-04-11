#include "battle/battle.h"

#include "battle/action.h"
#include "battle/action-menu.h"
#include "battle/effect.h"
#include "battle/target.h"
#include "battle/queue.h"

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

void Battle_Init() {}

void Battle_Update(float delta)
{
    switch (battle.state)
    {
    case BATTLE_SELECT_ACTION:
    {
        ActionMenuResult result = ActionMenu_Update(delta);
        break;
    }

    default:
        break;
    }
}

void Battle_Draw(UI *ui) {}