#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "creature.h"
#include "state.h"

extern struct AppState appState;

StatBonuses CreateEmptyStatBonuses()
{
    return (StatBonuses)
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,
    };
}

void EmptyStatusEffects(CreatureStats* _creature)
{
    for (int i = 0; i < SE_LENGTH; i++)
    {
        _creature->statusEffects[i] = 0;
    }
}

void ResetTurnClock(CreatureStats* _creature)
{
    float value = 100.0 / (100 + (*_creature).baseStats.speed + (*_creature).encounterStats.speed + (*_creature).itemStats.speed);
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
        (*target).encounterStats.shield -= damage;
        if((*target).encounterStats.shield < 0)
        {
            (*target).baseStats.currentHealth += (*target).encounterStats.shield;
            (*target).encounterStats.shield = 0;
        }
    }
    else
    {
        (*target).encounterStats.shield -= CalculateDamage(damage, target);
        if((*target).encounterStats.shield < 0)
        {
            (*target).baseStats.currentHealth += (*target).encounterStats.shield;
            (*target).encounterStats.shield = 0;
        }
    }

    if((*target).baseStats.currentHealth <= 0)
    {
        (*target).baseStats.currentHealth = 0;
        char* message = CombineStrings((*target).baseStats.name, " was slain!");
        AddMessageToFeed(message);
    }
}

