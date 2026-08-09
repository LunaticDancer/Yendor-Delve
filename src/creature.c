#include <stdlib.h>
#include <stdio.h>
#include "creature.h"
#include "state.h"

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

Ability* InitAbilities(ABILITY abilities[], short count)
{
    Ability* result = malloc(count * sizeof(Ability));

    for(int i = 0; i<count;i++)
    {
        result[i] = InitAbility(abilities[i]);
    }

    return result;
}

char* GetAbilityDescription(ABILITY id, CreatureStats* caster)
{
    char strnum[6];
    char* result;
    switch(id)
    {
        case AB_WAIT:
        return "Inaction. Let the opportunity pass.";
        break;
        case AB_MONK_MEDITATE:
        sprintf(strnum, "%.0f", ((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1) * 
            1 +  caster->baseStats.critCounter / CRIT_PROGRESS_MAX);
        result = CombineStrings("Gain 10 + ", strnum);
        result = CombineStrings(result,  " (10% Mastery) mastery.");
        return result;
        break;
        case AB_MONK_TRUE_STRIKE:
        sprintf(strnum, "%.0f", ((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) * 
            1 +  caster->baseStats.critCounter / CRIT_PROGRESS_MAX);
        result = CombineStrings("Deal 100 + ", strnum);
        result = CombineStrings(result, (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ?
            " (80% Mastery) unavoidable damage to all enemies." : " (80% Mastery) unavoidable damage to target enemy.");
        return result;
        case AB_MONK_ATTUNEMENT:
        sprintf(strnum, "%.0f", ((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.9) * 
            1 +  caster->baseStats.critCounter / CRIT_PROGRESS_MAX);
        result = CombineStrings((caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ?  "Shield all allies for 10 + " : " Shield a target ally for 10 + ", strnum);
        result = CombineStrings(result, " (90% Mastery) health points.");
        return result;
        case AB_MONK_CLEANSE:
        result = (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ?
            "Cleanse all status effects from target team." : "Cleanse all status effects from target creature.";
        return result;
    }
}

void CastAbility(ABILITY id, CreatureStats* caster, CreatureStats* targets, short numberOfTargets)
{
    switch(id)
    {
        case AB_WAIT:
        AddMessageToFeed(CombineStrings(caster->baseStats.name, " does nothing."));
        break;
    }
}