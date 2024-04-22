#include "enemy-display-3d.h"

#include "stddef.h"

static const float twoXPositions[] = {-1.0f, 1.0f, 0.0f};
static const float threeXPositions[] = {-1.5f, 0.0f, 1.5f};
static const float yOffset = 0.5f;
static const float frontZ = 5.0f;
static const float backZ = 3.0f;

static Vector3 enemyPosition(const Combatant *enemy, int numEnemies, int index)
{
    float x = 0.0f;
    if (numEnemies == 2)
        x = twoXPositions[index];
    else if (numEnemies == 3)
        x = threeXPositions[index];

    float z = enemy->row == ROW_FRONT ? frontZ : backZ;

    return (Vector3){x, yOffset, z};
}

static void drawEnemy(const Camera3D *camera, const Combatant *enemy, Vector3 pos, const Event *event)
{
    DrawBillboard(*camera, enemy->sprite, pos, 1.0f, WHITE);
}

void EnemyDisplay3D_Draw(const Camera3D *camera, const Combatant combatants[MAX_COMBATANTS], const Event *event)
{
    int count = 0;
    for (Id i = FIRST_ENEMY_ID; i < MAX_COMBATANTS; i++)
    {
        const Combatant *enemy = &combatants[i];
        if (enemy->state == COMBATANT_STATE_INVALID)
            continue;

        count++;
    }

    int index = 0;
    for (Id i = FIRST_ENEMY_ID; i < MAX_COMBATANTS; i++)
    {
        const Combatant *enemy = &combatants[i];
        if (enemy->state == COMBATANT_STATE_INVALID)
            continue;

        const Vector3 pos = enemyPosition(enemy, count, index++);
        const Event *e = event != NULL && event->id == i ? event : NULL;

        drawEnemy(camera, enemy, pos, e);
    }
}