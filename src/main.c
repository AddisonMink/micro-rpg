#include "raylib.h"
#include "raymath.h"

#include "asset/asset.h"
#include "ui/ui.h"
#include "battle/battle.h"

#include "battle/battle-types.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template-5.0.0");
    UI *ui = UIAlloc(100);
    Battle *battle = BattleAlloc();

    while (!WindowShouldClose())
    {
        const float delta = GetFrameTime();
        BattleUpdate(battle, delta);

        BeginDrawing();
        ClearBackground(BLACK);
        BattleDraw(battle, ui);
        EndDrawing();
    }

    CloseWindow();

    UIFree(ui);
    BattleFree(battle);
    AssetUnloadAll();
    return 0;
}