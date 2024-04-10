#pragma once

#include "asset/asset.h"
#include "ui/ui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void UI_BodyLabel(UI *ui, const char *text);

void UI_HeaderLabel(UI *ui, const char *text);

void UI_ScreenCenter(UI *ui);

Vector2 UI_Panel(UI *ui, float width, float height);

void UI_PanelEnd(UI *ui);