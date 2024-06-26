#include "action.h"

static const Action actions[] = {
    [ACTION_ATTACK] = {
        .id = ACTION_ATTACK,
        .name = "Attack",
        .range = RANGE_MELEE,
        .magicLevel = 0,
        .cost = 0,
        .effects = {
            .data = {MAKE_EFFECT_TEMPLATE_ATTACK(false, DAMAGE_PHYSICAL)},
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },

    [ACTION_MOVE] = {
        .id = ACTION_MOVE,
        .name = "Move",
        .range = RANGE_SELF,
        .magicLevel = 0,
        .cost = 0,
        .effects = {
            .data = {MAKE_EFFECT_TEMPLATE_AUTO_MOVE(false)},
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },

    [ACTION_WAIT] = {
        .id = ACTION_WAIT,
        .name = "Wait",
        .range = RANGE_SELF,
        .magicLevel = 0,
        .cost = 0,
        .effects = {
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 0,
        },
    },

    [ACTION_OGRE_SMASH] = {
        .id = ACTION_OGRE_SMASH,
        .name = "Smash",
        .range = RANGE_MELEE,
        .magicLevel = 0,
        .cost = 0,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_ATTACK(false, DAMAGE_PHYSICAL),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },

    [ACTION_SCAMP_BLOWGUN] = {
        .id = ACTION_SCAMP_BLOWGUN,
        .name = "Blowgun",
        .range = RANGE_PROJECTILE,
        .magicLevel = 0,
        .cost = 0,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_DAMAGE(false, DAMAGE_PHYSICAL, 2),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },

    [ACTION_SCAMP_HOOK] = {
        .id = ACTION_SCAMP_HOOK,
        .name = "Hook",
        .range = RANGE_MELEE,
        .magicLevel = 0,
        .cost = 0,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_ATTACK(false, DAMAGE_PHYSICAL),
                MAKE_EFFECT_TEMPLATE_MOVE(true, DIRECTION_BACK),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 2,
        },
    },

    [ACTION_SCAMP_KNIFE] = {
        .id = ACTION_SCAMP_KNIFE,
        .name = "Knife",
        .range = RANGE_MELEE,
        .magicLevel = 0,
        .cost = 0,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_ATTACK(false, DAMAGE_PHYSICAL),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },

    [ACTION_LODESTONE_ZAP] = {
        .id = ACTION_LODESTONE_ZAP,
        .name = "Zap",
        .range = RANGE_REACH,
        .magicLevel = 1,
        .cost = 1,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_DAMAGE(false, DAMAGE_MAGICAL, 2),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },

    [ACTION_LODESTONE_PULL] = {
        .id = ACTION_LODESTONE_PULL,
        .name = "Pull",
        .range = RANGE_PROJECTILE,
        .magicLevel = 1,
        .cost = 2,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_MOVE(false, DIRECTION_FORWARD),
                MAKE_EFFECT_TEMPLATE_DAMAGE(false, DAMAGE_MAGICAL, 1),
                MAKE_EFFECT_TEMPLATE_STATUS(false, STATUS_STUCK),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 3,
        },
    },

    [ACTION_WAND_BONK] = {
        .id = ACTION_WAND_BONK,
        .name = "Bonk",
        .range = RANGE_MELEE,
        .magicLevel = 0,
        .cost = 2,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_ATTACK(false, DAMAGE_MAGICAL),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },

    [ACTION_WAND_SHOOT] = {
        .id = ACTION_WAND_SHOOT,
        .name = "Shoot",
        .range = RANGE_PROJECTILE,
        .magicLevel = 1,
        .cost = 1,
        .effects = {
            .data = {
                MAKE_EFFECT_TEMPLATE_DAMAGE(false, DAMAGE_MAGICAL, 1),
            },
            .capacity = MAX_EFFECT_TEMPLATES,
            .count = 1,
        },
    },
};

const Action *Action_Load(ActionId id)
{
    return &actions[id];
}