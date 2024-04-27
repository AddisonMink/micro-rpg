#pragma once

#include "raylib.h"

#include "common/list-macros.h"

#define MAX_COMBATANTS 6
#define MAX_PLAYERS 3
#define MAX_ENEMIES 3
#define FIRST_ENEMY_ID 3

typedef int Id;

typedef enum CombatantType
{
    COMBATANT_TYPE_GALOOT,
    COMBATANT_TYPE_MAGICIAN,
    COMBATANT_TYPE_SCAMP,
} CombatantType;

typedef enum CombatantState
{
    COMBATANT_STATE_INVALID,
    COMBATANT_STATE_ALIVE,
    COMBATANT_STATE_DEAD,
} CombatantState;

typedef enum Row
{
    ROW_FRONT,
    ROW_BACK,
} Row;

#define NUM_ROWS 2

typedef struct Combatant
{
    Id id;
    CombatantType type;
    const char *name;
    Texture2D sprite;
    CombatantState state;
    int strength;
    int magic;
    int speed;
    int armor;
    int hpMax;
    int hp;
    Row row;
} Combatant;

STATIC_LIST_TYPE(CombatantRefList, Combatant *, MAX_COMBATANTS);

Combatant Combatant_Create(Id id, CombatantType type, Row row);

CombatantRefList Combatant_EnemyList(Combatant combatants[MAX_COMBATANTS]);