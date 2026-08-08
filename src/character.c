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
            "As reckless as he is effective, the Berserker dominates his opponents through sheer force and rage, endangering himself beyond reason just to chase the thrill of murder.",
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
                    50,
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
                InitAbilities((ABILITY[]){AB_WAIT}, 1),
                0,
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
            "Mysterious and elusive, the Assassin is a frail but tactical fighter, waiting for the right opportunity to deal a decisive strike.",
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
                    100,
                    80,
                    20,
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
                InitAbilities((ABILITY[]){AB_WAIT}, 1),
                0,
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
            "Speed and precission is what the Duelist is known for. This highly trained fighter becomes more deadly the longer the fight lasts.",
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
                    100,
                    100,
                    0,
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
                InitAbilities((ABILITY[]){AB_WAIT}, 1),
                0,
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
            "The Monk is a practitioner of wisdom and restraint. Less is more.",
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
                    60,
                    120,
                    100,
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
                InitAbilities((ABILITY[]){AB_WAIT}, 1),
                0,
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
            "A newly born abominaton, an unlikely aliance. The Flesh Golem is weak, but it absorbs every creature it kills, growing with limitless potential.",
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
                    100,
                    100,
                    0,
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
                InitAbilities((ABILITY[]){AB_WAIT}, 1),
                0,
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

     result = CombineStrings(result, "\nMastery: ");
     sprintf(str_num, "%d", ch.stats.baseStats.mastery + ch.stats.itemStats.mastery);
    result = CombineStrings(result, str_num);

     result = CombineStrings(result, "\nTarget Priority: ");
     sprintf(str_num, "%d", ch.stats.baseStats.targetPriority + ch.stats.itemStats.targetPriority);
    result = CombineStrings(result, str_num);

     return result;
}