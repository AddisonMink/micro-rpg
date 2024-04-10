#include "battle/battle.h"

#include "battle/action.h"
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

typedef struct BattleSelectTarget
{
    const Action *action;
    int itemIndexOpt;
    TargetList targets;
    float scrollCooldown;
} BattleSelectTarget;

typedef struct CompileEffects
{
    const Action *action;
    int itemIndexOpot;
    Id targetOpt;
} CompileEffects;

typedef struct Battle
{
    Combatant combatants[MAX_COMBATANTS];
    Queue queue;
    ItemList items;
    BattleState state;
} Battle;

static Battle battle;

void Battle_Init() {}

void Battle_Update(float delta) {}

void Battle_Draw(UI *ui) {}