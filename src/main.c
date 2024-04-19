#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "stddef.h"
#include "raylib.h"

#include "battle/battle.h"

static UI *ui;

static void run()
{
    const float delta = GetFrameTime();
    Battle_Update(delta);

    BeginDrawing();
    {
        ClearBackground(BLACK);
        Battle_Draw(ui);
    }
    EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template-5.0.0");
    ui = UI_Alloc(100);
    Battle_Init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(run, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        run();
    }
#endif

    CloseWindow();
    UI_Free(ui);
    return 0;
}