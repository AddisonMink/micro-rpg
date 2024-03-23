#include "combatant.h"

#include "asset/asset.h"

static const CombatantData combatantData[NUM_COMBATANT_TYPES] = {
    [COMBATANT_PLAYER] = {
        .name = "Player",
        .sprite = SPRITE_MAGICIAN,
        .maxHp = 8,
        .strength = 5,
        .magic = 5,
        .armor = 5,
        .magicLevel = MAGIC_MEDIUM,
    },
    [COMBATANT_SCAMP] = {
        .name = "Scamp",
        .sprite = SPRITE_SCAMP,
        .maxHp = 3,
        .strength = 2,
        .magic = 2,
        .armor = 2,
        .magicLevel = MAGIC_LOW,
    },
};

void CombatantInit(Combatant *combatant, CombatantId id, CombatantType type, Row row)
{
    AssetLoadSprite(combatantData[type].sprite);
    const CombatantData *data = CombatantGetData(type);

    // Metadata
    combatant->id = id;
    combatant->type = type;
    // State
    combatant->state = COMBATANT_STATE_ALIVE;
    combatant->hp = data->maxHp;
    combatant->row = row;
    for (int i = 0; i < NUM_STATUS_TYPES; i++)
    {
        combatant->status[i].duration = 0;
        combatant->status[i].magnitude = 0;
    }
}

const CombatantData *CombatantGetData(CombatantType type)
{
    return &combatantData[type];
}