float CalculateCritInfluence(CreatureStats* caster)
{
    return 1 +  (caster->baseStats.critCounter / CRIT_PROGRESS_MAX)
            *(((*caster).baseStats.critMultiplier + (*caster).itemStats.critMultiplier + (*caster).encounterStats.critMultiplier)*0.01);
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
        case AB_BERSERKER_SWING:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateCritInfluence(caster));
        result = CombineStrings("Bring the battle axe down in a wild swing, gaining ", strnum);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, " (10 + 10% Mastery) Berserk and dealing ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (50 + 80% Mastery) damage.");
        return result;
        case AB_BERSERKER_BASH:
        sprintf(strnum, "%.0f", (((caster->baseStats.armor + caster->encounterStats.armor + caster->itemStats.armor) * (caster->statusEffects[SE_BERSERK] + 1))) * CalculateCritInfluence(caster));
        result = CombineStrings("Stun an enemy with a powerful shield strike, delaying their next turn by ", strnum);
        result = CombineStrings(result, " (Armour x Berserk) ticks of time.");
        return result;
        case AB_BERSERKER_BATTLECRY:
        sprintf(strnum, "%.0f", (((caster->baseStats.currentStamina) * 0.25)) * CalculateCritInfluence(caster));
        result = CombineStrings("Perform a mighty cry, expending half of your current Stamina, gaining ", strnum);
        result = CombineStrings(result, " (50% of expended Stamina) Berserk and Target Priority.");
        return result;
        case AB_BERSERKER_BRACE:
        sprintf(strnum, "%.0f", ((1 + (caster->statusEffects[SE_BERSERK]) * 0.05)) * CalculateCritInfluence(caster));
        result = CombineStrings("Raise the shield in a defensive stance, gaining ", strnum);
        result = CombineStrings(result, " (1 + 5% Berserk) Armour and ");
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20 + 40% Mastery) Defense until next turn.");
        return result;
        case AB_ASSASSIN_SLASH:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateCritInfluence(caster));
        result = CombineStrings("Wound the enemy, dealing ", strnum);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, " (20 + 20% Mastery) damage and applying ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 10% Mastery) Bleed.");
        return result;
        case AB_ASSASSIN_PREPARE:
        sprintf(strnum, "%.0f", (((caster->baseStats.critRate + caster->encounterStats.critRate + caster->itemStats.critRate) 
            * (1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.01))));
        result = CombineStrings("Prepare for the next action, gaining ", strnum);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)));
        result = CombineStrings(result, " (improved by Crit Rate and Mastery) Crit Progress and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 30% Mastery) Crit Multiplier. This ability cannot crit.");
        return result;
        case AB_ASSASSIN_CONCEAL:
        sprintf(strnum, "%.0f", ((500 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1)) * CalculateCritInfluence(caster));
        result = CombineStrings("Target ally becomes untargettable for ", strnum);
        result = CombineStrings(result, " (500 + 100% Mastery) ticks of time.");
        return result;
        case AB_ASSASSIN_REND:
        result = "Performs a brutal finisher, dealing four times the amount of Bleed points the target enemy has as unavoidable damage.";
        return result;
        case AB_DUELIST_LUNGE:
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateCritInfluence(caster));
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, " (30 + 50% Mastery) damage and gain ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 20% Mastery) Speed.");
        return result;
        case AB_DUELIST_OPPORTUNITY:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateCritInfluence(caster));
        result = CombineStrings("Amplify the effectiveness of abilities by ", strnum);
        result = CombineStrings(result, "% (100 + 80% Mastery) a select number of turns from now (can also benefit enemies).");
        return result;
        case AB_DUELIST_PARRY:
        sprintf(strnum, "%.0f", ((5 + (caster->baseStats.speed + caster->encounterStats.speed + caster->itemStats.speed) * 0.05)) * CalculateCritInfluence(caster));
        result = CombineStrings("Gain ", strnum);
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, " (5 + 5% Speed) Armour until next turn. Each time you get hit within that time, gain ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20 + 50% Mastery) Speed.");
        return result;
        case AB_DUELIST_BREATH:
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateCritInfluence(caster));
        result = CombineStrings("Regain ", strnum);
        result = CombineStrings(result, " (50 + 100% Mastery) stamina.");
        return result;
        case AB_MONK_MEDITATE:
        sprintf(strnum, "%.0f", (10 + ((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateCritInfluence(caster));
        result = CombineStrings("Gain ", strnum);
        result = CombineStrings(result,  " (10 + 10% Mastery) mastery.");
        return result;
        case AB_MONK_TRUE_STRIKE:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateCritInfluence(caster));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ?
            " (100 + 80% Mastery) unavoidable damage to all enemies." : " (100 + 80% Mastery) unavoidable damage to target enemy. Becomes an area ability upon crit.");
        return result;
        case AB_MONK_ATTUNEMENT:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.9)) * CalculateCritInfluence(caster));
        result = CombineStrings((caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ?  "Shield all allies for " : " Shield a target ally for ", strnum);
        result = CombineStrings(result, " (10 + 90% Mastery) health points.");
        result = CombineStrings(result, (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ?  " " : " Becomes an area ability upon crit. ");
        return result;
        case AB_MONK_CLEANSE:
        result = (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ?
            "Cleanse all status effects from all creatures and entities." : "Cleanse all status effects from target creature. Becomes an area ability upon crit.";
        return result;
        case AB_FOLEM_STRIKE:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1 
            + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)) * CalculateCritInfluence(caster));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (10 + 10% Health + 30% Mastery) damage.");
        return result;
        case AB_FOLEM_EXPUNGE:
        sprintf(strnum, "%.0f", (( (caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.5)) * CalculateCritInfluence(caster));
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", (( (caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, " (50% Health) damage to an enemy and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10% Health) damage to self.");
        return result;
        case AB_FOLEM_EPIDERMIZE:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.9)) * CalculateCritInfluence(caster));
        result = CombineStrings("Gain ", strnum);
        sprintf(strnum, "%.0f", (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, " (10 + 90% Mastery) Defense and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10% Health) Shield points.");
        return result;
        case AB_FOLEM_CRIPPLE:
        sprintf(strnum, "%.0f", (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health - caster->baseStats.currentHealth) * 0.1)) * CalculateCritInfluence(caster));
        result = CombineStrings("Apply ", strnum);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateCritInfluence(caster));
        result = CombineStrings(result, " (10% missing Health) Bleed to an enemy.");
        return result;
        case AB_SHAPESHIFTER_SCRATCH:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateCritInfluence(caster));
        result = CombineStrings("Scratch an enemy for ", strnum);
        result = CombineStrings(result, " (10 + 100% Mastery) damage.");
        return result;
        case AB_SHAPESHIFTER_TRANSFORM:
        return "Become an exact copy of target enemy, retaining your ability to change shapes.";
        case AB_BLOFAEWAR_CUT:
        sprintf(strnum, "%.0f", ((1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateCritInfluence(caster));
        result = CombineStrings("Attack an enemy for ", strnum);
        result = CombineStrings(result, " (1 + 10% Mastery) damage, then apply the unmitigated damage as Bleed points.");
        return result;
        case AB_BLOFAEMYS_INSPIRE:
        return "Give 10 Mastery to every ally.";
        default:
        return "Ability description missing, oopsie!";
    }
}

