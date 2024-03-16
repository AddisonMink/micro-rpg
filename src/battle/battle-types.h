#pragma once

#include "raylib.h"

typedef const char *Name;

typedef int Index;

typedef int CombatantId;

typedef enum CombatantState
{
    COMBATANT_STATE_ALIVE,
    COMBATANT_STATE_DEAD,
    COMBATANT_STATE_INVALID
} CombatantState;

typedef enum Row
{
    ROW_FRONT,
    ROW_BACK
} Row;

typedef enum CombatantType
{
    COMBATANT_PLAYER,
    COMBATANT_SCAMP,
} CombatantType;

#define NUM_COMBATANT_TYPES 2

typedef enum MagicLevel
{
    MAGIC_LOW,
    MAGIC_MEDIUM,
} MagicLevel;

typedef enum StatusType
{
    STATUS_HOLD,
    STATUS_SHIELD,
} StatusType;

#define NUM_STATUS_TYPES 2

typedef struct Status
{
    int duration;
    int magnitude;
} Status;

typedef struct Combatant
{
    // Metadata
    CombatantId id;
    CombatantState state;
    CombatantType type;
    // Values derived from type.
    Name name;
    Texture2D sprite;
    int maxHp;
    int strength;
    int magic;
    int armor;
    MagicLevel magicLevel;
    // State
    int hp;
    Row row;
    Status status[NUM_STATUS_TYPES];
} Combatant;

#define NUM_COMBATANTS 6
#define FIRST_ENEMY_ID 3

typedef struct _Battle
{
    Combatant combatants[NUM_COMBATANTS];
} _Battle;