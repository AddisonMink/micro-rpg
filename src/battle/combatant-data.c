#include "combatant-data.h"

#include "../asset.h"

typedef struct CombatantData
{
    Name name;
    SpriteTag spriteTag;
    int maxHp;
    int strength;
    int magic;
    int armor;
    MagicLevel magicLevel;
} CombatantData;

static const CombatantData combatantData[NUM_COMBATANT_TYPES] = {
    [COMBATANT_PLAYER] = {
        .name = "Player",
        .spriteTag = SPRITE_NINESLICE,
        .maxHp = 20,
        .strength = 5,
        .magic = 5,
        .armor = 5,
        .magicLevel = MAGIC_MEDIUM,
    },
    [COMBATANT_SCAMP] = {
        .name = "Scamp",
        .spriteTag = SPRITE_SCAMP,
        .maxHp = 10,
        .strength = 2,
        .magic = 2,
        .armor = 2,
        .magicLevel = MAGIC_LOW,
    },
};

void CombatantInit(Combatant *combatant, CombatantId id, CombatantType type, Row row)
{
    // Metadata
    combatant->id = id;
    combatant->type = type;
    combatant->state = COMBATANT_STATE_ALIVE;
    // Values derived from type.
    combatant->name = combatantData[type].name;
    combatant->sprite = AssetSprite(combatantData[type].spriteTag);
    combatant->maxHp = combatantData[type].maxHp;
    combatant->strength = combatantData[type].strength;
    combatant->magic = combatantData[type].magic;
    combatant->armor = combatantData[type].armor;
    combatant->magicLevel = combatantData[type].magicLevel;
    // State
    combatant->hp = combatant->maxHp;
    combatant->row = row;
    for (int i = 0; i < NUM_STATUS_TYPES; i++)
    {
        combatant->status[i].duration = 0;
        combatant->status[i].magnitude = 0;
    }
}