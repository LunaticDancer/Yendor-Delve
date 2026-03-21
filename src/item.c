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

char* CombineStrings(char* str1, char* str2)
{
    char* new_str;
    if((new_str = malloc(strlen(str1)+strlen(str2)+1)) != NULL){
        new_str[0] = '\0';   // ensures the memory is an empty string
        strcat(new_str,str1);
        strcat(new_str,str2);
    } else {
        fprintf(stderr,"malloc failed!\n");
        // exit?
    }
}

Item InitItem(ITEM_ID id)
{
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
        Item result = (Item){
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
    }
}