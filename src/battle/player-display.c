#include "player-display.h"

#include "stddef.h"

#define SPACING 20
#define SPRITE_SCALING 4.0f
#define ROW_OFFSET -40
#define PANEL_HEIGHT 60
#define PANEL_SPACING 10

void ui_player(UI *ui, const Combatant *player, bool selected, const Event *eventOpt)
{
    const float width = player->sprite.width * SPRITE_SCALING;
    const float spriteHeight = player->sprite.height * SPRITE_SCALING;

    Color tint = WHITE;
    if (player->state == COMBATANT_STATE_DEAD)
        tint = DARKGRAY;
    else if (eventOpt != NULL && eventOpt->type == EVENT_FLASH)
        tint = eventOpt->data.flash;
    else if (eventOpt != NULL && eventOpt->type == EVENT_FADE)
        tint.a = (1 - eventOpt->elapsed / eventOpt->duration) * 255;

    Vector2 offset = {0, ROW_OFFSET};
    if (eventOpt != NULL && eventOpt->type == EVENT_MOVE)
    {
        const float progress = eventOpt->elapsed / eventOpt->duration;
        if (player->row == ROW_FRONT)
            offset.y = progress * ROW_OFFSET;
        else
            offset.y = (1 - progress) * ROW_OFFSET;
    }
    else if (player->row == ROW_BACK)
    {
        offset.y = 0;
    }

    UI_Offset(ui, offset);
    UI_Col(ui, 0);
    {
        UI_Overlay(ui);
        {
            UI_Sprite(ui, player->sprite, width, spriteHeight, tint);

            if (selected)
            {
                const Texture2D pointer = Asset_Sprite(SPRITE_POINTER_DOWN);

                UI_AlignShimH(ui, width, spriteHeight, ALIGN_H_CENTER);
                UI_Offset(ui, (Vector2){0, -pointer.height});
                UI_Sprite(ui, pointer, pointer.width, pointer.height, WHITE);
            }

            if (eventOpt != NULL && eventOpt->type == EVENT_ANIMATE)
            {
                const Animation *animation = eventOpt->data.animate;
                const Texture2D frame = Asset_AnimationSprite(animation);
                const Rectangle source = Asset_AnimationFrame(animation, eventOpt->elapsed);
                const float frameWidth = source.width * SPRITE_SCALING;
                const float frameHeight = source.height * SPRITE_SCALING;

                UI_AlignShim(ui, width, spriteHeight, ALIGN_H_CENTER, ALIGN_V_CENTER);
                UI_SpriteSlice(ui, frame, source, frameWidth, frameHeight, WHITE);
            }
        }
        UI_OverlayEnd(ui);

        const Vector2 size = UI_Panel(ui, width, PANEL_HEIGHT);
        {
            UI_Col(ui, PANEL_SPACING);
            {
                UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
                UI_BodyLabel(ui, player->name);

                UI_AlignShimH(ui, size.x, BODY_FONT_SIZE, ALIGN_H_CENTER);
                UI_Meter(ui, size.x, player->hp, player->hpMax, MAROON, DARKGRAY);
            }
            UI_ColEnd(ui);
        }
        UI_PanelEnd(ui);
    }
    UI_ColEnd(ui);
}

void PlayerDisplay_Draw(UI *ui, const Combatant combatants[MAX_COMBATANTS], Id selected, const Event *eventOpt)
{
    UI_Reset(ui);
    {
        UI_AlignShim(ui, SCREEN_WIDTH, SCREEN_HEIGHT, ALIGN_H_CENTER, ALIGN_V_BOTTOM);
        UI_Offset(ui, (Vector2){0, -SPACING});
        UI_Row(ui, SPACING);
        for (Id i = 0; i < FIRST_ENEMY_ID; i++)
        {
            const Combatant *combatant = &combatants[i];
            if (combatant->state != COMBATANT_STATE_INVALID)
            {
                const bool isSelected = i == selected;
                const Event *playerEvent = eventOpt != NULL && eventOpt->id == i ? eventOpt : NULL;
                ui_player(ui, combatant, isSelected, playerEvent);
            }
        }
        UI_RowEnd(ui);
    }
    UI_Draw(ui, (Vector2){0, 0});
}