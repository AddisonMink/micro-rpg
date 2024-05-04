#include "battle/battle.h"

#include "stddef.h"

#include "battle/action.h"
#include "battle/action-menu.h"
#include "battle/effect.h"
#include "battle/enemy-behavior.h"
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

STATIC_LIST_TYPE_PTR(Combatant, MAX_COMBATANTS);

static Battle battle;

static CombatantList PlayerList()
{
    CombatantList list = {
        .capacity = MAX_PLAYERS,
        .count = 0,
    };
    for (Id i = 0; i < MAX_PLAYERS; i++)
        list.data[list.count++] = &battle.combatants[i];
    return list;
}

static CombatantList EnemyList()
{
    CombatantList list = {
        .capacity = MAX_ENEMIES,
        .count = 0,
    };
    for (Id i = FIRST_ENEMY_ID; i < MAX_COMBATANTS; i++)
        list.data[list.count++] = &battle.combatants[i];
    return list;
}

void Battle_Init()
{
    battle.combatants[0] = Combatant_Create(0, COMBATANT_TYPE_MAGICIAN, ROW_BACK);
    battle.combatants[1] = Combatant_Create(1, COMBATANT_TYPE_GALOOT, ROW_FRONT);
    battle.combatants[FIRST_ENEMY_ID] = Combatant_Create(FIRST_ENEMY_ID, COMBATANT_TYPE_SCAMP, ROW_BACK);
    battle.combatants[FIRST_ENEMY_ID + 1] = Combatant_Create(FIRST_ENEMY_ID + 1, COMBATANT_TYPE_SCAMP, ROW_FRONT);
    battle.combatants[FIRST_ENEMY_ID + 2] = Combatant_Create(FIRST_ENEMY_ID + 2, COMBATANT_TYPE_SCAMP, ROW_BACK);

    battle.queue = Queue_Create(battle.combatants);

    battle.items = (ItemList)LIST_INIT(MAX_ITEMS);
    LIST_APPEND((&battle.items), Item_Create(ITEM_LODESTONE));
    LIST_APPEND((&battle.items), Item_Create(ITEM_WAND));

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
                TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_COMPILE_EFFECTS");
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
                TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_SELECT_TARGET");
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
            TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_SELECT_ACTION");
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
                .targetOpt = LIST_ELEM_DEPRECATED((&data->targets), data->targets.index),
            };
            TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_COMPILE_EFFECTS");
        }
        break;
    }
    case BATTLE_COMPILE_EFFECTS:
    {
        const Id id = Queue_Peek(&battle.queue);
        const Combatant *actor = &battle.combatants[id];
        const CompileEffects *data = &battle.data.compileEffects;
        EffectList effects = Effect_Compile(data->action, actor, data->targetOpt, data->itemIndexOpt);
        battle.state = BATTLE_EXECUTE_EFFECTS;
        battle.data.executeEffects = (ExecuteEffects){
            .effects = effects,
        };
        TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_EXECUTE_EFFECTS");
        break;
    }
    case BATTLE_EXECUTE_EFFECTS:
    {
        EffectList *effects = &battle.data.executeEffects.effects;

        if (LIST_EMPTY_DEPRECATED(effects))
        {
            battle.state = BATTLE_END_TURN;
            TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_END_TURN");
        }
        else
        {
            Effect effect = LIST_ELEM_DEPRECATED(effects, 0);
            LIST_DELETE(effects, 0);

            EffectResult result = Effect_Execute(battle.combatants, &battle.items, &battle.queue, effect);
            LIST_CONCAT(effects, (&result.effects));

            if (!LIST_EMPTY_DEPRECATED((&result.events)))
            {
                battle.state = BATTLE_SHOW_EVENTS;
                battle.data.showEvents = (ShowEvents){
                    .effects = *effects,
                    .events = result.events,
                };
                TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_SHOW_EVENTS");
            }
        }
        break;
    }
    case BATTLE_SHOW_EVENTS:
    {
        ShowEvents *data = &battle.data.showEvents;
        EventList *events = &data->events;

        if (LIST_EMPTY_DEPRECATED(events))
        {
            battle.data.executeEffects = (ExecuteEffects){
                .effects = data->effects,
            };
            battle.state = BATTLE_EXECUTE_EFFECTS;
        }
        else
        {
            Event *event = &LIST_ELEM_DEPRECATED(events, 0);
            event->elapsed += delta;
            if (event->elapsed >= event->duration)
            {
                LIST_DELETE(events, 0);
            }
        }
        break;
    }
    case BATTLE_END_TURN:
    {
        CombatantList players = PlayerList();
        CombatantList enemies = EnemyList();
        const Id id = Queue_PeekNext(&battle.queue);

        const bool allPlayersDead = !LIST_EXISTS_PTR_DEPRECATED((&players), Combatant, {
            success = elem->state == COMBATANT_STATE_ALIVE;
        });

        const bool allEnemiesDead = !LIST_EXISTS_PTR_DEPRECATED((&enemies), Combatant, {
            success = elem->state == COMBATANT_STATE_ALIVE;
        });

        const bool allEnemiesBack = !LIST_EXISTS_PTR_DEPRECATED((&enemies), Combatant, {
            success = elem->state == COMBATANT_STATE_ALIVE && elem->row == ROW_FRONT;
        });

        const bool allPlayersBack = !LIST_EXISTS_PTR_DEPRECATED((&players), Combatant, {
            success = elem->state == COMBATANT_STATE_ALIVE && elem->row == ROW_FRONT;
        });

        if (allPlayersDead)
        {
            battle.state = BATTLE_LOSE;
        }
        else if (allEnemiesDead)
        {
            battle.state = BATTLE_WIN;
        }
        else if (allEnemiesBack || allPlayersBack)
        {
            battle.state = BATTLE_EXECUTE_EFFECTS;
            battle.data.executeEffects = (ExecuteEffects){
                .effects = LIST_INIT(MAX_EFFECTS),
            };

            if (allPlayersBack)
            {
                LIST_FOREACH_PTR_DEPRECATED((&players), Combatant, {
                    if (elem->state == COMBATANT_STATE_ALIVE)
                    {
                        LIST_APPEND((&battle.data.executeEffects.effects), EffectMove_Create(DIRECTION_FORWARD, elem->id));
                    }
                });
            }

            if (allEnemiesBack)
            {
                LIST_FOREACH_PTR_DEPRECATED((&enemies), Combatant, {
                    if (elem->state == COMBATANT_STATE_ALIVE)
                    {
                        LIST_APPEND((&battle.data.executeEffects.effects), EffectMove_Create(DIRECTION_FORWARD, elem->id));
                    }
                });
            }
            TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_EXECUTE_EFFECTS");
            break;
        }
        else if (id < FIRST_ENEMY_ID)
        {
            Queue_Next(&battle.queue);
            ActionMenu_Init(&battle.items, &battle.combatants[id]);
            battle.state = BATTLE_SELECT_ACTION;
            TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_SELECT_ACTION");
        }
        else
        {
            Queue_Next(&battle.queue);
            battle.state = BATTLE_ENEMY_TURN;
            TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_ENEMY_TURN");
        }
        break;
    }
    case BATTLE_ENEMY_TURN:
    {
        EnemyBehavior behavior = EnemyBehavior_Get(battle.combatants, Queue_Peek(&battle.queue));
        battle.data.compileEffects = (CompileEffects){
            .action = behavior.action,
            .targetOpt = behavior.targetOpt,
        };
        battle.state = BATTLE_COMPILE_EFFECTS;
        TraceLog(LOG_INFO, "Battle_Update: Transition to BATTLE_COMPILE_EFFECTS");
        break;
    }
    case BATTLE_WIN:
    {
        break;
    }
    case BATTLE_LOSE:
    {
        break;
    }
    }
}

