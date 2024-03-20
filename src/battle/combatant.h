#pragma once

#include "../asset.h"

typedef int CombatantId;

typedef enum CombatantState
{
    COMBATANT_STATE_INVALID,
    COMBATANT_STATE_ALIVE,
    COMBATANT_STATE_DEAD,
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

typedef struct CombatantData
{
    const char *name;
    SpriteTag sprite;
    int maxHp;
    int strength;
    int magic;
    int armor;
    MagicLevel magicLevel;
} CombatantData;

typedef struct Combatant
{
    // Metadata
    CombatantId id;
    CombatantType type;
    // State
    CombatantState state;
    int hp;
    Row row;
    Status status[NUM_STATUS_TYPES];
} Combatant;

void CombatantInit(Combatant *combatant, CombatantId id, CombatantType type, Row row);

const CombatantData *CombatantGetData(CombatantType type);