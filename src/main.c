#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "stddef.h"
#include "raylib.h"

#include "battle/battle.h"

static Camera3D camera = {0};
static UI *ui;

static void run()
{
    const float delta = GetFrameTime();
    Battle_Update(delta);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(camera);
        {
            DrawGrid(10, 1.0f);
            DrawCube((Vector3){-2, 1, 0.0f}, 1.0f, 2.0f, 12.0f, RED);
            DrawCube((Vector3){2, 1, 0.0f}, 1.0f, 2.0f, 12.0f, RED);
            DrawCube((Vector3){0, -1, 0.0f}, 4.0f, 2.0f, 12.0f, BLUE);
            DrawCube((Vector3){0, 3, 0.0f}, 4.0f, 2.0f, 12.0f, BLUE);
        }
        EndMode3D();

        Battle_Draw(camera, ui);
    }
    EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template-5.0.0");
    ui = UI_Alloc(100);
    Battle_Init();

    camera.position = (Vector3){0.0f, 1, 5.0f};
    camera.target = (Vector3){0.0f, 1.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

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