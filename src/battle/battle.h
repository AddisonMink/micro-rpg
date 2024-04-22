#pragma once

#include "common/common-ui.h"

void Battle_Init();

void Battle_Update(float delta);

void Battle_Draw(UI *ui);

void Battle_Draw3D(const Camera3D *camera);