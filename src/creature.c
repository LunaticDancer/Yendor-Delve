#include <stdlib.h>
#include <stdio.h>
#include "creature.h"

StatBonuses CreateEmptyStatBonuses()
{
    return (StatBonuses)
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,
    };
}

void CreateEmptyStatusEffects(int statusEffects[])
{
    for (int i = 0; i < SE_LENGTH; i++)
    {
        statusEffects[i] = 0;
    }
}

void ResetTurnClock(CreatureStats* _creature, short itemSpeedBonus)
{
    float value = 100.0 / (100 + (*_creature).baseStats.speed + (*_creature).encounterStats.speed + itemSpeedBonus);
    (*_creature).baseStats.ticksUntilNextTurn = (short)(value*1000);
}


short CalculateDamage(short baseDamage, CreatureStats* target)
{
    short result = 0;

    short effectiveDef = (*target).baseStats.defense + (*target).encounterStats.defense + (*target).itemStats.defense - (short)((float)(*target).statusEffects[SE_BERSERK]/100);
    short effectiveArmor = (*target).baseStats.armor + (*target).encounterStats.armor + (*target).itemStats.armor;

   if(effectiveDef>0)
   {
        result = (short)((float)baseDamage * (100.0/(100 + effectiveDef)));
   }
   else
   {
        result = (short)((float)baseDamage * ((100 - effectiveDef)/100.0));
   }
   result -= effectiveArmor;

    return result;
}


void DealDamage(short damage, CreatureStats* target, bool trueDamage)
{
    if(trueDamage)
    {
        (*target).baseStats.currentHealth -= damage;
    }
    else
    {
        (*target).baseStats.currentHealth -= CalculateDamage(damage, target);
    }

    if((*target).baseStats.currentHealth <= 0)
    {
        (*target).baseStats.currentHealth = 0;
        char* message = CombineStrings((*target).baseStats.name, " was slain!");
        AddMessageToFeed(message);
    }
}

char* GetAbilityDescription(ABILITY id, CreatureStats* caster)
{
    switch(id)
    {
        case AB_WAIT:
        return "Inaction. Let the opportunity pass.";
        break;
    }
}

Ability* InitAbilities(ABILITY abilities[])
{
    char count = sizeof(abilities) /  sizeof(ABILITY);
    Ability* result = malloc(count * sizeof(Ability));

    for(int i = 0; i<count;i++)
    {
        result[i] = InitAbility(abilities[i]);
    }

    return result;
}

void CastAbility(ABILITY id, CreatureStats* caster, CreatureStats* targets, short numberOfTargets)
{
    switch(id)
    {
        case AB_WAIT:
        break;
    }
}