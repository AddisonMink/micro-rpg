#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "stddef.h"
#include "raylib.h"

#include "battle/battle.h"

static UI *ui;
static Vector3 cubePosition = {0.0f, 1.0f, 0.0f};
static Vector3 leftWallPos = {-3.0f, 1.0f, 4.0f};
static Vector3 rightWallPos = {3.0f, 1.0f, 4.0f};
static Camera3D camera = {0};

static void run()
{
    const float delta = GetFrameTime();
    Battle_Update(delta);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(camera);
        {
            DrawCube(cubePosition, 4.0f, 2.0f, 2.0f, RED);
            DrawCubeWires(cubePosition, 4.0f, 2.0f, 2.0f, MAROON);

            DrawCube(leftWallPos, 2.0f, 2.0f, 8.0f, RED);
            DrawCubeWires(leftWallPos, 2.0f, 2.0f, 8.0f, MAROON);

            DrawCube(rightWallPos, 2.0f, 2.0f, 8.0f, RED);
            DrawCubeWires(rightWallPos, 2.0f, 2.0f, 8.0f, MAROON);
        }
        DrawGrid(20, 1.0f);

        EndMode3D();

        Battle_Draw(ui);
    }
    EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template-5.0.0");
    ui = UI_Alloc(100);
    Battle_Init();

    camera.position = (Vector3){0.0f, 1.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type

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