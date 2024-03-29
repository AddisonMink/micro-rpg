#pragma once

#include "asset/asset.h"
#include "ui/ui.h"

#define UI_PANEL_PAD 6
#define UI_PANEL_SPACING 6

Vector2 UIPanel(UI *ui, float width, float height);

void UIPanelEnd(UI *ui);

void UIMeter(UI *ui, float width, float height, int current, int max, Color color);