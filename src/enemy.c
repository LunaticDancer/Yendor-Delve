#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>

Enemy InitEnemyData(ENEMY_ID id)
{
    switch(id)
    {
        case EN_NONE:
        return (Enemy){
            id,
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "None",
                    .tileset = TL_GENERAL,
                    .tileLookupPosition = (Vector2){1,0},
                    .color = GRAY,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 0,
                    .maxHealth = 0,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 100,
                    .speed = 100,
                    .defense = 50,
                    .armor = 1,
                    .critMultiplier = 100,
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
                .abilities = (Ability*)malloc(0),
                .abilityCount = 0,
            },
            TG_TRUE_RANDOM,
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
                    .color = BROWN,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1000,
                    .maxHealth = 1000,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 100,
                    .speed = 100,
                    .defense = 50,
                    .armor = 1,
                    .critMultiplier = 100,
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
                .abilities = InitAbilities((ABILITY[]){AB_WAIT}, 1),
                .abilityCount = 1,
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
                    .color = RED,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 250,
                    .defense = 130,
                    .armor = 1,
                    .critMultiplier = 100,
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
                    .color = RED,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 140,
                    .maxHealth = 140,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 400,
                    .defense = 110,
                    .armor = 3,
                    .critMultiplier = 100,
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
                    .color = GREEN,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 600,
                    .maxHealth = 600,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 300,
                    .defense = 80,
                    .armor = 0,
                    .critMultiplier = 100,
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
                .abilities = InitAbilities((ABILITY[]){AB_SHAPESHIFTER_SCRATCH,AB_WAIT}, 2),
                .abilityCount = 2,
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
                    .color = LIME,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 100,
                    .defense = 20,
                    .armor = 0,
                    .critMultiplier = 100,
                    .critRate = 5,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 300,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_SHAPESHIFTER_SCRATCH,AB_WAIT}, 2),
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
                    .color = RAYWHITE,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 300,
                    .maxHealth = 300,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 120,
                    .defense = 100,
                    .armor = 2,
                    .critMultiplier = 100,
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
                .abilities = InitAbilities((ABILITY[]){AB_SHAPESHIFTER_SCRATCH,AB_WAIT}, 2),
                .abilityCount = 2,
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
                    .color = RAYWHITE,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 200,
                    .defense = 50,
                    .armor = 2,
                    .critMultiplier = 100,
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
                .abilities = InitAbilities((ABILITY[]){AB_SHAPESHIFTER_SCRATCH,AB_WAIT}, 2),
                .abilityCount = 2,
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
                    .color = SKYBLUE,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 200,
                    .defense = 300,
                    .armor = 0,
                    .critMultiplier = 100,
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
                .abilities = InitAbilities((ABILITY[]){AB_SHAPESHIFTER_SCRATCH,AB_WAIT}, 2),
                .abilityCount = 2,
            },
            TG_TRUE_RANDOM,
            AB_WAIT,
        };
        break;
    }
}