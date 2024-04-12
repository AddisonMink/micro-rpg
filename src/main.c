#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "stddef.h"
#include "raylib.h"

#include "battle/enemy-display.h"
#include "common/list-macros.h"

static UI *ui;
static Event event;
static Combatant combatants[MAX_COMBATANTS];

static void run()
{
    const float delta = GetFrameTime();
    if (event.elapsed < event.duration)
        event.elapsed += delta;
    if (IsKeyPressed(KEY_ENTER))
        event.elapsed = 0;

    BeginDrawing();
    {
        ClearBackground(BLACK);
        EnemyDisplay_Draw(ui, combatants, -1, event.elapsed < event.duration ? &event : NULL, false);
    }
    EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template-5.0.0");
    ui = UI_Alloc(100);

    combatants[FIRST_ENEMY_ID] = Combatant_Create(FIRST_ENEMY_ID, COMBATANT_TYPE_SCAMP, ROW_FRONT);
    combatants[FIRST_ENEMY_ID + 1] = Combatant_Create(FIRST_ENEMY_ID + 1, COMBATANT_TYPE_SCAMP, ROW_BACK);
    event = Event_Status(FIRST_ENEMY_ID, 1);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update, 0, 1);
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