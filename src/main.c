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
static Mesh cubeMesh;
static Model wallModel;

static Texture2D floorTexture;
static Mesh floorMesh;
static Model floorModel;

static void run()
{
    const float delta = GetFrameTime();
    Battle_Update(delta);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode3D(camera);
        {
            float progress = 0.0f;
            for (float z = 10; z >= -5; z -= 2)
            {
                Color tint = WHITE;
                tint.r = WHITE.r - (progress / 3 * 255.0f);
                tint.g = WHITE.g - (progress / 1.5 * 255.0f);
                tint.b = WHITE.b - (progress * 255.0f);
                progress += 0.2f;
                
                DrawModelEx(wallModel, (Vector3){-2.5, 1, z}, (Vector3){0, 1, 0}, 0.0f, (Vector3){1, 1, 1}, tint);
                DrawModelEx(wallModel, (Vector3){2.5, 1, z}, (Vector3){1, 0, 0}, 180.0f, (Vector3){1, 1, 1}, tint);
                DrawModelEx(floorModel, (Vector3){-1, -1, z}, (Vector3){1, 0, 0}, 90.0f, (Vector3){1, 1, 1}, tint);
                DrawModelEx(floorModel, (Vector3){1, -1, z}, (Vector3){1, 0, 0}, 90.0f, (Vector3){1, 1, 1}, tint);
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
    cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    wallModel = LoadModelFromMesh(cubeMesh);
    SetMaterialTexture(&wallModel.materials[0], MATERIAL_MAP_DIFFUSE, wallTexture);

    floorTexture = Asset_Sprite(SPRITE_FLOOR);
    floorModel = LoadModelFromMesh(cubeMesh);
    SetMaterialTexture(&floorModel.materials[0], MATERIAL_MAP_DIFFUSE, floorTexture);

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