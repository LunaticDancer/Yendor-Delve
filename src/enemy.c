#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>
#include "colors.h"

bool IsEnemyIrrelevant(Enemy e)
{
    switch(e.enemyId)
    {
        case EN_CULTIST_SUMMON: return true;
        default: return false;
    }
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
                    .maxHealth = 0,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 30000,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
        };
        break;
        
        case ENB_BLACK_DRAGON:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Black Dragon",
                    .tileset = TL_BIG_CREATURES,
                    .tileLookupPosition = (Vector2){3,0},
                    .color = darkgray,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 10000,
                    .maxHealth = 10000,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 2500,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 150,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 1000,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 140,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 600,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 50,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 300,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 120,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 500,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 100,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
                    .maxHealth = 500,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
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
        };
        break;
    }
}