#include "enemy-display.h"

#include "stddef.h"

#include "raymath.h"

static const float Z_POS = 0.0;
static const float Z_POS_BACK_ROW_OFFSET = 3.0;
static const float Y_POS = 0.0;
static const float X_POS_1[MAX_ENEMIES] = {0, 0, 0};
static const float X_POS_2[MAX_ENEMIES] = {-1, 1, 0};
static const float X_POS_3[MAX_ENEMIES] = {-1, 0, 1};

static const float STATUS_PANEL_WIDTH = 100;
static const float STATUS_PANEL_HEIGHT = 50;
static const float STATUS_PANEL_SPACING = 5;

static const float MESSAGE_PANEL_WIDTH = 120;
static const float MESSAGE_PANEL_HEIGHT = 25;

static void renderPointer(Camera3D camera, Vector3 origin)
{
    origin.y += 0.6;
    DrawBillboard(camera, Asset_Sprite(SPRITE_POINTER_DOWN), origin, 0.2, WHITE);
}

static void renderEnemySprite(
    Camera3D camera,
    const Combatant *enemy,
    Vector3 origin,
    const Event *eventOpt)
{
    const Color tint =
        eventOpt && eventOpt->type == EVENT_FLASH  ? eventOpt->data.flash
        : eventOpt && eventOpt->type == EVENT_FADE ? Fade(WHITE, 1.0 - eventOpt->elapsed)
        : enemy->state == COMBATANT_STATE_DEAD     ? BLANK
                                                   : WHITE;

    if (eventOpt && eventOpt->type == EVENT_MOVE)
    {
        const float dest = enemy->row == ROW_FRONT ? 0 : -Z_POS_BACK_ROW_OFFSET;
        const float start = enemy->row == ROW_FRONT ? -Z_POS_BACK_ROW_OFFSET : 0;
        const float progress = eventOpt->elapsed / eventOpt->duration;

        origin.z = Lerp(start, dest, progress);
    }

    DrawBillboard(camera, enemy->sprite, origin, 1.0, tint);
}

static void renderAnimation(
    Camera3D camera,
    const Animation *animation,
    float elapsed,
    Vector3 origin)
{
    const Texture2D sprite = Asset_AnimationSprite(animation);
    const Rectangle src = Asset_AnimationFrame(animation, elapsed);
    const Vector3 up = {0, 1, 0};
    const Vector2 size = {1.0, 1.0};
    DrawBillboardPro(camera, sprite, src, origin, up, size, (Vector2){0, 0}, 0, WHITE);
}

static void drawStatusPanel(UI *ui, const Combatant *enemy, Vector2 pos)
{
    const Vector2 origin = (Vector2){pos.x - STATUS_PANEL_WIDTH / 2, pos.y - STATUS_PANEL_HEIGHT / 2};

    UI_Reset(ui);
    Vector2 size = UI_Panel(ui, STATUS_PANEL_WIDTH, STATUS_PANEL_HEIGHT);
    {
        UI_AlignShimV(ui, size.x, size.y, ALIGN_V_CENTER);
        UI_Col(ui, STATUS_PANEL_SPACING);
        {
            UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
            UI_BodyLabel(ui, enemy->name);

            UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
            UI_Meter(ui, size.x, enemy->hp, enemy->hpMax, RED, DARKGRAY);
        }
        UI_ColEnd(ui);
    }
    UI_PanelEnd(ui);
    UI_Draw(ui, origin);
}

static void drawMessagePanel(UI *ui, const char *message, Vector2 pos)
{
    const Vector2 origin = (Vector2){pos.x - MESSAGE_PANEL_WIDTH / 2, pos.y - MESSAGE_PANEL_HEIGHT / 2};

    UI_Reset(ui);
    Vector2 size = UI_Panel(ui, MESSAGE_PANEL_WIDTH, MESSAGE_PANEL_HEIGHT);
    {
        UI_AlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UI_BodyLabel(ui, message);
    }
    UI_PanelEnd(ui);
    UI_Draw(ui, origin);
}

static void renderEnemy(
    Camera3D camera, UI *ui,
    const Combatant *enemy,
    Vector3 origin,
    const Event *eventOpt,
    bool showStatus,
    bool selected)
{
    if (enemy->row == ROW_BACK)
        origin.z -= Z_POS_BACK_ROW_OFFSET;

    const EventType eventType = eventOpt == NULL ? -1 : eventOpt->type;
    const Vector2 pos = GetWorldToScreen(origin, camera);

    BeginMode3D(camera);
    {
        if (selected)
        {
            renderPointer(camera, origin);
        }

        renderEnemySprite(camera, enemy, origin, eventOpt);

        if (eventType == EVENT_ANIMATE)
        {
            renderAnimation(camera, eventOpt->data.animate, eventOpt->elapsed, origin);
        }
    }
    EndMode3D();

    if (showStatus || eventType == EVENT_STATUS)
    {
        drawStatusPanel(ui, enemy, pos);
    }

    if (eventType == EVENT_MESSAGE)
    {
        drawMessagePanel(ui, eventOpt->data.message, pos);
    }
}

void EnemyDisplay_Draw(
    Camera3D camera,
    UI *ui,
    Combatant combatants[MAX_ENEMIES],
    Id selected,
    const Event *eventOpt,
    bool showStatus)
{
    CombatantRefList enemies = Combatant_EnemyList(combatants);

    const float *x_pos =
        enemies.count == 1   ? X_POS_1
        : enemies.count == 2 ? X_POS_2
                             : X_POS_3;

    int index = 0;
    LIST_FOREACH(&enemies, Combatant *, enemy)
    {
        const Id id = (*enemy)->id;
        const Vector3 origin = (Vector3){x_pos[index], Y_POS, Z_POS};
        const Event *event = eventOpt && eventOpt->id == id ? eventOpt : NULL;
        const bool isSelected = id == selected;
        renderEnemy(camera, ui, *enemy, origin, event, showStatus, isSelected);
        index++;
    }
}