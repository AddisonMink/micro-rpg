#include "raylib.h"
#include "raymath.h"

#include "ui/ui.h"

int main(void)
{
    InitWindow(800, 448, "Template-5.0.0");
    UI *ui = UIAlloc(100);
    Texture2D texture = LoadTexture("assets/nineslice.png");
    Texture2D goblinTexture = LoadTexture("assets/goblin_32.png");
    Font font = LoadFont("assets/kongtext.ttf");

    int size = 128;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            size = (size == 128) ? 64 : 128;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        UIReset(ui);
        {
            UIShim(ui, 800, 448);
            UIAlign(ui, ALIGN_H_CENTER, ALIGN_V_CENTER);
            UIRow(ui, 20);

            UIOverlay(ui);
            UIShim(ui,128,128);
            UIAlign(ui, ALIGN_H_CENTER, ALIGN_V_CENTER);
            UISprite(ui, goblinTexture, size, size, BLUE);

            UINineSlice(ui,texture,1,4,128,28,WHITE);

            UIAlignH(ui,ALIGN_H_CENTER);
            UIOffset(ui,(Vector2){0,8});
            UILabel(ui, font, "Cleaver", 12, RAYWHITE);
            UIOverlayEnd(ui);
            
            UISprite(ui, goblinTexture, 128, 128, WHITE);
            UISprite(ui, goblinTexture, 128, 128, WHITE);
            UIRowEnd(ui);
        }
        UIDraw(ui, (Vector2){0, 0});

        EndDrawing();
    }

    CloseWindow();

    UIFree(ui);
    UnloadTexture(texture);
    UnloadTexture(goblinTexture);
    UnloadFont(font);
    return 0;
}