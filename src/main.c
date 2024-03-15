#include "raylib.h"
#include "raymath.h"

#include "ui/ui.h"

int main(void)
{
    InitWindow(800, 448, "Template-5.0.0");
    UI *ui = UIAlloc(10);
    Texture2D texture = LoadTexture("assets/nineslice.png");
    Font font = LoadFont("assets/kongtext.ttf");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);        

        UIReset(ui);
        {
            UIOverlay(ui);
            UINineSlice(ui, texture, 1, 4, 200, 100, WHITE);
            UIOffset(ui, (Vector2){10,10});
            UILabel(ui, font, "Hello, World!", 12, WHITE);
            UIOverlayEnd(ui);
        }
        UIDraw(ui, (Vector2){100, 100});

        EndDrawing();
    }

    CloseWindow();

    UIFree(ui);
    UnloadTexture(texture);
    UnloadFont(font);
    return 0;
}