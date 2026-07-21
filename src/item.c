#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* GetItemStatSpread(Item it)    // what do you mean "item" is a keyword
{
     char* result = malloc(1);
     result[0] =  '\0'; 
     
     // scan through stats
     if(it.statBonuses.health != 0)
     {
        if (it.statBonuses.health > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.health);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Health\n");
     }
     if(it.statBonuses.stamina != 0)
     {
        if (it.statBonuses.stamina > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.stamina);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Stamina\n");
     }
     if(it.statBonuses.staminaRegen != 0)
     {
        if (it.statBonuses.staminaRegen > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.staminaRegen);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Stamina Regen\n");
     }
     if(it.statBonuses.speed != 0)
     {
        if (it.statBonuses.speed > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.speed);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Speed\n");
     }
     if(it.statBonuses.defense != 0)
     {
        if (it.statBonuses.defense > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.defense);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Defense\n");
     }
     if(it.statBonuses.armor != 0)
     {
        if (it.statBonuses.armor > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.armor);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Armour\n");
     }
     if(it.statBonuses.critMultiplier != 0)
     {
        if (it.statBonuses.critMultiplier > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%.2f", it.statBonuses.critMultiplier);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Crit Multiplier\n");
     }
     if(it.statBonuses.critRate != 0)
     {
        if (it.statBonuses.critRate > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.critRate);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Crit Rate\n");
     }
     if(it.statBonuses.mastery != 0)
     {
        if (it.statBonuses.mastery > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.mastery);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Mastery\n");
     }
     if(it.statBonuses.damageMultiplier != 0)
     {
        if (it.statBonuses.damageMultiplier > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%.2f", it.statBonuses.damageMultiplier);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Damage Multiplier\n");
     }
     if(it.statBonuses.targetPriority != 0)
     {
        if (it.statBonuses.targetPriority > 0)
        {
            result = CombineStrings(result, "+");
        }
        char str_num[6];
        sprintf(str_num, "%d", it.statBonuses.targetPriority);
        result = CombineStrings(result, str_num);
        result = CombineStrings(result, " Target Priority\n");
     }

     return result;
}

