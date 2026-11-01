#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"

char* GetItemStatSpread(Item it)
{
    char* result = malloc(1);
    result[0] = '\0';
    char str_num[8];

    char* abilityDesc = GetLingeringEffectDescription(it.triggerAbility);
    if(abilityDesc != NULL)
    {
        result = CombineStrings(abilityDesc, "\n");
    }

    #define GET_INT(field, label)\
        do {\
            if (it.statBonuses.field != 0) {\
                if (it.statBonuses.field > 0) result = CombineStrings(result, "+");\
                sprintf(str_num, "%d", it.statBonuses.field);\
                result = CombineStrings(result, str_num);\
                result = CombineStrings(result, " " label "\n"); \
            }\
        } while (0)

    #define GET_FLOAT(field, label, scale)\
        do {\
            if (it.statBonuses.field != 0) {\
                if (it.statBonuses.field > 0) result = CombineStrings(result, "+");\
                sprintf(str_num, "%.0f", it.statBonuses.field * scale);\
                result = CombineStrings(result, str_num);\
                result = CombineStrings(result, " " label "\n");\
            }\
        } while (0)

    GET_INT  (health,  "Health");
    GET_INT  (stamina, "Stamina");
    GET_INT  (staminaRegen, "Stamina Regen");
    GET_INT  (speed, "Speed");
    GET_INT  (defense, "Defense");
    GET_INT  (armor, "Armour");
    GET_FLOAT(critBonus, "% Crit Bonus", 1.0f);
    GET_INT  (critRate, "Crit Rate");
    GET_INT  (mastery, "Mastery");
    GET_FLOAT(damageMultiplier, "% Damage Multiplier", 100.0f);
    GET_INT  (targetPriority, "Target Priority");

    #undef GET_INT
    #undef GET_FLOAT

    if (result[0] == '\0')
    {
        result = "This item provides no benefits.";
    }

    return result;
}

char* GetItemStatDiff(Item current, Item selected)
{
    char* result = malloc(1);
    result[0] = '\0';
    char str_num[8];
    
    char* abilityDesc = GetLingeringEffectDescription(current.triggerAbility);
    if(abilityDesc != NULL)
    {
        result = CombineStrings("LOSE: ",abilityDesc);
        result = CombineStrings(result, "\n");
    }
    abilityDesc = GetLingeringEffectDescription(selected.triggerAbility);
    if(abilityDesc != NULL)
    {
        result = CombineStrings(result, "GAIN: ");
        result = CombineStrings(result,abilityDesc);
        result = CombineStrings(result, "\n");
    }

    #define DIFF_INT(field, label)\
        do {\
            int d = selected.statBonuses.field - current.statBonuses.field;\
            if (d != 0) {\
                if (d > 0) result = CombineStrings(result, "+");\
                sprintf(str_num, "%d", d);\
                result = CombineStrings(result, str_num);\
                result = CombineStrings(result, " " label "\n"); \
            }\
        } while (0)

    #define DIFF_FLOAT(field, label, scale)\
        do {\
            float d = (selected.statBonuses.field - current.statBonuses.field) * (scale); \
            if (d != 0) {\
                if (d > 0) result = CombineStrings(result, "+");\
                sprintf(str_num, "%.0f", d);\
                result = CombineStrings(result, str_num);\
                result = CombineStrings(result, " " label "\n");\
            }\
        } while (0)

    DIFF_INT  (health,  "Health");
    DIFF_INT  (stamina, "Stamina");
    DIFF_INT  (staminaRegen, "Stamina Regen");
    DIFF_INT  (speed, "Speed");
    DIFF_INT  (defense, "Defense");
    DIFF_INT  (armor, "Armour");
    DIFF_FLOAT(critBonus, "% Crit Bonus", 1.0f);
    DIFF_INT  (critRate, "Crit Rate");
    DIFF_INT  (mastery, "Mastery");
    DIFF_FLOAT(damageMultiplier, "% Damage Multiplier", 100.0f);
    DIFF_INT  (targetPriority, "Target Priority");

    #undef DIFF_INT
    #undef DIFF_FLOAT

    if (result[0] == '\0')
    {
        result = "No difference.";
    }

    return result;
}

