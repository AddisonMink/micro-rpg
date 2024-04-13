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
    int itemIndexOpt;
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
        const ActionMenuResult *result = ActionMenu_Update(delta);
        if (result != NULL)
        {
            const Id id = battle.queue.data[battle.queue.index];

            if (result->action->range == RANGE_SELF)
            {
                battle.state = BATTLE_COMPILE_EFFECTS;
                battle.data.compileEffects = (CompileEffects){
                    .action = result->action,
                    .itemIndexOpt = result->itemIndexOpt,
                    .targetOpt = id,
                };
            }
            else
            {
                battle.state = BATTLE_SELECT_TARGET;
                battle.data.selectTarget = (SelectTarget){
                    .action = result->action,
                    .itemIndexOpt = result->itemIndexOpt,
                    .targets = TargetList_Create(result->action, battle.combatants, id),
                    .scrollCooldown = 0.0f,
                };
            }
        }
        break;
    }
    case BATTLE_SELECT_TARGET:
    {
        SelectTarget *data = &battle.data.selectTarget;
        if (data->scrollCooldown > 0)
        {
            data->scrollCooldown -= delta;
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE))
        {
            battle.state = BATTLE_SELECT_ACTION;
            ActionMenu_Init(&battle.items, &battle.combatants[battle.queue.data[battle.queue.index]]);
            break;
        }
        else if (IsKeyDown(KEY_D) && data->scrollCooldown <= 0 && data->targets.count > 0)
        {
            data->targets.index = (data->targets.index + 1) % data->targets.count;
            data->scrollCooldown = 0.2f;
        }
        else if (IsKeyDown(KEY_A) && data->scrollCooldown <= 0 && data->targets.count > 0)
        {
            data->targets.index = (data->targets.index - 1 + data->targets.count) % data->targets.count;
            data->scrollCooldown = 0.2f;
        }
        else if (IsKeyPressed(KEY_ENTER) && data->targets.count > 0)
        {
            battle.state = BATTLE_COMPILE_EFFECTS;
            battle.data.compileEffects = (CompileEffects){
                .action = data->action,
                .itemIndexOpt = data->itemIndexOpt,
                .targetOpt = LIST_ELEM((&data->targets), data->targets.index),
            };
        }
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
    case BATTLE_SELECT_TARGET:
    {
        const TargetList *targets = &battle.data.selectTarget.targets;
        const Id id = targets->count > 0 ? LIST_ELEM(targets, targets->index) : -1;
        const bool showEnemyStatus = IsKeyDown(KEY_TAB);
        EnemyDisplay_Draw(ui, battle.combatants, id, NULL, showEnemyStatus);
        PlayerDisplay_Draw(ui, battle.combatants, id, NULL);

        if (id == -1)
        {
            UI_Reset(ui);
            {
                UI_AlignShimH(ui, SCREEN_WIDTH, SCREEN_HEIGHT, ALIGN_H_CENTER);
                UI_Offset(ui, (Vector2){0, 100});
                const Vector2 size = UI_Panel(ui, 200, 30);
                {
                    UI_AlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
                    UI_BodyLabel(ui, "NO TARGETS");
                }
                UI_PanelEnd(ui);
            }
            UI_Draw(ui, (Vector2){0, 0});
        }
        break;
    }
    case BATTLE_COMPILE_EFFECTS:
    {
        TraceLog(LOG_INFO, "BATTLE_COMPILE_EFFECTS");
        break;
    }

    default:
        break;
    }
}