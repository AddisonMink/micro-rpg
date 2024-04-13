#include "enemy-display.h"

#include "stddef.h"

#define SPACING 20
#define SPRITE_SCALE 4.0
#define BACK_ROW_SCALE 0.75
#define MESSAGE_HEIGHT 30
#define STATUS_HEIGHT (MESSAGE_HEIGHT * 2)
#define STATUS_SPACING 10

static void ui_enemy(UI *ui, const Combatant *enemy, bool selected, const Event *eventOpt, bool showStatus)
{
    if (eventOpt != NULL && eventOpt->type == EVENT_FADE)
        TraceLog(LOG_INFO, "FADE");

    const float width = enemy->sprite.width * SPRITE_SCALE;
    const float height = enemy->sprite.height * SPRITE_SCALE;

    float spriteWidth = width;
    float spriteHeight = height;
    if (eventOpt != NULL && eventOpt->type == EVENT_MOVE)
    {
        const float diff = 1 - BACK_ROW_SCALE;
        const float progress = eventOpt->elapsed / eventOpt->duration;

        float scale;
        if (enemy->row == ROW_FRONT)
            scale = BACK_ROW_SCALE + diff * progress;
        else
            scale = 1 - diff * progress;
        if (scale > 1)
            scale = 1;

        spriteWidth *= scale;
        spriteHeight *= scale;
    }
    else if (enemy->row == ROW_BACK)
    {
        spriteWidth *= BACK_ROW_SCALE;
        spriteHeight *= BACK_ROW_SCALE;
    }

    Color color = WHITE;
    if (enemy->state == COMBATANT_STATE_DEAD)
        color.a = 0;
    else if (eventOpt != NULL && eventOpt->type == EVENT_FLASH)
        color = eventOpt->data.flash;
    if (eventOpt != NULL && eventOpt->type == EVENT_FADE)
        color.a = (1 - eventOpt->elapsed / eventOpt->duration) * 255;

    UI_Overlay(ui);
    {
        UI_AlignShim(ui, width, height, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UI_Sprite(ui, enemy->sprite, spriteWidth, spriteHeight, color);

        if (selected)
        {
            const Texture2D pointer = Asset_Sprite(SPRITE_POINTER_DOWN);
            UI_AlignShimH(ui, width, height, ALIGN_H_CENTER);
            UI_Offset(ui, (Vector2){0, -pointer.height});
            UI_Sprite(ui, pointer, pointer.width, pointer.height, WHITE);
        }

        if (eventOpt != NULL && eventOpt->type == EVENT_ANIMATE)
        {
            const Texture2D animSprite = Asset_AnimationSprite(eventOpt->data.animate);
            const Rectangle source = Asset_AnimationFrame(eventOpt->data.animate, eventOpt->elapsed);
            const float animWidth = source.width * SPRITE_SCALE;
            const float animHeight = source.height * SPRITE_SCALE;

            UI_AlignShim(ui, width, height, ALIGN_H_CENTER, ALIGN_V_CENTER);
            UI_SpriteSlice(ui, animSprite, source, animWidth, animHeight, WHITE);
        }

        if (eventOpt != NULL && eventOpt->type == EVENT_MESSAGE)
        {
            UI_AlignShim(ui, width, height, ALIGN_H_CENTER, ALIGN_V_CENTER);
            const Vector2 size = UI_Panel(ui, width, MESSAGE_HEIGHT);
            {
                UI_BodyLabel(ui, eventOpt->data.message);
            }
            UI_PanelEnd(ui);
        }

        if (showStatus || eventOpt != NULL && eventOpt->type == EVENT_STATUS)
        {
            UI_AlignShim(ui, width, height, ALIGN_H_CENTER, ALIGN_V_CENTER);
            const Vector2 size = UI_Panel(ui, width, STATUS_HEIGHT);
            {
                UI_AlignShimV(ui, size.x, size.y, ALIGN_V_CENTER);
                UI_Col(ui, STATUS_SPACING);
                {
                    UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
                    UI_BodyLabel(ui, enemy->name);

                    UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
                    UI_Meter(ui, size.x, enemy->hp, enemy->hpMax, RED, DARKGRAY);
                }
                UI_ColEnd(ui);
            }
            UI_PanelEnd(ui);
        }
    }
    UI_OverlayEnd(ui);
}

void EnemyDisplay_Draw(UI *ui, const Combatant combatants[MAX_ENEMIES], Id selected, const Event *eventOpt, bool showStatus)
{
    UI_Reset(ui);
    {
        UI_AlignShim(ui, SCREEN_WIDTH, SCREEN_HEIGHT, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UI_Row(ui, SPACING);
        for (Id i = FIRST_ENEMY_ID; i < MAX_COMBATANTS; i++)
        {
            const Combatant *combatant = &combatants[i];
            if (combatant->state != COMBATANT_STATE_INVALID)
            {
                const bool isSelected = i == selected;
                const Event *enemyEvent = eventOpt != NULL && eventOpt->id == i ? eventOpt : NULL;
                ui_enemy(ui, combatant, isSelected, enemyEvent, showStatus);
            }
        }
        UI_RowEnd(ui);
    }
    UI_Draw(ui, (Vector2){0, 0});
}