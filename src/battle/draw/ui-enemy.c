#include "ui-enemy.h"

#include "battle/draw/draw-util.h"

static const float spriteScale = 4.0f;
static const int fontSize = 12;

static const float statusPanelWidth = 100.0f;
static const float statusPanelHeight = 50.0f;

static Vector2 uiEnemySprite(UI *ui, const EnemyDisplay *display)
{
    const Texture2D sprite = AssetSprite(display->spriteTag);
    const float width = sprite.width * spriteScale;
    const float height = sprite.height * spriteScale;

    Color tint;
    if (display->option == ENEMY_DISPLAY_TINT)
        tint = display->optionData.tint.color;
    else if (display->option == ENEMY_DISPLAY_DEAD)
        tint = (Color){255, 255, 255, 0};
    else
        tint = WHITE;

    UISprite(ui, sprite, width, height, tint);
    return (Vector2){width, height};
}

static void uiPointer(UI *ui, Vector2 size)
{
    const Texture2D pointer = AssetSprite(SPRITE_POINTER_DOWN);

    UIAlignShimH(ui, size.x, pointer.height, ALIGN_H_CENTER);
    UIOffset(ui, (Vector2){0, -pointer.height});
    UISprite(ui, pointer, pointer.width, pointer.height, WHITE);
}

static void uiStatus(UI *ui, Vector2 size, const EnemyDisplayStatus *status)
{
    const Font font = AssetFont(FONT_KONGTEXT);

    UIAlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
    const Vector2 panelSize = UIPanel(ui, statusPanelWidth, statusPanelHeight);
    {
        UIAlignShimH(ui, panelSize.x, fontSize, ALIGN_H_CENTER);
        UILabel(ui, font, status->name, fontSize, RAYWHITE);

        UIMeter(ui, panelSize.x, fontSize, status->hp, status->maxHp, MAROON);
    }
    UIPanelEnd(ui);
}

static void uiAnimation(UI *ui, Vector2 size, AnimationTag tag, float time)
{
    const Animation *animation = AssetAnimation(tag);
    const Texture2D sprite = AssetAnimationSprite(animation);
    const Rectangle frame = AssetAnimationFrame(animation, time);
    const float width = frame.width * spriteScale;
    const float height = frame.height * spriteScale;

    UIAlignShim(ui, size.x, size.y, ALIGN_H_CENTER, ALIGN_V_CENTER);
    UISpriteSlice(ui, sprite, frame, width, height, WHITE);
}

void UIEnemy(UI *ui, const EnemyDisplay *display)
{
    UIOverlay(ui);
    {
        const Vector2 size = uiEnemySprite(ui, display);

        switch (display->option)
        {
        case ENEMY_DISPLAY_NONE:
            break;
        case ENEMY_DISPLAY_DEAD:
            break;
        case ENEMY_DISPLAY_SELECTED:
            uiPointer(ui, size);
            break;
        case ENEMY_DISPLAY_STATUS:
            uiStatus(ui, size, &display->optionData.status);
            break;
        case ENEMY_DISPLAY_TINT:
            break;
        case ENEMY_DISPLAY_ANIMATION:
            uiAnimation(ui, size, display->optionData.animation.tag, display->optionData.animation.time);
            break;
        }
    }
    UIOverlayEnd(ui);
}