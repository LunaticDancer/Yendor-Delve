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
                (CreatureBaseStats){
                    "Berserker",
                    TL_CREATURES,
                    (Vector2){6, 0},
                    RED,

                    0,
                    1400,
                    1400,
                    1000,
                    1000,
                    150,
                    100,
                    10,
                    1,
                    100,
                    5,
                    0,
                    0,
                    600,
                },
                {},
                CreateEmptyStatBonuses(),
                CreateEmptyStatBonuses(),
                InitAbilities((ABILITY[]){AB_BERSERKER_SWING,AB_BERSERKER_BASH,AB_BERSERKER_BATTLECRY,AB_BERSERKER_BRACE,AB_WAIT}, 5),
                5,
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
                (CreatureBaseStats){
                    "Assassin",
                    TL_CREATURES,
                    (Vector2){1, 1},
                    GRAY,

                    0,
                    600,
                    600,
                    1200,
                    1200,
                    150,
                    110,
                    40,
                    0,
                    125,
                    25,
                    0,
                    0,
                    400,
                },
                {},
                CreateEmptyStatBonuses(),
                CreateEmptyStatBonuses(),
                InitAbilities((ABILITY[]){AB_ASSASSIN_SLASH, AB_ASSASSIN_PREPARE, AB_ASSASSIN_CONCEAL, AB_ASSASSIN_REND,AB_WAIT}, 5),
                5,
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
                (CreatureBaseStats){
                    "Duelist",
                    TL_CREATURES,
                    (Vector2){7, 0},
                    GOLD,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    150,
                    100,
                    20,
                    0,
                    100,
                    5,
                    0,
                    0,
                    500,
                },
                {},
                CreateEmptyStatBonuses(),
                CreateEmptyStatBonuses(),
                InitAbilities((ABILITY[]){AB_DUELIST_LUNGE,AB_DUELIST_OPPORTUNITY, AB_DUELIST_PARRY, AB_DUELIST_BREATH,AB_WAIT}, 5),
                5,
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
                (CreatureBaseStats){
                    "Monk",
                    TL_CREATURES,
                    (Vector2){2, 7},
                    GREEN,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    150,
                    100,
                    60,
                    0,
                    100,
                    5,
                    0,
                    0,
                    500,
                },
                {},
                CreateEmptyStatBonuses(),
                CreateEmptyStatBonuses(),
                InitAbilities((ABILITY[]){AB_MONK_MEDITATE, AB_MONK_TRUE_STRIKE, AB_MONK_ATTUNEMENT, AB_MONK_CLEANSE, AB_WAIT}, 5),
                5,
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
                (CreatureBaseStats){
                    "Shapeshifter",
                    TL_CREATURES,
                    (Vector2){8, 0},
                    BEIGE,

                    0,
                    400,
                    400,
                    100,
                    100,
                    150,
                    200,
                    100,
                    1,
                    100,
                    15,
                    0,
                    0,
                    500,
                },
                {},
                CreateEmptyStatBonuses(),
                CreateEmptyStatBonuses(),
                InitAbilities((ABILITY[]){AB_SHAPESHIFTER_SCRATCH, AB_SHAPESHIFTER_TRANSFORM, AB_WAIT}, 3),
                3,
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
                (CreatureBaseStats){
                    "Flesh Golem",
                    TL_CREATURES,
                    (Vector2){5, 4},
                    MAROON,

                    0,
                    400,
                    400,
                    1000,
                    1000,
                    150,
                    90,
                    10,
                    0,
                    100,
                    5,
                    0,
                    0,
                    500,
                },
                {},
                CreateEmptyStatBonuses(),
                CreateEmptyStatBonuses(),
                InitAbilities((ABILITY[]){AB_FOLEM_STRIKE,AB_FOLEM_EXPUNGE, AB_FOLEM_EPIDERMIZE,AB_FOLEM_CRIPPLE,AB_WAIT}, 5),
                5,
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