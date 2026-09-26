#include "character.h"
#include <stdlib.h>
#include <stdio.h>

Character InitCharacterData(CHARACTER_ID id)
{
    switch (id)
    {
        case CHAR_BERSERKER:
        return (Character){
            id,
            "As reckless as he is effective, the Berserker dominates his opponents through sheer force and rage, endangering himself beyond reason just to chase the thrill of murder.\nThe Berserker is all about calculated risks, enraging himself which amplifies both outgoing and incoming damage. His brute strength is capable of incapacitating his enemies or killing them outright, but is held back by his lack of speed.",
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Berserker",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){6, 0},
                    .color = RED,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1500,
                    .maxHealth = 1500,
                    .currentStamina = 600,
                    .maxStamina = 600,
                    .staminaRegen = 100,
                    .speed = 100,
                    .defense = 60,
                    .armor = 1,
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
                .abilities = InitAbilities((ABILITY[]){AB_BERSERKER_SWING,AB_BERSERKER_BASH,AB_BERSERKER_BATTLECRY,AB_BERSERKER_BRACE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
        };
        break;
        case CHAR_ASSASSIN:
        return (Character){
            id,
            "Mysterious and elusive, the Assassin is a frail but tactical fighter, waiting for the right opportunity to deal a decisive strike.\nThe Assassin is a tricky character, specializing in stealth, capitalizing on critical opportunities and bleeding the opponent to death.",
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Assassin",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){1, 1},
                    .color = GRAY,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 600,
                    .maxHealth = 600,
                    .currentStamina = 1200,
                    .maxStamina = 1200,
                    .staminaRegen = 150,
                    .speed = 110,
                    .defense = 40,
                    .armor = 0,
                    .critMultiplier = 125,
                    .critRate = 25,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 400,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_ASSASSIN_SLASH, AB_ASSASSIN_PREPARE, AB_ASSASSIN_CONCEAL, AB_ASSASSIN_REND,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
        };
        break;
        case CHAR_DUELIST:
        return (Character){
            id,
            "Speed and precission is what the Duelist is known for. This highly trained fighter becomes more deadly the longer the fight lasts.\nThe Duelist focuses on abilities bolstering her own speed, letting her take more turns between enemy actions.",
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Duelist",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){7, 0},
                    .color = GOLD,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1000,
                    .maxHealth = 1000,
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
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_DUELIST_LUNGE,AB_DUELIST_OPPORTUNITY, AB_DUELIST_PARRY, AB_DUELIST_BREATH,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
        };
        break;
        case CHAR_MONK:
        return (Character){
            id,
            "The Monk is a practitioner of wisdom and restraint. Less is more.\nThis higly strategic character boasts very expensive but also extremely potent abilities, which also become area effects upon critical activation.",
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Monk",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){2, 7},
                    .color = GREEN,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1000,
                    .maxHealth = 1000,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 100,
                    .defense = 60,
                    .armor = 0,
                    .critMultiplier = 100,
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
                .abilities = InitAbilities((ABILITY[]){AB_MONK_MEDITATE, AB_MONK_TRUE_STRIKE, AB_MONK_ATTUNEMENT, AB_MONK_CLEANSE, AB_WAIT}, 5),
                .abilityCount = 5,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
        };
        break;
        case CHAR_SHAPESHIFTER:
        return (Character){
            id,
            "Blessed to share a shape with all of creation. Cursed to lose its sense of self.\nThis character can freely transform into enemies, giving you access to their unique abilities.",
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Shapeshifter",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){8, 0},
                    .color = BEIGE,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 400,
                    .maxHealth = 400,
                    .currentStamina = 100,
                    .maxStamina = 100,
                    .staminaRegen = 150,
                    .speed = 200,
                    .defense = 100,
                    .armor = 1,
                    .critMultiplier = 100,
                    .critRate = 15,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 500,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_SHAPESHIFTER_SCRATCH, AB_SHAPESHIFTER_TRANSFORM, AB_WAIT}, 3),
                .abilityCount = 3,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
        };
        break;
        case CHAR_FLESH_GOLEM:
        return (Character){
            id,
            "A newly born abominaton, an unlikely aliance. The Flesh Golem is weak, but inaction allows it to grow with limitless potential.\nIt gains permanent health upgrades for each unique skill not used during combat.",
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Flesh Golem",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){5, 4},
                    .color = MAROON,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 200,
                    .maxHealth = 200,
                    .currentStamina = 1000,
                    .maxStamina = 1000,
                    .staminaRegen = 150,
                    .speed = 90,
                    .defense = 10,
                    .armor = 0,
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
                .abilities = InitAbilities((ABILITY[]){AB_FOLEM_STRIKE,AB_FOLEM_EXPUNGE, AB_FOLEM_EPIDERMIZE,AB_FOLEM_CRIPPLE,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
        };
        break;
        case CHAR_MAGUS:
        return (Character){
            id,
            "Master of the arcane, ascendant beyond mortal limits.\nMagus is an extremely potent but frail spellcaster, using their Stamina as a makeshift shield.",
            (CreatureStats){
                .baseStats = (CreatureBaseStats){
                    .name = "Magus",
                    .tileset = TL_CREATURES,
                    .tileLookupPosition = (Vector2){10, 6},
                    .color = PURPLE,

                    .ticksUntilNextTurn = 0,
                    .currentHealth = 1,
                    .maxHealth = 1,
                    .currentStamina = 500,
                    .maxStamina = 500,
                    .staminaRegen = 150,
                    .speed = 100,
                    .defense = 0,
                    .armor = 0,
                    .critMultiplier = 100,
                    .critRate = 10,
                    .critCounter = 0,
                    .mastery = 0,
                    .targetPriority = 400,
                },
                .statusEffects = {},
                .encounterStats = CreateEmptyStatBonuses(),
                .itemStats = CreateEmptyStatBonuses(),
                .temporaryStats = {},
                .lingeringEffects = {},
                .abilities = InitAbilities((ABILITY[]){AB_MAGUS_DISINTEGRATE,AB_MAGUS_ARCANE_BLAST,AB_MAGUS_TUTOR,AB_MAGUS_SARCOPHAGUS,AB_WAIT}, 5),
                .abilityCount = 5,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
        };
        break;
    }
}

void EquipItem(Character* ch, ITEM_ID it, char slot)
{
    UnequipItem(ch, slot);

    (*ch).items[slot] = InitItem(it);

    (*ch).stats.itemStats.armor += (*ch).items[slot].statBonuses.armor;
    (*ch).stats.itemStats.critMultiplier += (*ch).items[slot].statBonuses.critMultiplier;
    (*ch).stats.itemStats.critRate += (*ch).items[slot].statBonuses.critRate;
    (*ch).stats.itemStats.damageMultiplier += (*ch).items[slot].statBonuses.damageMultiplier;
    (*ch).stats.itemStats.defense += (*ch).items[slot].statBonuses.defense;
    (*ch).stats.itemStats.health += (*ch).items[slot].statBonuses.health;
    (*ch).stats.itemStats.mastery += (*ch).items[slot].statBonuses.mastery;
    (*ch).stats.itemStats.speed += (*ch).items[slot].statBonuses.speed;
    (*ch).stats.itemStats.stamina += (*ch).items[slot].statBonuses.stamina;
    (*ch).stats.itemStats.staminaRegen += (*ch).items[slot].statBonuses.staminaRegen;
    (*ch).stats.itemStats.targetPriority += (*ch).items[slot].statBonuses.targetPriority;

    if((*ch).stats.baseStats.currentHealth > (*ch).stats.itemStats.health + (*ch).stats.baseStats.maxHealth)
    {
        (*ch).stats.baseStats.currentHealth = (*ch).stats.itemStats.health + (*ch).stats.baseStats.maxHealth;
    }
}

void UnequipItem(Character* ch, char slot)
{
    (*ch).stats.itemStats.armor -= (*ch).items[slot].statBonuses.armor;
    (*ch).stats.itemStats.critMultiplier -= (*ch).items[slot].statBonuses.critMultiplier;
    (*ch).stats.itemStats.critRate -= (*ch).items[slot].statBonuses.critRate;
    (*ch).stats.itemStats.damageMultiplier -= (*ch).items[slot].statBonuses.damageMultiplier;
    (*ch).stats.itemStats.defense -= (*ch).items[slot].statBonuses.defense;
    (*ch).stats.itemStats.health -= (*ch).items[slot].statBonuses.health;
    (*ch).stats.itemStats.mastery -= (*ch).items[slot].statBonuses.mastery;
    (*ch).stats.itemStats.speed -= (*ch).items[slot].statBonuses.speed;
    (*ch).stats.itemStats.stamina -= (*ch).items[slot].statBonuses.stamina;
    (*ch).stats.itemStats.staminaRegen -= (*ch).items[slot].statBonuses.staminaRegen;
    (*ch).stats.itemStats.targetPriority -= (*ch).items[slot].statBonuses.targetPriority;

    if((*ch).stats.baseStats.currentHealth > (*ch).stats.itemStats.health + (*ch).stats.baseStats.maxHealth)
    {
        (*ch).stats.baseStats.currentHealth = (*ch).stats.itemStats.health + (*ch).stats.baseStats.maxHealth;
    }

    (*ch).items[slot] = InitItem(ITEM_NONE);
}

char* GetCharacterStatsRundown(Character ch)
{
     char* result = malloc(1);
     result[0] =  '\0'; 
     char str_num[6];

     result = CombineStrings(result, "Health: ");
     sprintf(str_num, "%d", ch.stats.baseStats.currentHealth);
    result = CombineStrings(result, str_num);
     result = CombineStrings(result, "/");
     sprintf(str_num, "%d", ch.stats.baseStats.maxHealth + ch.stats.itemStats.health);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nStamina: ");
     sprintf(str_num, "%d", ch.stats.baseStats.maxStamina + ch.stats.itemStats.stamina);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nStamina regeneration: ");
     sprintf(str_num, "%d", ch.stats.baseStats.staminaRegen + ch.stats.itemStats.staminaRegen);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nSpeed: ");
     sprintf(str_num, "%d", ch.stats.baseStats.speed + ch.stats.itemStats.speed);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nDefense: ");
     sprintf(str_num, "%d", ch.stats.baseStats.defense + ch.stats.itemStats.defense);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nArmour: ");
     sprintf(str_num, "%d", ch.stats.baseStats.armor + ch.stats.itemStats.armor);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nCrit Rate: ");
     sprintf(str_num, "%d", ch.stats.baseStats.critRate + ch.stats.itemStats.critRate);
    result = CombineStrings(result, str_num);
     result = CombineStrings(result, "%");

     result = CombineStrings(result, "\nCrit Bonus: ");
     sprintf(str_num, "%.0f", ch.stats.baseStats.critMultiplier + ch.stats.itemStats.critMultiplier);
    result = CombineStrings(result, str_num);
     result = CombineStrings(result, "%");

     result = CombineStrings(result, "\nMastery: ");
     sprintf(str_num, "%d", ch.stats.baseStats.mastery + ch.stats.itemStats.mastery);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nTarget Priority: ");
     sprintf(str_num, "%d", ch.stats.baseStats.targetPriority + ch.stats.itemStats.targetPriority);
    result = CombineStrings(result, str_num);

     return result;
}