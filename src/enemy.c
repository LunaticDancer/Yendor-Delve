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
                    .speed = 300,
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
                    .speed = 150,
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
                .abilities = InitAbilities((ABILITY[]){AB_BLOFAEMYS_INSPIRE,AB_BLOFAEMYS_HASTE,AB_WAIT}, 3),
                .abilityCount = 3,
            },
            TG_WEIGHTED_RANDOM,
            AB_WAIT,
        };
        break;
    }
}