Item InitItem(ITEM_ID id)
{
    Item result;
    switch (id)
    {
        //
        case ITEM_NONE:
        return (Item){
            id,
            "<None>",
            TL_GENERAL,
            (Vector2){1,0},
            gray,
            ES_EVERY,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        //
        case ITEM_AMULET_OF_YENDOR:
        return (Item){
            id,
            "Amulet of Yendor",
            TL_ITEMS,
            (Vector2){11,1},
            skyblue,
            ES_EVERY,
            CreateEmptyStatBonuses(),
            LE_AMULET_OF_YENDOR,
        };
        //
        case ITEM_APPRENTICE_BOOTS:
        result = (Item){
            id,
            "Apprentice's Boots",
            TL_ITEMS,
            (Vector2){3,2},
            brown,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.mastery = 60;
        result.statBonuses.speed = 40;
        return result;
        //
        case ITEM_APPRENTICE_ROBE:
        result = (Item){
            id,
            "Apprentice's Robe",
            TL_ITEMS,
            (Vector2){0,2},
            blue,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.mastery = 40;
        result.statBonuses.stamina = 200;
        result.statBonuses.staminaRegen = 100;
        return result;
        //
        case ITEM_APPRENTICE_STAFF:
        result = (Item){
            id,
            "Apprentice's Staff",
            TL_ITEMS,
            (Vector2){2,3},
            beige,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.mastery = 80;
        result.statBonuses.targetPriority = -50;
        return result;
        //
        case ITEM_APPRENTICE_TIARA:
        result = (Item){
            id,
            "Apprentice's Tiara",
            TL_COMMUNITY,
            (Vector2){4,1},
            lightgray,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.mastery = 60;
        result.statBonuses.armor = 1;
        return result;
        //
        case ITEM_JUGGERNAUT_ARMOR:
        result = (Item){
            id,
            "Juggernaut Armour",
            TL_ITEMS,
            (Vector2){2,2},
            darkgray,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.health = 500;
        result.statBonuses.armor = 8;
        result.statBonuses.defense = 160;
        result.statBonuses.speed = -20;
        return result;
        //
        case ITEM_JUGGERNAUT_GREATHAMMER:
        result = (Item){
            id,
            "Juggernaut Hammer",
            TL_ITEMS,
            (Vector2){5,0},
            darkgray,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.critRate =15;
        result.statBonuses.critBonus = 80;
        result.statBonuses.mastery = 60;
        result.statBonuses.speed = - 20;
        return result;
        //
        case ITEM_JUGGERNAUT_HELM:
        result = (Item){
            id,
            "Juggernaut Helm",
            TL_COMMUNITY,
            (Vector2){2,3},
            darkgray,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.armor = 5;
        result.statBonuses.defense = 100;
        result.statBonuses.speed = -15;
        return result;
        //
        case ITEM_JUGGERNAUT_LEGGINS:
        result = (Item){
            id,
            "Juggernaut Leggings",
            TL_ITEMS,
            (Vector2){13,2},
            darkgray,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.health = 300;
        result.statBonuses.armor = 4;
        result.statBonuses.defense = 80;
        result.statBonuses.speed = -15;
        return result;
        //
        case ITEM_MERCENARY_AXE:
        result = (Item){
            id,
            "Mercenary's Axe",
            TL_ITEMS,
            (Vector2){6,1},
            lightgray,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.mastery = 20;
        result.statBonuses.stamina = 400;
        result.statBonuses.damageMultiplier = 0.1;
        return result;
        //
        case ITEM_MERCENARY_CUIRAS:
        result = (Item){
            id,
            "Mercenary's Cuirass",
            TL_ITEMS,
            (Vector2){2,2},
            orange,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.armor = 1;
        result.statBonuses.defense = 50;
        result.statBonuses.staminaRegen = 50;
        return result;
        //
        case ITEM_MERCENARY_GREAVES:
        result = (Item){
            id,
            "Mercenary's Greaves",
            TL_ITEMS,
            (Vector2){13,2},
            beige,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.armor = 2;
        result.statBonuses.defense = 30;
        result.statBonuses.speed = 30;
        return result;
        //
        case ITEM_MERCENARY_HELMET:
        result = (Item){
            id,
            "Mercenary's Helmet",
            TL_ITEMS,
            (Vector2){12,4},
            orange,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.armor = 1;
        result.statBonuses.defense = 30;
        result.statBonuses.staminaRegen = 50;
        return result;
        //
        case ITEM_THIEF_CAPE:
        result = (Item){
            id,
            "Thief's Cape",
            TL_ITEMS,
            (Vector2){7,4},
            gray,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.targetPriority = -200;
        result.statBonuses.defense = 40;
        result.statBonuses.speed = 20;
        return result;
        //
        case ITEM_THIEF_DAGGER:
        result = (Item){
            id,
            "Thief's Dagger",
            TL_ITEMS,
            (Vector2){0,1},
            darkgray,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.critRate = 25;
        result.statBonuses.critBonus = 40;
        result.statBonuses.speed = 50;
        return result;
        //
        case ITEM_THIEF_HOOD:
        result = (Item){
            id,
            "Thief's Hood",
            TL_COMMUNITY,
            (Vector2){3,3},
            gray,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.targetPriority  = -50;
        result.statBonuses.defense = 30;
        result.statBonuses.mastery = 30;
        return result;
        //
        case ITEM_THIEF_PANTS:
        result = (Item){
            id,
            "Thief's Pants",
            TL_ITEMS,
            (Vector2){12,2},
            darkbrown,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.speed = 50;
        result.statBonuses.staminaRegen = 100;
        return result;
        //
        case ITEM_SOFUS_HAT:
        result = (Item){
            id,
            "Hatfus",
            TL_COMMUNITY,
            (Vector2){6,0},
            white,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.mastery = 150;
        result.statBonuses.health = -150;
        result.statBonuses.targetPriority = 60;
        return result;
        //
        case ITEM_SOFUS_LEGS:
        result = (Item){
            id,
            "Sofoot",
            TL_CREATURES,
            (Vector2){14,15},
            white,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.speed = 180;
        result.statBonuses.defense = -60;
        result.statBonuses.targetPriority = 60;
        return result;
        //
        case ITEM_SOFUS_TORSO:
        result = (Item){
            id,
            "Torsofus",
            TL_COMMUNITY,
            (Vector2){7,0},
            white,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.stamina = 1000;
        result.statBonuses.armor = -10;
        result.statBonuses.targetPriority = 60;
        return result;
        //
        case ITEM_TOWER_SHIELD:
        result = (Item){
            id,
            "Tower Shield",
            TL_ITEMS,
            (Vector2){9,2},
            gray,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.defense = 100;
        result.statBonuses.armor = 10;
        result.statBonuses.targetPriority = 100;
        result.statBonuses.speed = -10;
        return result;
        //
        case ITEM_JESTER_HAT:
        result = (Item){
            id,
            "Jester's Hat",
            TL_GENERAL,
            (Vector2){3,1},
            red,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.targetPriority = 500;
        return result;
        //
        case ITEM_HALLOWED_FLESH:
        result = (Item){
            id,
            "Hallowed Flesh",
            TL_ITEMS,
            (Vector2){7,4},
            beige,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.health = 1500;
        return result;
        //
        case ITEM_TRAVELER_SHOES:
        result = (Item){
            id,
            "Traveler's Shoes",
            TL_ITEMS,
            (Vector2){3,2},
            darkblue,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.stamina = 300;
        result.statBonuses.staminaRegen = 150;
        return result;
        //
        case ITEM_BATTLE_SHIELD:
        result = (Item){
            id,
            "Combat Shield",
            TL_ITEMS,
            (Vector2){8,2},
            red,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.targetPriority = 100;
        result.statBonuses.defense = 100;
        result.statBonuses.mastery = 50;
        return result;
        //
        case ITEM_ELEMANCER_CROWN:
        result = (Item){
            id,
            "Elemancer's Crown",
            TL_COMMUNITY,
            (Vector2){4,1},
            red,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.targetPriority = 100;
        result.statBonuses.critRate = 35;
        result.statBonuses.staminaRegen = 50;
        result.statBonuses.mastery = 70;
        return result;
        //
        case ITEM_ELEMANCER_CAPE:
        result = (Item){
            id,
            "Elemancer's Cape",
            TL_COMMUNITY,
            (Vector2){5,5},
            red,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.targetPriority = 100;
        result.statBonuses.critRate = 35;
        result.statBonuses.staminaRegen = 50;
        result.statBonuses.defense = 70;
        return result;
        //
        case ITEM_ELEMANCER_BELT:
        result = (Item){
            id,
            "Elemancer Belt",
            TL_COMMUNITY,
            (Vector2){2,6},
            red,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.targetPriority = 100;
        result.statBonuses.critRate = 35;
        result.statBonuses.staminaRegen = 50;
        result.statBonuses.armor = 15;
        return result;
        //
        case ITEM_ELEMANCER_AXE:
        result = (Item){
            id,
            "Elemancer's Axe",
            TL_COMMUNITY,
            (Vector2){3,1},
            red,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.critRate = -35;
        result.statBonuses.critBonus = 200;
        result.statBonuses.defense = 30;
        result.statBonuses.speed = 30;
        return result;
        //
        case ITEM_WANDERER_HAT:
        result = (Item){
            id,
            "Wanderer's Hat",
            TL_ITEMS,
            (Vector2){4,2},
            darkgreen,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_ONABIL_WANDERER_HAT,
        };
        result.statBonuses.mastery = 60;
        result.statBonuses.critRate = 10;
        return result;
        //
        case ITEM_PENITENT_MAIL:
        result = (Item){
            id,
            "Penitent's Mail",
            TL_ITEMS,
            (Vector2){2,2},
            maroon,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_ONHIT_PENITENT_MAIL,
        };
        result.statBonuses.targetPriority = 100;
        result.statBonuses.defense = 100;
        result.statBonuses.armor = 20;
        return result;
        //
        case ITEM_COMFORTABLE_SHOES:
        result = (Item){
            id,
            "Comfortable Shoes",
            TL_ITEMS,
            (Vector2){3,2},
            darkgreen,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_ONABIL_COMFORTABLE_SHOES,
        };
        result.statBonuses.speed = 80;
        result.statBonuses.targetPriority -= 100;
        return result;
        //
        case ITEM_HAMMER_OF_GLORY:
        result = (Item){
            id,
            "Hammer of Glory",
            TL_ITEMS,
            (Vector2){5,0},
            skyblue,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_ONATT_HAMMER_OF_GLORY,
        };
        result.statBonuses.mastery = 60;
        result.statBonuses.speed = 60;
        return result;
        //
        case ITEM_CROWN_OF_FLOWERS:
        result = (Item){
            id,
            "Crown of Flowers",
            TL_ITEMS,
            (Vector2){4,3},
            lime,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.staminaRegen = 400;
        return result;
        //
        case ITEM_LIVING_CHAINMAIL:
        result = (Item){
            id,
            "Living Chainmail",
            TL_ITEMS,
            (Vector2){1,2},
            skyblue,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_ONABIL_LIVING_CHAINMAIL,
        };
        result.statBonuses.speed = 20;
        result.statBonuses.armor = 10;
        return result;
        //
        case ITEM_BALL_ON_CHAIN:
        result = (Item){
            id,
            "Ball on Chain",
            TL_ITEMS,
            (Vector2){20,3},
            blue,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_ONATT_BALL_ON_CHAIN,
        };
        result.statBonuses.speed = -10;
        result.statBonuses.critBonus = 100;
        return result;
        //
        case ITEM_MORNINGSTAR:
        result = (Item){
            id,
            "Morningstar",
            TL_ITEMS,
            (Vector2){8,1},
            darkgray,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_ONATT_MORNINGSTAR,
        };
        result.statBonuses.stamina = 200;
        result.statBonuses.damageMultiplier = 0.2;
        return result;
        //
        case ITEM_EIGHT_EYED_MASK:
        result = (Item){
            id,
            "Eight Eyed Mask",
            TL_COMMUNITY,
            (Vector2){7,6},
            purple,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.speed = 300;
        result.statBonuses.critBonus = 50;
        result.statBonuses.critRate = -10;
        result.statBonuses.mastery = -50;
        result.statBonuses.defense = -20;
        return result;
        //
        case ITEM_HOLLOW_SKULL:
        result = (Item){
            id,
            "Hollow Skull",
            TL_COMMUNITY,
            (Vector2){8,6},
            raywhite,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.mastery = -50;
        result.statBonuses.targetPriority = +50;
        result.statBonuses.staminaRegen = 70;
        result.statBonuses.defense = 70;
        result.statBonuses.health = 750;
        
        return result;
        //
        case ITEM_MALEVOLENCE:
        result = (Item){
            id,
            "Malevolence",
            TL_ITEMS,
            (Vector2){1,3},
            purple,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_ONATT_MALEVOLENCE,
        };
        result.statBonuses.mastery = 40;
        
        return result;
        //
        case ITEM_CRYSTAL_MAIL:
        result = (Item){
            id,
            "Crystal Mail",
            TL_COMMUNITY,
            (Vector2){5,2},
            raywhite,
            ES_TORSO,
            CreateEmptyStatBonuses(),
            LE_ONHIT_CRYSTAL_MAIL,
        };
        result.statBonuses.defense = 100;
        result.statBonuses.armor = 50;
        result.statBonuses.damageMultiplier = 0.1;

        return result;
        //
        case ITEM_ASSASSIN_SHOES:
        result = (Item){
            id,
            "Assassin's Shoes",
            TL_ITEMS,
            (Vector2){3,2},
            darkgray,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_ONABIL_ASSASSIN_SHOES,
        };
        result.statBonuses.targetPriority = -200;

        return result;
        //
        case ITEM_AMBROSIA:
        result = (Item){
            id,
            "Ambrosia",
            TL_ITEMS,
            (Vector2){1,4},
            orange,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_ONTURN_AMBROSIA,
        };
        result.statBonuses.stamina = 500;
        result.statBonuses.mastery = 50;
        result.statBonuses.staminaRegen = 200;
        result.statBonuses.defense = 100;
        
        return result;
        //
        case ITEM_SCRYER_BALL:
        result = (Item){
            id,
            "Scryer's Ball",
            TL_ITEMS,
            (Vector2){8,4},
            violet,
            ES_WEAPON,
            CreateEmptyStatBonuses(),
            LE_ONABIL_SCRYER_BALL,
        };
        result.statBonuses.mastery = 241;
        result.statBonuses.damageMultiplier = -1.0;
        result.statBonuses.speed = 77;
        
        return result;
        //
        case ITEM_ANTLER_HELMET:
        result = (Item){
            id,
            "Antler Helmet",
            TL_COMMUNITY,
            (Vector2){0,7},
            lightgray,
            ES_HEAD,
            CreateEmptyStatBonuses(),
            LE_ONABIL_ANTLER_HELMET,
        };
        result.statBonuses.defense = 30;
        result.statBonuses.armor = 1;
        result.statBonuses.mastery = 30;
        
        return result;
        //
        case ITEM_STONE_BOOTS:
        result = (Item){
            id,
            "Stone Boots",
            TL_COMMUNITY,
            (Vector2){1,7},
            gray,
            ES_LEGS,
            CreateEmptyStatBonuses(),
            LE_NONE,
        };
        result.statBonuses.defense = 40;
        result.statBonuses.armor = 3;
        result.statBonuses.speed = -100;
        result.statBonuses.damageMultiplier = 0.5;
        
        return result;
    }
}