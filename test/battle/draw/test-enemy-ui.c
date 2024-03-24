#include "raylib.h"

#include "asset/asset.h"
#include "ui/ui.h"
#include "battle/types.h"
#include "battle/draw/ui-enemy.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Enemy UI Test");
    UI *ui = UIAlloc(100);

    // option = none
    /*
    EnemyDisplay enemy = {
        .spriteTag = SPRITE_SCAMP,
        .row = ROW_FRONT,
        .option = ENEMY_DISPLAY_NONE,
    };
    */

    // option = selected
    /*
    EnemyDisplay enemy = {
        .spriteTag = SPRITE_SCAMP,
        .row = ROW_FRONT,
        .option = ENEMY_DISPLAY_SELECTED,
    };
    */

    // option = status
    /*
    EnemyDisplay enemy = {
        .spriteTag = SPRITE_SCAMP,
        .row = ROW_FRONT,
        .option = ENEMY_DISPLAY_STATUS,
        .optionData.status = {
            .name = "Scamp",
            .hp = 2,
            .maxHp = 3,
        },
    };
    */

    // option = tint
    /*
    EnemyDisplay enemy = {
        .spriteTag = SPRITE_SCAMP,
        .row = ROW_FRONT,
        .option = ENEMY_DISPLAY_TINT,
        .optionData.tint = {
            .color = (Color){255, 0, 0, 125},
        },
    };
    */

    // option = animation
    /*
    EnemyDisplay enemy = {
        .spriteTag = SPRITE_SCAMP,
        .row = ROW_FRONT,
        .option = ENEMY_DISPlAY_ANIMATION,
        .optionData.animation = {
            .tag = ANIMATION_SLASH,
            .time = 0.25f,
        },
    };
    */

    // option = action
    EnemyDisplay enemy = {
        .spriteTag = SPRITE_SCAMP,
        .row = ROW_FRONT,
        .option = ENEMY_DISPLAY_ACTION,
        .optionData.action = {
            .name = "Attack",
        },
    };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        UIReset(ui);
        UIAlignShim(ui, screenWidth, screenHeight, ALIGN_H_CENTER, ALIGN_V_CENTER);
        UIEnemy(ui, &enemy);
        UIDraw(ui, (Vector2){0, 0});

        EndDrawing();
    }

    CloseWindow();
    UIFree(ui);
    AssetUnloadAll();

    return 0;
}