Item InitItem(ITEM_ID id)
{
    Item result;
    switch (id)
    {
        case ITEM_NONE:
        return (Item){
            id,
            "<None>",
            TL_GENERAL,
            (Vector2){1,0},
            GRAY,
            ES_EVERY,
            CreateEmptyStatBonuses(),
        };
        case ITEM_TEST:
        result = (Item){
            id,
            "Test Item",
            TL_ITEMS,
            (Vector2){0,0},
            GRAY,
            ES_EVERY,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.health = 25;
        result.statBonuses.stamina = -25;
        result.statBonuses.damageMultiplier = 0.35;
        return result;
        case ITEM_APPRENTICE_BOOTS:
        result = (Item){
            id,
            "Apprentice's Boots",
            TL_ITEMS,
            (Vector2){3,2},
            BROWN,
            ES_LEGS,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.mastery = 30;
        result.statBonuses.speed = 20;
        return result;
        case ITEM_APPRENTICE_ROBE:
        result = (Item){
            id,
            "Apprentice's Robe",
            TL_ITEMS,
            (Vector2){0,2},
            BLUE,
            ES_TORSO,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.mastery = 20;
        result.statBonuses.stamina = 100;
        result.statBonuses.staminaRegen = 100;
        return result;
        case ITEM_APPRENTICE_STAFF:
        result = (Item){
            id,
            "Apprentice's Staff",
            TL_ITEMS,
            (Vector2){2,3},
            BEIGE,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.mastery = 20;
        result.statBonuses.targetPriority = -50;
        return result;
        case ITEM_APPRENTICE_TIARA:
        result = (Item){
            id,
            "Apprentice's Tiara",
            TL_ITEMS,
            (Vector2){0,3},
            GOLD,
            ES_HEAD,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.mastery = 40;
        result.statBonuses.armor = 1;
        return result;
        case ITEM_JUGGERNAUT_ARMOR:
        result = (Item){
            id,
            "Juggernaut Armour",
            TL_ITEMS,
            (Vector2){2,2},
            DARKGRAY,
            ES_TORSO,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.health = 200;
        result.statBonuses.armor = 5;
        result.statBonuses.defense = 100;
        result.statBonuses.speed = -40;
        return result;
        case ITEM_JUGGERNAUT_GREATHAMMER:
        result = (Item){
            id,
            "Juggernaut Greathammer",
            TL_ITEMS,
            (Vector2){5,0},
            DARKGRAY,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.critRate =15;
        result.statBonuses.critMultiplier = 80;
        result.statBonuses.mastery = 60;
        result.statBonuses.speed = - 30;
        return result;
        case ITEM_JUGGERNAUT_HELM:
        result = (Item){
            id,
            "Juggernaut Helm",
            TL_ITEMS,
            (Vector2){6,2},
            DARKGRAY,
            ES_HEAD,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.armor = 3;
        result.statBonuses.defense = 60;
        result.statBonuses.speed = -20;
        return result;
        case ITEM_JUGGERNAUT_LEGGINS:
        result = (Item){
            id,
            "Juggernaut Leggings",
            TL_ITEMS,
            (Vector2){13,2},
            DARKGRAY,
            ES_LEGS,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.health = 100;
        result.statBonuses.armor = 3;
        result.statBonuses.defense = 60;
        result.statBonuses.speed = -20;
        return result;
        case ITEM_MERCENARY_AXE:
        result = (Item){
            id,
            "Mercenary's Axe",
            TL_ITEMS,
            (Vector2){6,1},
            LIGHTGRAY,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.mastery = 20;
        result.statBonuses.stamina = 400;
        result.statBonuses.damageMultiplier = 10;
        return result;
        case ITEM_MERCENARY_CUIRAS:
        result = (Item){
            id,
            "Mercenary's Cuirass",
            TL_ITEMS,
            (Vector2){2,2},
            ORANGE,
            ES_TORSO,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.armor = 1;
        result.statBonuses.defense = 40;
        result.statBonuses.staminaRegen = 40;
        return result;
        case ITEM_MERCENARY_GREAVES:
        result = (Item){
            id,
            "Mercenary's Greaves",
            TL_ITEMS,
            (Vector2){13,2},
            BEIGE,
            ES_LEGS,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.armor = 1;
        result.statBonuses.defense = 30;
        result.statBonuses.speed = 30;
        return result;
        case ITEM_MERCENARY_HELMET:
        result = (Item){
            id,
            "Mercenary's Helmet",
            TL_ITEMS,
            (Vector2){12,4},
            ORANGE,
            ES_HEAD,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.armor = 1;
        result.statBonuses.defense = 20;
        result.statBonuses.staminaRegen = 50;
        return result;
        case ITEM_THIEF_CAPE:
        result = (Item){
            id,
            "Thief's Cape",
            TL_ITEMS,
            (Vector2){7,4},
            GRAY,
            ES_TORSO,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.targetPriority = -200;
        result.statBonuses.defense = 20;
        return result;
        case ITEM_THIEF_DAGGER:
        result = (Item){
            id,
            "Thief's Dagger",
            TL_ITEMS,
            (Vector2){0,1},
            DARKGRAY,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.critRate = 25;
        result.statBonuses.critMultiplier = 40;
        result.statBonuses.speed = 50;
        return result;
        case ITEM_THIEF_HOOD:
        result = (Item){
            id,
            "Thief's Hood",
            TL_ITEMS,
            (Vector2){12,4},
            GRAY,
            ES_HEAD,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.targetPriority  = -50;
        result.statBonuses.defense = 10;
        result.statBonuses.mastery = 10;
        return result;
        case ITEM_THIEF_PANTS:
        result = (Item){
            id,
            "Thief's Pants",
            TL_ITEMS,
            (Vector2){12,2},
            DARKBROWN,
            ES_LEGS,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.speed = 40;
        result.statBonuses.staminaRegen = 100;
        return result;
        case ITEM_SOFUS_HAT:
        result = (Item){
            id,
            "Hatfus",
            TL_CREATURES,
            (Vector2){10,9},
            WHITE,
            ES_HEAD,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.mastery = 100;
        result.statBonuses.health = -150;
        result.statBonuses.targetPriority = 60;
        return result;
        case ITEM_SOFUS_LEGS:
        result = (Item){
            id,
            "Sofoot",
            TL_CREATURES,
            (Vector2){14,15},
            WHITE,
            ES_LEGS,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.speed = 100;
        result.statBonuses.defense = -60;
        result.statBonuses.targetPriority = 60;
        return result;
        case ITEM_SOFUS_TORSO:
        result = (Item){
            id,
            "Torsofus",
            TL_CREATURES,
            (Vector2){6,9},
            WHITE,
            ES_TORSO,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.stamina = 300;
        result.statBonuses.armor = -10;
        result.statBonuses.targetPriority = 60;
        return result;
        case ITEM_BATTLE_SHIELD:
        result = (Item){
            id,
            "Battle Shield",
            TL_ITEMS,
            (Vector2){9,2},
            GRAY,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
        };
        result.statBonuses.defense = 80;
        result.statBonuses.armor = 4;
        result.statBonuses.targetPriority = 100;
        result.statBonuses.speed = -20;
        return result;
    }
}