static void drawGlobalMessage(UI *ui, const char *text)
{
    UI_Reset(ui);
    {
        UI_AlignShimH(ui, SCREEN_WIDTH, SCREEN_HEIGHT, ALIGN_H_CENTER);
        UI_Offset(ui, (Vector2){0, 100});
        UI_BodyMessage(ui, text);
    }
    UI_Draw(ui, (Vector2){0, 0});
}

void Battle_Draw(Camera3D camera, UI *ui)
{
    switch (battle.state)
    {
    case BATTLE_SELECT_ACTION:
    {
        const bool showEnemyStatus = IsKeyDown(KEY_TAB);
        EnemyDisplay_Draw(camera, ui, battle.combatants, -1, NULL, showEnemyStatus);
        PlayerDisplay_Draw(ui, battle.combatants, -1, NULL);
        ActionMenu_Draw(ui);
        break;
    }
    case BATTLE_SELECT_TARGET:
    {
        const TargetList *targets = &battle.data.selectTarget.targets;
        const Id id = targets->count > 0 ? LIST_ELEM_DEPRECATED(targets, targets->index) : -1;
        const bool showEnemyStatus = IsKeyDown(KEY_TAB);
        EnemyDisplay_Draw(camera, ui, battle.combatants, id, NULL, showEnemyStatus);
        PlayerDisplay_Draw(ui, battle.combatants, id, NULL);

        if (id == -1)
        {
            drawGlobalMessage(ui, "NO TARGETS");
        }
        break;
    }
    case BATTLE_SHOW_EVENTS:
    {
        const ShowEvents *data = &battle.data.showEvents;
        if (LIST_EMPTY_DEPRECATED((&data->events)))
        {
            EnemyDisplay_Draw(camera, ui, battle.combatants, -1, NULL, false);
            PlayerDisplay_Draw(ui, battle.combatants, -1, NULL);
            break;
        }
        const Event *event = &LIST_ELEM_DEPRECATED((&data->events), 0);

        EnemyDisplay_Draw(camera, ui, battle.combatants, -1, event, false);
        PlayerDisplay_Draw(ui, battle.combatants, -1, event);

        if (event->type == EVENT_GLOBAL_MESSAGE)
        {
            drawGlobalMessage(ui, event->data.globalMessage);
        };
        break;
    }
    case BATTLE_WIN:
    {
        EnemyDisplay_Draw(camera, ui, battle.combatants, -1, NULL, false);
        PlayerDisplay_Draw(ui, battle.combatants, -1, NULL);
        drawGlobalMessage(ui, "V I C T O R Y");
        break;
    }
    case BATTLE_LOSE:
    {
        EnemyDisplay_Draw(camera, ui, battle.combatants, -1, NULL, false);
        PlayerDisplay_Draw(ui, battle.combatants, -1, NULL);
        drawGlobalMessage(ui, "D E F E A T");
        break;
    }
    default:
    {
        EnemyDisplay_Draw(camera, ui, battle.combatants, -1, NULL, false);
        PlayerDisplay_Draw(ui, battle.combatants, -1, NULL);
        break;
    }
    }
}