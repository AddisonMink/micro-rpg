#include "combatant.h"

#include "asset/asset.h"

typedef struct CombatantData
{
    CombatantType type;
    const char *name;
    SpriteId spriteId;
    int strength;
    int magic;
    int speed;
    int armor;
    int hpMax;
} CombatantData;

static const CombatantData combatantData[] = {
    [COMBATANT_TYPE_GALOOT] = {
        .type = COMBATANT_TYPE_GALOOT,
        .name = "Galoot",
        .spriteId = SPRITE_GALOOT,
        .strength = 2,
        .magic = 0,
        .speed = 1,
        .armor = 1,
        .hpMax = 8,
    },

    [COMBATANT_TYPE_MAGICIAN] = {
        .type = COMBATANT_TYPE_MAGICIAN,
        .name = "Magician",
        .spriteId = SPRITE_MAGICIAN,
        .strength = 1,
        .magic = 1,
        .speed = 2,
        .armor = 0,
        .hpMax = 6,
    },

    [COMBATANT_TYPE_SCAMP] = {
        .type = COMBATANT_TYPE_SCAMP,
        .name = "Scamp",
        .spriteId = SPRITE_SCAMP,
        .strength = 1,
        .magic = 0,
        .speed = 2,
        .armor = 0,
        .hpMax = 4,
    },
};

Combatant Combatant_Create(Id id, CombatantType type, Row row)
{
    const CombatantData *data = &combatantData[type];
    Texture2D sprite = Asset_Sprite(data->spriteId);

    return (Combatant){
        .id = id,
        .type = type,
        .name = data->name,
        .sprite = sprite,
        .state = COMBATANT_STATE_ALIVE,
        .strength = data->strength,
        .magic = data->magic,
        .speed = data->speed,
        .armor = data->armor,
        .hp = data->hpMax,
        .hpMax = data->hpMax,
        .row = row,
    };
}

CombatantRefList Combatant_EnemyList(Combatant combatants[MAX_COMBATANTS])
{
    CombatantRefList list = LIST_INIT(MAX_ENEMIES);
    for (Id i = FIRST_ENEMY_ID; i < MAX_COMBATANTS; i++)
    {
        Combatant *combatant = &combatants[i];
        if (combatant->state != COMBATANT_STATE_INVALID)
        {
            LIST_APPEND(&list, combatant);
        }
    }
    return list;
}