#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>
#include "colors.h"

bool IsEnemyIrrelevant(Enemy e)
{
    return e.optionalToKill;
}

Enemy InitEnemyData(ENEMY_ID id)
{
    switch(id)
    {
        case EN_NONE:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Empty Space",
                    .tileset = TL_GENERAL,
                    .tileLookupPosition = (Vector2){1,0},
                    .color = gray,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 0,
                    .health = 0,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 100,
                    .speed = 100,
                    .defense = 0,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 0,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_WAIT},1),
                .abilityCount = 1,
            },
            TG_TRUE_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case ENB_DUNGEON_MASTER:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Dungeon Master",
                    .tileset = TL_BIG_CREATURES,
                    .tileLookupPosition = (Vector2){5,3},
                    .color = gold,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 30000,
                    .health = 30000,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 300,
                    .speed = 400,
                    .defense = 150,
                    .armor = 5,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CULTIST_SPAWN_ENROOT, AB_CULTIST_SPAWN_SPORES,AB_CULTIST_SPAWN_GROW,AB_CULTIST_SPAWN_INVIGORATE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case ENB_BLACK_DRAGON:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Black Dragon",
                    .tileset = TL_BIG_COMMUNITY,
                    .tileLookupPosition = (Vector2){6,0},
                    .color = darkgray,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 10000,
                    .health = 10000,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 250,
                    .defense = 250,
                    .armor = 10,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CULTIST_SPAWN_ENROOT, AB_CULTIST_SPAWN_SPORES,AB_CULTIST_SPAWN_GROW,AB_CULTIST_SPAWN_INVIGORATE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case ENB_CAT_WIZARD:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Nebulon the Archmage",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){0,9},
                    .color = purple,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 2500,
                    .health = 2500,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 300,
                    .defense = 50,
                    .armor = 1,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CAT_WIZARD_ARCANE_BLAST, AB_CAT_WIZARD_CURSE_OF_MEEK, AB_CAT_WIZARD_HEX_OF_AMBITION,
                    AB_CAT_WIZARD_THOUSAND_SCRATCHES, AB_CAT_WIZARD_CRUSH, AB_CAT_WIZARD_ERECT_PILLAR, AB_CAT_WIZARD_ERECT_PILLAR,AB_WAIT}, 8),
                .abilityCount = 8,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case ENB_CAT_WIZARD_PILLAR:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Arcane Resonator",
                    .tileset = TL_COMMUNITY,
                    .tileLookupPosition = (Vector2){1,2},
                    .color = skyblue,

                    .ticksUntilNextTurn = 500,
                    .currentHealth = 300,
                    .health = 300,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 100,
                    .defense = 200,
                    .armor = 5,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CAT_WIZARD_PILLAR_ZAP,AB_CAT_WIZARD_PILLAR_OVERCLOCK,AB_CAT_WIZARD_PILLAR_AMPLIFY,AB_WAIT}, 4),
                .abilityCount = 4,
                .creatureEffect = LE_ONABIL_CAT_WIZARD_PILLAR,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "The arcane pillar crumbles.",
            true,
        };
        break;
        
        case ENB_GREEN_DRAGON:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Green Dragon",
                    .tileset = TL_BIG_COMMUNITY,
                    .tileLookupPosition = (Vector2){2,0},
                    .color = green,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 2500,
                    .health = 2500,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 150,
                    .defense = 150,
                    .armor = 5,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CULTIST_SPAWN_ENROOT, AB_CULTIST_SPAWN_SPORES,AB_CULTIST_SPAWN_GROW,AB_CULTIST_SPAWN_INVIGORATE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case ENB_CHOIR_LEFT:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Choir",
                    .tileset = TL_BIG_COMMUNITY,
                    .tileLookupPosition = (Vector2){3,0},
                    .color = orange,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 900,
                    .health = 900,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 150,
                    .defense = 100,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CULTIST_SPAWN_ENROOT, AB_CULTIST_SPAWN_SPORES,AB_CULTIST_SPAWN_GROW,AB_CULTIST_SPAWN_INVIGORATE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case ENB_CHOIR_RIGHT:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Choir",
                    .tileset = TL_BIG_COMMUNITY,
                    .tileLookupPosition = (Vector2){5,0},
                    .color = orange,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 900,
                    .health = 900,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 150,
                    .defense = 100,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CULTIST_SPAWN_ENROOT, AB_CULTIST_SPAWN_SPORES,AB_CULTIST_SPAWN_GROW,AB_CULTIST_SPAWN_INVIGORATE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case ENB_CHOIR_PREACHER:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Choir",
                    .tileset = TL_BIG_COMMUNITY,
                    .tileLookupPosition = (Vector2){4,0},
                    .color = orange,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1500,
                    .health = 1500,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 150,
                    .defense = 100,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CULTIST_SPAWN_ENROOT, AB_CULTIST_SPAWN_SPORES,AB_CULTIST_SPAWN_GROW,AB_CULTIST_SPAWN_INVIGORATE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;

        case EN_CULTIST_SUMMON:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Mindless Spawn",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){10,14},
                    .color = darkpurple,

                    .ticksUntilNextTurn = 100,
                    .currentHealth = 150,
                    .health = 150,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 150,
                    .defense = 50,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CULTIST_SPAWN_ENROOT, AB_CULTIST_SPAWN_SPORES,AB_CULTIST_SPAWN_GROW,AB_CULTIST_SPAWN_INVIGORATE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            true,
        };
        break;
        
        case EN_MIMIC:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Mimic",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){5,14},
                    .color = brown,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1000,
                    .health = 1000,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 200,
                    .speed = 350,
                    .defense = 150,
                    .armor = 5,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 600,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_MIMIC_CHOMP,AB_MIMIC_IMPALE,AB_MIMIC_PETRIFY,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_BLOOD_FAE_WARRIOR:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Blood Fae Warrior",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){12,3},
                    .color = red,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .health = 200,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 250,
                    .defense = 130,
                    .armor = 1,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_BLOFAEWAR_CUT,AB_WAIT}, 2),
                .abilityCount = 2,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_BLOOD_FAE_MYSTIC:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Blood Fae Mystic",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){13,3},
                    .color = red,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 140,
                    .health = 140,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 400,
                    .defense = 110,
                    .armor = 3,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_BLOFAEMYS_INSPIRE,AB_BLOFAEMYS_HASTE,AB_BLOFAEMYS_MOCK,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_CAUSTIC_CUBE:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Caustic Cube",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){6,14},
                    .color = lime,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 600,
                    .health = 600,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 150,
                    .defense = 80,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 600,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CAUCUBE_NIBBLE,AB_CAUCUBE_SPLIT,AB_CAUCUBE_METABOLIZE,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_CAUSTIC_CUBE_CLONE:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Caustic Cubeling",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){6,14},
                    .color = green,

                    .ticksUntilNextTurn = 300,
                    .currentHealth = 50,
                    .health = 50,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 120,
                    .defense = 20,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 50,
                    .targetPriority = 300,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_CAUCUBE_NIBBLE,AB_WAIT}, 2),
                .abilityCount = 2,
            },
            TG_FRONT,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_TOOTH_FAIRY:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Tooth Fairy",
                    .tileset = TL_COMMUNITY,
                    .tileLookupPosition = (Vector2){4,4},
                    .color = raywhite,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 300,
                    .health = 300,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 100,
                    .defense = 100,
                    .armor = 2,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_TOOFAIRY_SUMMON,AB_TOOFAIRY_CALCINIZE,AB_TOOFAIRY_BONEDUST,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_TOOTH_GOLEM:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Tooth Golem",
                    .tileset = TL_COMMUNITY,
                    .tileLookupPosition = (Vector2){5,4},
                    .color = raywhite,

                    .ticksUntilNextTurn = 400,
                    .currentHealth = 120,
                    .health = 120,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 150,
                    .defense = 50,
                    .armor = 2,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 800,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_TOOTHOLEM_STRIKE,AB_TOOTHOLEM_HARDEN,AB_TOOTHOLEM_PROTECT,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_FRONT,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_STARVING_REVENANT:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Starving Revenant",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){4,6},
                    .color = skyblue,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .health = 200,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 200,
                    .defense = 300,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 1,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_STEVENANT_SIPHON, AB_STEVENANT_PHASING_STRIKE,AB_STEVENANT_ECTOPLASMIC_MANIFESTATION,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_TRUE_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_BANDIT_STRONGMAN:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Strongman",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){0,4},
                    .color = orange,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 500,
                    .health = 500,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 80,
                    .defense = 100,
                    .armor = 1,
                    .critBonus = 100,
                    .critRate = 1,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_BANDIT_STRONGMAN_SLAM,AB_WAIT}, 2),
                .abilityCount = 2,
            },
            TG_TRUE_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_BANDIT_CUTTHROAT:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Cutpurse",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){0,1},
                    .color = lightgray,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .health = 200,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 150,
                    .defense = 50,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 1,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_BANDIT_CUTPURSE_SHIV, AB_BANDIT_CUTPURSE_DISCOMBOBULATE,AB_BANDIT_CUTPURSE_INVIGORATING_SMOKE,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_BACK,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_BANDIT_JESTER:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Jester",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){5,0},
                    .color = maroon,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 100,
                    .health = 100,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 260,
                    .defense = 500,
                    .armor = 0,
                    .critBonus = 100,
                    .critRate = 1,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_BANDIT_JESTER_MOCK, AB_BANDIT_JESTER_BACKFLIP,AB_BANDIT_JESTER_PRANK,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        
        case EN_GORLAK:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Gorlak",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){8,15},
                    .color = magenta,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 500,
                    .health = 500,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 140,
                    .defense = 100,
                    .armor = 2,
                    .critBonus = 100,
                    .critRate = 1,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_GORLAK_FLURRY,AB_GORLAK_CLAP,AB_GORLAK_EQUIP,AB_GORLAK_POSE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            TG_WEIGHTED_RANDOM,
            AB_GORLAK_EQUIP,
            "The Gorlak nods approvingly and disappears in a puff of magic.",
            false,
        };
        break;

        case EN_ICE_ELEMENTAL:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Ice Elemental",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){11,6},
                    .color = skyblue,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 400,
                    .health = 400,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 120,
                    .defense = 100,
                    .armor = 2,
                    .critBonus = 100,
                    .critRate = 1,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_ICE_ELEMENTAL_FREEZE,AB_ICE_ELEMENTAL_SHARDS,AB_ICE_ELEMENTAL_AURA,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        case EN_FIRE_ELEMENTAL:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Fire Elemental",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){12,6},
                    .color = orange,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 300,
                    .health = 300,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 175,
                    .speed = 200,
                    .defense = 90,
                    .armor = 1,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_FIRE_ELEMENTAL_FIREBALL,AB_FIRE_ELEMENTAL_FLARE,AB_FIRE_ELEMENTAL_INFERNO,AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        case EN_MANRAT:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Manrat",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){14,3},
                    .color = lightgray,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 180,
                    .health = 180,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 150,
                    .speed = 150,
                    .defense = 120,
                    .armor = 1,
                    .critBonus = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 600,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_MANRAT_RALLYING_ASSAULT,AB_MANRAT_STRUGGLE,AB_MANRAT_PINPOINT,AB_WAIT}, 4),
                .abilityCount = 4,
                .creatureEffect = LE_ONDTH_MANRAT,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        case EN_GIANT_FROG:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Giant Frog",
                    .tileset = TL_COMMUNITY,
                    .tileLookupPosition = (Vector2){4,6},
                    .color = darkblue,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1000,
                    .health = 1000,
                    .currentStamina = 1000,
                    .stamina = 1000,
                    .staminaRegen = 255,
                    .speed = 200,
                    .defense = 200,
                    .armor = 3,
                    .critBonus = 100,
                    .critRate = 35,
                    .critCounter = 0,
                    .mastery = 30,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_FROG_DEVOUR, AB_FROG_STOMP, AB_FROG_SING, AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        case EN_FROG_STINKFLY:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Stinkfly",
                    .tileset = TL_COMMUNITY,
                    .tileLookupPosition = (Vector2){5,6},
                    .color = brown,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 400,
                    .health = 400,
                    .currentStamina = 1000,
                    .stamina = 500,
                    .staminaRegen = 1000,
                    .speed = 300,
                    .defense = 110,
                    .armor = 0,
                    .critBonus = 0,
                    .critRate = 25,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 100,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_FROG_FLY_BUZZ, AB_FROG_FLY_STINK, AB_WAIT}, 3),
                .abilityCount = 3,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "\0",
            false,
        };
        break;
        break;
        case EN_QUICKPIG:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Quickpig",
                    .tileset = TL_COMMUNITY,
                    .tileLookupPosition = (Vector2){1,0},
                    .color = brown,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 800,
                    .health = 800,
                    .currentStamina = 200,
                    .stamina = 200,
                    .staminaRegen = 1000,
                    .speed = 240,
                    .defense = 240,
                    .armor = 0,
                    .critBonus = 40,
                    .critRate = 40,
                    .critCounter = 0,
                    .mastery = 100,
                    .targetPriority = 50,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_QPIG_ROB, AB_QPIG_SPRINT, AB_QPIG_TACKLE, AB_WAIT}, 4),
                .abilityCount = 4,
            },
            TG_WEIGHTED_RANDOM,
            AB_QPIG_ROB,
            "\0",
            false,
        };
        break;
        case EN_PIGJAR:
        Color* pickedcolor[16] = {beige, beige, beige, beige, brown, brown, brown, brown, orange, red, skyblue, raywhite, gold, lightgray, lime, purple};
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Jar",
                    .tileset = TL_COMMUNITY,
                    .tileLookupPosition = (rand() % 8 == 0)? (Vector2){2,0} : (Vector2){3,0},
                    .color = pickedcolor[(rand() % 16)],

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 4,
                    .health = 4,
                    .currentStamina = 0,
                    .stamina = 0,
                    .staminaRegen = 0,
                    .speed = 0,
                    .defense = 250,
                    .armor = 15,
                    .critBonus = 500,
                    .critRate = 0,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 100,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_WAIT}, 1),
                .abilityCount = 1,
                .creatureEffect = LE_ONDTH_JAR_BREAK_GOODIES,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
            "The jar shatters into pieces.",
            true,
        };
        break;
    }
}