void CastAbility(ABILITY id, short cost, CreatureStats* caster, CreatureStats** targets, short numberOfTargets)
{
    char* message;
    char strnum[6];
    short primaryEffectValue;
    bool isCrit = (*caster).baseStats.critCounter >= CRIT_PROGRESS_MAX;
    bool dontResetCritProgress = false;

    caster->baseStats.currentStamina -= cost;

    switch(id)
    {
        case AB_WAIT:
        AddMessageToFeed(CombineStrings(caster->baseStats.name, " does nothing."));
        break;
        case AB_MONK_MEDITATE:
        primaryEffectValue = (10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1) * CalculateCritInfluence(caster);
        (*caster).encounterStats.mastery += primaryEffectValue;
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " meditates, gaining ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery));
        message = CombineStrings(message, " Mastery, for a total of ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, ".");
        AddMessageToFeed(message);
        break;
        case AB_MONK_TRUE_STRIKE:
        primaryEffectValue = (100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) * CalculateCritInfluence(caster);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " uses True Strike, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " unavoidable damage to ");
        for(int i = 0; i < numberOfTargets; i++)
        {
            if(i == numberOfTargets - 1)
            {
                if(i != 0)
                {
                    message = CombineStrings(message, " and ");
                }
                message = CombineStrings(message, targets[i]->baseStats.name);
                message = CombineStrings(message, ".");
            }
            else
            {
                if(i != 0)
                {
                    message = CombineStrings(message, ", ");
                }
                message = CombineStrings(message, targets[i]->baseStats.name);
            }
        }
        AddMessageToFeed(message);
        for(int i = 0; i < numberOfTargets; i++)
        {
            DealDamage(primaryEffectValue, targets[i], true);
        }
        break;
        case AB_MONK_ATTUNEMENT:
        primaryEffectValue = (10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.9) * CalculateCritInfluence(caster);
        for(int i = 0; i < numberOfTargets; i++)
        {
            targets[i]->encounterStats.shield += primaryEffectValue;
        }
        sprintf(strnum, "%d", primaryEffectValue);
        if(isCrit)
        {
            message = CombineStrings((*caster).baseStats.name, " shields their team for ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " hit points.");
        }
        else
        {
            message = CombineStrings((*caster).baseStats.name, " shields ");
            message = CombineStrings(message, targets[0]->baseStats.name);
            message = CombineStrings(message, " for ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " hit points.");
        }
        AddMessageToFeed(message);
        break;
        case AB_MONK_CLEANSE:
        for(int i = 0; i < numberOfTargets; i++)
        {
            EmptyStatusEffects(targets[i]);
        }
        if(isCrit)
        {
            message = CombineStrings((*caster).baseStats.name, " cleanses all ailments from every creature on the battlefield.");
        }
        else
        {
            message = CombineStrings((*caster).baseStats.name, " cleanses all ailments from ");
            message = CombineStrings(message, targets[0]->baseStats.name);
            message = CombineStrings(message, ".");
        }
        AddMessageToFeed(message);
        break;
        case AB_FOLEM_STRIKE:
        if(appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 0) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 1;
        }
        primaryEffectValue = ((10 + (caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1 
            + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)) * CalculateCritInfluence(caster);
        sprintf(strnum, "%d", CalculateDamage( primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " slams ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with a mass of chaotic flesh, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        DealDamage(primaryEffectValue, targets[0], false);
        break;
        case AB_FOLEM_EXPUNGE:
        if(appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 1) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 2;
        }
        primaryEffectValue = (( (caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.5)) * CalculateCritInfluence(caster);
        sprintf(strnum, "%d", CalculateDamage( primaryEffectValue, targets[0]));
        short folemExpungeValue = (( (caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1)) * CalculateCritInfluence(caster);
        message = CombineStrings((*caster).baseStats.name, " explodes violently with viscera, dealing ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", CalculateDamage( folemExpungeValue, caster));
        message = CombineStrings(message, " damage to ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage to self.");
        AddMessageToFeed(message);
        DealDamage(primaryEffectValue, targets[0], false);
        DealDamage(folemExpungeValue, caster, false);
        break;
        case AB_FOLEM_EPIDERMIZE:
        if(appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 2) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 4;
        }
        primaryEffectValue = ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.9)) * CalculateCritInfluence(caster);
        sprintf(strnum, "%d", primaryEffectValue);
        short folemEpidermizeShield = (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1)) * CalculateCritInfluence(caster);
        message = CombineStrings((*caster).baseStats.name, " rapidly hardens its epidermis into a carapace, gaining ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", folemEpidermizeShield);
        message = CombineStrings(message, " Defense and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Shield points.");
        AddMessageToFeed(message);
        caster->encounterStats.defense += primaryEffectValue;
        caster->encounterStats.shield += folemEpidermizeShield;
        break;
        case AB_FOLEM_CRIPPLE:
        if(appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 3) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 8;
        }
        primaryEffectValue = (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health - caster->baseStats.currentHealth) * 0.1)) * CalculateCritInfluence(caster);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " mauls ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with its exposed endoskeleton, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Bleed.");
        AddMessageToFeed(message);
        targets[0]->statusEffects[SE_BLEED] += primaryEffectValue;
        break;
        case AB_SHAPESHIFTER_SCRATCH:
        primaryEffectValue = (10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateCritInfluence(caster);
        sprintf(strnum, "%d", CalculateDamage( primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " scratches ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        DealDamage(primaryEffectValue, targets[0], false);
        break;
        case AB_SHAPESHIFTER_TRANSFORM:
        message = CombineStrings((*caster).baseStats.name, " becomes ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ".");
        AddMessageToFeed(message);
        char abCount = targets[0]->abilityCount + 1;
        ABILITY* abilities = malloc(abCount * sizeof(int));
        for(int i= 0; i<targets[0]->abilityCount-1; i++)
        {
            abilities[i] = targets[0]->abilities[i].abilityId;
        }
        abilities[abCount-2] = AB_SHAPESHIFTER_TRANSFORM;
        abilities[abCount-1] = AB_WAIT;
        caster->baseStats = targets[0]->baseStats;
        caster->abilities = InitAbilities(abilities, abCount);
        caster->abilityCount = abCount;
        break;
        case AB_BLOFAEWAR_CUT:
        primaryEffectValue = CalculateDamage( ((1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateCritInfluence(caster), targets[0]);
        primaryEffectValue = primaryEffectValue < 0 ? 0 : primaryEffectValue;
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " cuts ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Bleed.");
        AddMessageToFeed(message);
        targets[0]->statusEffects[SE_BLEED] += primaryEffectValue;
        break;
        case AB_BLOFAEMYS_INSPIRE:
        for(int i = 0; i < numberOfTargets; i++)
        {
            targets[i]->encounterStats.mastery += 10;
        }
        message = CombineStrings((*caster).baseStats.name, " sings an ancient fae hymn, increasing Mastery by 10 for each team member.");
        AddMessageToFeed(message);
        break;
        default:
        message = CombineStrings((*caster).baseStats.name, " uses an ability that wasn't implemented yet, how embarassing!");
        AddMessageToFeed(message);
        break;
    }
    if(!dontResetCritProgress)
    {
        (*caster).baseStats.critCounter = (*caster).baseStats.critCounter % CRIT_PROGRESS_MAX;
    }
    (*caster).baseStats.critCounter += (*caster).baseStats.critRate + (*caster).itemStats.critRate + (*caster).encounterStats.critRate;

    // Monk's quirk
    if(strcmp(caster->baseStats.name, "Monk") == 0)
    {
        if((*caster).baseStats.critCounter >= CRIT_PROGRESS_MAX)
        {
            if(!DoesAbilityHaveFlag(caster->abilities[1], AF_AOE))
            {
                caster->abilities[1].abilityFlags += AF_AOE;
                caster->abilities[2].abilityFlags += AF_AOE;
                caster->abilities[3].abilityFlags += AF_AOE;
            }
        }
        else
        {
            if(DoesAbilityHaveFlag(caster->abilities[1], AF_AOE))
            {
                caster->abilities[1].abilityFlags -= AF_AOE;
                caster->abilities[2].abilityFlags -= AF_AOE;
                caster->abilities[3].abilityFlags -= AF_AOE;
            }
        }
    }
}