#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// GenMeshPlane
// DrawMesh

#include "stddef.h"
#include "raylib.h"

#include "battle/battle.h"

static Camera3D camera = {0};
static UI *ui;

static Texture2D wallTexture;
static Mesh planeMesh;
static Model wallModel;

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

            for (int z = 5; z >= -5; z -= 2)
            {
                DrawModelEx(wallModel, (Vector3){-2.5, 1, z}, (Vector3){0, 1, 0}, 0.0f, (Vector3){1, 1, 1}, WHITE);
                DrawModelEx(wallModel, (Vector3){2.5, 1, z}, (Vector3){1, 0, 0}, 180.0f, (Vector3){1, 1, 1}, WHITE);
            }
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

    wallTexture = Asset_Sprite(SPRITE_WALL);
    // planeMesh = GenMeshPlane(1.0f, 1.0f, 1, 1);
    planeMesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    wallModel = LoadModelFromMesh(planeMesh);
    SetMaterialTexture(&wallModel.materials[0], MATERIAL_MAP_DIFFUSE, wallTexture);

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