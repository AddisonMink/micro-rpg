#include "raylib.h"
#include "raymath.h"

#include "asset.h"
#include "ui/ui.h"
#include "battle/battle.h"

int main(void)
{
    InitWindow(800, 600, "Template-5.0.0");
    UI *ui = UIAlloc(100);
    Battle *battle = BattleAlloc();
    Texture2D nineSlice = AssetSprite(SPRITE_NINESLICE);
    Font font = AssetFont(FONT_TAG_KONGTEXT);
    Texture2D scamp = AssetSprite(SPRITE_SCAMP);

    while (!WindowShouldClose())
    {
        const float delta = GetFrameTime();
        BattleUpdate(battle, delta);
        BeginDrawing();
        ClearBackground(BLACK);
        UIReset(ui);
        UISprite(ui, scamp, 128, 128, WHITE);
        UIDraw(ui, (Vector2){0, 0});
        BattleDraw(battle, ui);
        EndDrawing();
    }

    CloseWindow();

    UIFree(ui);
    BattleFree(battle);
    AssetUnloadAll();
    return 0;
}