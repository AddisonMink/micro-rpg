#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"

#include "battle/action-menu.h"
#include "common/list-macros.h"

static UI *ui;

static void run()
{
    const float delta = GetFrameTime();

    BeginDrawing();
    {
        ClearBackground(BLACK);
        ActionMenu_Draw(ui);
    }
    EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template-5.0.0");
    ui = UI_Alloc(100);

    Combatant player = Combatant_Create(0, COMBATANT_TYPE_MAGICIAN, ROW_FRONT);

    ItemList items = LIST_INIT(MAX_ITEMS);
    LIST_APPEND((&items), Item_Create(ITEM_LODESTONE));

    ActionMenu_Init(&items, &player);

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