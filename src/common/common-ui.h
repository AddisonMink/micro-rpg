#pragma once

#include "asset/asset.h"
#include "ui/ui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BODY_FONT_SIZE 14
#define HEADER_FONT_SIZE 18

void UI_BodyLabel(UI *ui, const char *text);

void UI_HeaderLabel(UI *ui, const char *text);

void UI_ScreenCenter(UI *ui);

Vector2 UI_Panel(UI *ui, float width, float height);

void UI_PanelEnd(UI *ui);

void UI_Meter(UI *ui, float width, float value, float max, Color color, Color bgColor);

void UI_BodyMessage(UI *ui, const char *text);