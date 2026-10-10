#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "creature.h"
#include "state.h"

extern struct AppState appState;
extern ITEM_ID itemPoolTier1[];

StatBonuses CreateEmptyStatBonuses()
{
    return (StatBonuses){
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
}

void EmptyStatDebuffs(CreatureStats *_creature)
{
    for (int i = 0; i < STAT_DEBUFFS; i++)
    {
        _creature->temporaryStats[i] = (StatDebuff){
            0,
            CreateEmptyStatBonuses(),
        };
    }
}

void ApplyStatDebuff(CreatureStats *c, StatDebuff d)
{
    char index = -1;
    short lowestDuration = d.tickDuration;
    for (int i = 0; i < STAT_DEBUFFS; i++)
    {
        if (c->temporaryStats[i].tickDuration > lowestDuration)
            continue;
        index = i;
        lowestDuration = c->temporaryStats[i].tickDuration;
    }
    if (index == -1)
        return;
    c->temporaryStats[index] = d;
    c->encounterStats.armor += d.debuff.armor;
    c->encounterStats.critBonus += d.debuff.critBonus;
    c->encounterStats.critRate += d.debuff.critRate;
    c->encounterStats.damageMultiplier += d.debuff.damageMultiplier;
    c->encounterStats.defense += d.debuff.defense;
    c->encounterStats.health += d.debuff.health;
    c->encounterStats.mastery += d.debuff.mastery;
    c->encounterStats.shield += d.debuff.shield;
    c->encounterStats.speed += d.debuff.speed;
    c->encounterStats.stamina += d.debuff.stamina;
    c->encounterStats.staminaRegen += d.debuff.staminaRegen;
    c->encounterStats.targetPriority += d.debuff.targetPriority;
}

void EmptyLingeringEffects(CreatureStats *c)
{
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        c->lingeringEffects[i].effectId = LE_NONE;
        c->lingeringEffects[i].triggerLimit = 0;
        c->lingeringEffects[i].tickDuration = 0;
    }
}

void ApplyLingeringEffect(CreatureStats *c, LingeringEffect l)
{
    short shortestTicks = 9999;
    char shortestIndex = 0;
    char matchingIndex = -1;

    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (c->lingeringEffects[i].effectId == l.effectId)
        {
            matchingIndex = i;
        }
        if (c->lingeringEffects[i].tickDuration < shortestTicks)
        {
            shortestTicks = c->lingeringEffects[i].tickDuration;
            shortestIndex = i;
        }
    }

    if (matchingIndex != -1)
    {
        c->lingeringEffects[matchingIndex] = l;
    }
    else
    {
        c->lingeringEffects[shortestIndex] = l;
    }
}

void ProgressLingeringEffects(CreatureStats *c, short t)
{
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (c->lingeringEffects[i].effectId == LE_NONE)
            continue;

        c->lingeringEffects[i].tickDuration -= t;
        if (c->lingeringEffects[i].tickDuration < 1 && c->lingeringEffects[i].triggerLimit < 1)
        {
            c->lingeringEffects[i].effectId = LE_NONE;
        }
    }
}

void HandleOnHitEffects(CreatureStats *c, short damage, CreatureStats *caster)
{
    char strnum[6];
    char *message;
    short primaryValue;
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (c->lingeringEffects[i].effectId == LE_NONE)
            continue;
        switch (c->lingeringEffects[i].effectId)
        {
        case LE_ONHIT_DUELIST_PARRY:
            primaryValue = ((40 + (c->baseStats.mastery + c->encounterStats.mastery + c->itemStats.mastery) * 0.5)) * CalculateEffectAmplification(c, true);
            sprintf(strnum, "%d", primaryValue);
            message = CombineStrings((*c).baseStats.name, " parries the attack, gaining ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Speed.");
            AddMessageToFeed(message);
            c->encounterStats.speed += primaryValue;
            break;
        case LE_ONHIT_WOLF_BITE:
            primaryValue = c->lingeringEffects[i].storedValue;
            sprintf(strnum, "%d", primaryValue);
            message = CombineStrings((*c).baseStats.name, "'s bite mark itches, causing ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Berserk.");
            AddMessageToFeed(message);
            c->statusEffects[SE_BERSERK] += primaryValue;
            break;
        }
    }
}

void HandleOnDeathEffects(CreatureStats *c)
{
    // Ripper reset
    for(int i = 0; i<3;i++)
    {
        if(appState.stateData.gameState.playerTeam[i].characterId != CHAR_RIPPER) continue;
        appState.stateData.gameState.playerTeam[i].stats.baseStats.ticksUntilNextTurn = 0;
        appState.stateData.gameState.playerTeam[i].stats.baseStats.critCounter += 100;
        AddMessageToFeed("The Ripper gets excited and acts out of turn.");
    }
}

void HandleOnAbilityEffects(CreatureStats *c, ABILITY a)
{
}

void EmptyStatusEffects(CreatureStats *_creature)
{
    for (int i = 0; i < SE_LENGTH; i++)
    {
        _creature->statusEffects[i] = 0;
    }
}

void ResetTurnClock(CreatureStats *_creature)
{
    (*_creature).baseStats.ticksUntilNextTurn = CalculateNextTurnTicks(_creature);
}

short CalculateNextTurnTicks(CreatureStats *_creature)
{
    short speed = (*_creature).baseStats.speed + (*_creature).encounterStats.speed + (*_creature).itemStats.speed - (*_creature).statusEffects[SE_EXHAUSTION];
    if (speed > 0)
    {
        float value = 100.0 / (100 + speed);
        return (short)(value * 1000);
    }
    else
    {
        return 1000 - speed;
    }
}

short CalculateDamage(short baseDamage, CreatureStats *target)
{
    short result = 0;

    short effectiveDef = (*target).baseStats.defense + (*target).encounterStats.defense + (*target).itemStats.defense - (*target).statusEffects[SE_BERSERK];
    short effectiveArmor = (*target).baseStats.armor + (*target).encounterStats.armor + (*target).itemStats.armor;

    if (effectiveDef > 0)
    {
        result = (short)((float)baseDamage * (100.0 / (100 + effectiveDef)));
    }
    else
    {
        result = (short)((float)baseDamage * ((100 - effectiveDef) / 100.0));
    }
    result -= effectiveArmor;

    return result;
}

void DealDamage(short damage, CreatureStats *target, bool trueDamage, CreatureStats *dealer)
{
    bool wasAlreadyDead = (*target).baseStats.currentHealth <= 0;
    short finalValue = 0;
    finalValue = (trueDamage ? damage : CalculateDamage(damage, target));
    (*target).encounterStats.shield -= finalValue;
    if ((*target).encounterStats.shield < 0)
    {
        if(strcmp((*target).baseStats.name, "Magus") == 0)
        {
            (*target).baseStats.currentStamina += (*target).encounterStats.shield;
            if ((*target).baseStats.currentStamina < 0) 
            {
                (*target).baseStats.currentHealth += (*target).baseStats.currentStamina;
                (*target).baseStats.currentStamina = 0;
            }
        }
        else
        {
            (*target).baseStats.currentHealth += (*target).encounterStats.shield;
        }
        (*target).encounterStats.shield = 0;
    }
    HandleOnHitEffects(target, finalValue, dealer);

    if ((*target).baseStats.currentHealth <= 0)
    {
        if(strcmp(target->baseStats.name, "Empty Space") == 0)
        {
            AddMessageToFeed("The empty space has been truly killed dead.");
        }
        else if(wasAlreadyDead)
        {
            char *message = CombineStrings((*target).baseStats.name, " isn't getting any deader.");
            AddMessageToFeed(message);
        }
        else
        {
            (*target).baseStats.currentHealth = 0;
            char *message = CombineStrings((*target).baseStats.name, " was slain!");
            AddMessageToFeed(message);
            HandleOnDeathEffects(target);
        }
    }
}

void HandlePain(CreatureStats* c)
{
    if(c->statusEffects[SE_PAIN] <= 0) return;
    if(c->baseStats.currentHealth <= 0) return;

    char* message = CombineStrings(c->baseStats.name, " winces in pain, receiving ");
    char strnum[6];
    sprintf(strnum, "%d", CalculateDamage(c->statusEffects[SE_PAIN], c));
    message = CombineStrings(message, strnum);
    message = CombineStrings(message, " damage from their wounds.");
    AddMessageToFeed(message);
    DealDamage(c->statusEffects[SE_PAIN], c, false, NULL);
}

float CalculateEffectAmplification(CreatureStats *caster, bool affectedByBerserk)
{
    return 1 + ((caster->baseStats.critCounter / CRIT_PROGRESS_MAX) * (((*caster).baseStats.critBonus + (*caster).itemStats.critBonus + (*caster).encounterStats.critBonus) * 0.01)) + (affectedByBerserk ? caster->statusEffects[SE_BERSERK] * 0.01 : 0) + ((appState.stateData.gameState.stateData.battleState.opportunitySkillCountdown == 0) ? appState.stateData.gameState.stateData.battleState.opportunityMult : 0);
}

Ability *InitAbilities(ABILITY abilities[], short count)
{
    Ability *result = malloc(count * sizeof(Ability));

    for (int i = 0; i < count; i++)
    {
        result[i] = InitAbility(abilities[i]);
    }

    return result;
}

char *GetAbilityDescription(ABILITY id, CreatureStats *caster)
{
    char strnum[6];
    char *result;
    switch (id)
    {
    case AB_WAIT:
        return "Inaction. Let the opportunity pass.";
    case AB_BERSERKER_SWING:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Bring the battle axe down in a wild swing, gaining ", strnum);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (10 + 10% Mastery) Berserk, removing ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 40% Mastery) Defense from the target and dealing ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 40% Mastery) damage.");
        return result;
    case AB_BERSERKER_BASH:
        sprintf(strnum, "%.0f", (((caster->baseStats.armor + caster->encounterStats.armor + caster->itemStats.armor) * (caster->statusEffects[SE_BERSERK] + 1))) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Stun an enemy with a powerful shield strike, delaying their next turn by ", strnum);
        result = CombineStrings(result, " (Armour x Berserk) ticks of time.");
        return result;
    case AB_BERSERKER_BATTLECRY:
        sprintf(strnum, "%.0f", (((caster->baseStats.currentStamina) * 0.2)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Perform a mighty cry, expending half of your current Stamina, gaining ", strnum);
        result = CombineStrings(result, " (40% of expended Stamina) Berserk and Target Priority. Take an additional turn immediately after.");
        return result;
    case AB_BERSERKER_BRACE:
        sprintf(strnum, "%.0f", ((1 + (caster->statusEffects[SE_BERSERK]) * 0.05)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Raise the shield in a defensive stance, gaining ", strnum);
        result = CombineStrings(result, " (1 + 5% Berserk) Armour and ");
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20 + 40% Mastery) Defense until next turn. Take an additional turn immediately after.");
        return result;
    case AB_ASSASSIN_SLASH:
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Wound the enemy, dealing ", strnum);
        sprintf(strnum, "%.0f", ((15 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (20 + 20% Mastery) damage and applying ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 10% Mastery) Bleed.");
        return result;
    case AB_ASSASSIN_PREPARE:
        sprintf(strnum, "%.0f", (((caster->baseStats.critRate + caster->encounterStats.critRate + caster->itemStats.critRate) * (1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.01))));
        result = CombineStrings("Prepare for the next action, gaining ", strnum);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)));
        result = CombineStrings(result, " (improved by Crit Rate and Mastery) Crit Progress and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, "% (10 + 30% Mastery) Crit Bonus. This ability cannot crit.");
        return result;
    case AB_ASSASSIN_CONCEAL:
        sprintf(strnum, "%.0f", ((300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Target ally becomes untargettable for ", strnum);
        result = CombineStrings(result, " (300 + 100% Mastery) ticks of time.");
        return result;
    case AB_ASSASSIN_REND:
        result = "Performs a brutal finisher, dealing four times the amount of Bleed points the target enemy has as unavoidable damage.";
        return result;
    case AB_DUELIST_LUNGE:
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (30 + 50% Mastery) damage and gain ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20 + 20% Mastery) Speed.");
        return result;
    case AB_DUELIST_OPPORTUNITY:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8
            + (caster->baseStats.speed + caster->encounterStats.speed + caster->itemStats.speed) * 0.1)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Amplify the effectiveness of abilities by ", strnum);
        result = CombineStrings(result, "% (100 + 80% Mastery + 10% Speed) a select number of turns from now (can also benefit enemies).");
        return result;
    case AB_DUELIST_PARRY:
        sprintf(strnum, "%.0f", ((5 + (caster->baseStats.speed + caster->encounterStats.speed + caster->itemStats.speed) * 0.05)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Gain ", strnum);
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (5 + 5% Speed) Armour until next turn. Each time you get hit within that time, gain ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (40 + 50% Mastery) Speed.");
        return result;
    case AB_DUELIST_BREATH:
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Regain ", strnum);
        result = CombineStrings(result, " (50 + 100% Mastery) stamina and gain double the crit progress.");
        return result;
    case AB_MONK_MEDITATE:
        sprintf(strnum, "%.0f", (10 + ((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Gain ", strnum);
        result = CombineStrings(result, " (10 + 40% Mastery) Mastery and double the Crit Progress this turn.");
        return result;
    case AB_MONK_TRUE_STRIKE:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ? " (100 + 200% Mastery) unavoidable damage to all enemies." : " (100 + 200% Mastery) unavoidable damage to target enemy. Becomes an area ability upon crit.");
        return result;
    case AB_MONK_ATTUNEMENT:
        sprintf(strnum, "%.0f", ((150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings((caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ? "Shield all allies for " : " Shield a target ally for ", strnum);
        result = CombineStrings(result, " (150 + 200% Mastery) health points.");
        result = CombineStrings(result, (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ? " " : " Becomes an area ability upon crit. ");
        return result;
    case AB_MONK_CLEANSE:
        result = (caster->baseStats.critCounter >= CRIT_PROGRESS_MAX) ? "Cleanse all status effects from all creatures and entities." : "Cleanse all status effects from target creature. Becomes an area ability upon crit.";
        return result;
    case AB_FOLEM_STRIKE:
        sprintf(strnum, "%.0f", (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1 + 
        (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (10% Health + 30% Mastery) damage.");
        return result;
    case AB_FOLEM_EXPUNGE:
        sprintf(strnum, "%.0f", (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (50% Health) damage to an enemy and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20% Health) damage to self.");
        return result;
    case AB_FOLEM_EPIDERMIZE:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.9)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Gain ", strnum);
        sprintf(strnum, "%.0f", (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1) +
            ((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health - caster->baseStats.currentHealth) * 0.1) + 
        (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (10 + 90% Mastery) Defense and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (100% Mastery + 10% Health + 10% missing Health) Shield points.");
        return result;
    case AB_FOLEM_CRIPPLE:
        sprintf(strnum, "%.0f", (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health - caster->baseStats.currentHealth) * 0.1)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (10% missing Health) Bleed to an enemy.");
        return result;
    case AB_FOLEM_MEND:
        sprintf(strnum, "%.0f", (((caster->baseStats.currentHealth) * 0.3)));
        result = CombineStrings("Sacrifice ", strnum);
        result = CombineStrings(result, " (30% current Health) Health. Heal an ally by 60% of the Health lost and cleanse their status effects.");
        return result;
    case AB_SHAPESHIFTER_SCRATCH:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Scratch an enemy for ", strnum);
        result = CombineStrings(result, " (10 + 100% Mastery) damage.");
        return result;
    case AB_SHAPESHIFTER_TRANSFORM:
        return "Become an exact copy of target enemy, retaining your ability to change shapes.";
    case AB_MAGUS_DISINTEGRATE:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.2) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_DISINTEGRATE_COST) * 0.3) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (100 + 120% Mastery + 30% missing Stamina) unavoidable damage to an enemy.");
        return result;
    case AB_MAGUS_ARCANE_BLAST:
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_ARCANE_BLAST_COST) * 0.5) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 80% Mastery + 50% missing Stamina) damage to all enemies.");
        return result;
    case AB_MAGUS_TUTOR:
        sprintf(strnum, "%.0f", (40+(caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Give an ally ", strnum);
        sprintf(strnum, "%.0f", ((10+(caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
        +MAGUS_TUTOR_COST) * 0.01)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (40 + 40% Mastery) Mastery and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 1% missing Stamina) Crit Multiplier.");
        return result;
    case AB_MAGUS_SARCOPHAGUS:
        sprintf(strnum, "%.0f", (((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_SARCOPHAGUS_COST)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give an ally ", strnum);
        sprintf(strnum, "%.0f", (((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.05) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_SARCOPHAGUS_COST) * 0.1) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (100% Mastery + 100% missing Stamina) Defense and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (5% Mastery + 10% missing Stamina) Armor until their next turn, while delaying it by another turn.");
        return result;
    case AB_RIPPER_REND:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.15)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        sprintf(strnum, "%.0f", ((2 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.25)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (10 + 15% Mastery) Bleed to an enemy and remove ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (2 + 25% Mastery) of their Armour. If there's no more Armour to remove, apply five times as much Pain instead.");
        return result;
    case AB_RIPPER_EVISCERATE:
        result = "Deal unavoidable damage to an enemy equal to twice the sum of negative effects they carry.";
        return result;
    case AB_RIPPER_STALK:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)));
        result = CombineStrings("Make an ally Untargettable for 500 ticks and give them ", strnum);
        result = CombineStrings(result, " (20 + 30% Mastery) Crit Bonus. This ability doesn't Crit.");
        return result;
    case AB_RIPPER_CHASE:
        sprintf(strnum, "%.0f", (60 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (60 + 100% Mastery) Exhaustion to an enemy and gain Mastery equal to 20% of their missing Health.");
        return result;
    case AB_CULTIST_MADDENING_TOUCH:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", ((200 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (40 + 100% Mastery) Damage and apply Confusion for ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (100 + 120% Mastery) ticks to an enemy.");
        return result;
    case AB_CULTIST_MANIA:
        sprintf(strnum, "%.0f", ((150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give an ally ", strnum);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (150 + 200% Mastery) Stamina Regen, ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (50 + 150% Mastery) Mastery and apply Confusion for 500 ticks.");
        return result;
    case AB_CULTIST_SUMMON:
        return "Create a helpful monster in an empty enemy spot.";
    case AB_CULTIST_PRAY:
        return "Re-randomize enemy intent. Sometimes might result in an additional boon (likeliness diminished by Mastery).";
    case AB_WOLF_BITE:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Target enemy gains ", strnum);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (20 + 40% Mastery) Berserk each time they get hit until your next turn. Deal ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (50 + 80% Mastery) damage to that enemy.");
        return result;
    case AB_WOLF_HUNT:
        sprintf(strnum, "%.0f", ((300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 3.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (300 + 300% Mastery) damage to an enemy, lose 50 Speed.");
        return result;
    case AB_WOLF_FERAL_AURA:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give everyone else ", strnum);
        result = CombineStrings(result, " (20 + 20% Mastery) Berserk.");
        return result;
    case AB_WOLF_PURSUE:
        return "Target enemy loses half of their Berserk and loses Speed equal to twice the Berserk lost.";
    case AB_GUIDE_RESONANT_STRIKE:
        sprintf(strnum, "%.0f", ((50+ (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Gain ", strnum);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (50 + 80% Mastery) Shield. Deal ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 20% Mastery) damage to an enemy. If your Defense is higher than the target's, the damage is further amplified by the difference.");
        return result;
    case AB_GUIDE_HARMONIZE:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give every ally ", strnum);
        result = CombineStrings(result, " (40 + 80% Mastery) Defense.");
        return result;
    case AB_GUIDE_DISTRACT:
        sprintf(strnum, "%.0f", ((300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Target ally becomes a guaranteed target of every enemy ability for ", strnum);
        result = CombineStrings(result, " (300 + 150% Mastery) ticks.");
        return result;
    case AB_GUIDE_DANCE_OF_THE_DESPERATE:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give every ally ", strnum);
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (100 + 60% Mastery) Stamina and Max Stamina, apply ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (30 + 100% Mastery) Pain to self.");
        return result;
        case AB_CULTIST_SPAWN_ENROOT:
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 40% Mastery) damage to all allies.");
        return result;
    case AB_CULTIST_SPAWN_SPORES:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Confuse all allies for ", strnum);
        result = CombineStrings(result, " (100 + 100% Mastery) ticks.");
        return result;
    case AB_CULTIST_SPAWN_GROW:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Gain ", strnum);
        result = CombineStrings(result, " (40 + 80% Mastery) Speed.");
        return result;
        case AB_CULTIST_SPAWN_INVIGORATE:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give ", strnum);
        result = CombineStrings(result, " (40 + 40% Mastery) Stamina to an enemy.");
        return result;
    case AB_MIMIC_CHOMP:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Attack an enemy for ", strnum);
        result = CombineStrings(result, " (100 + 20% Mastery) damage, and remove ");
        sprintf(strnum, "%.0f", ((150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (150 + 80% Mastery) of their Stamina.");
        return result;
    case AB_MIMIC_IMPALE:
        sprintf(strnum, "%.0f", ((150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (150 + 120% Mastery) damage to all enemies.");
        return result;
    case AB_MIMIC_PETRIFY:
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give an enemy ", strnum);
        result = CombineStrings(result, " (50 + 100% Mastery) Defense, and remove the same amount of Speed.");
        return result;
    case AB_BLOFAEWAR_CUT:
        sprintf(strnum, "%.0f", ((1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Attack an enemy for ", strnum);
        result = CombineStrings(result, " (1 + 20% Mastery) damage, then apply the unmitigated damage as Bleed points.");
        return result;
    case AB_BLOFAEMYS_INSPIRE:
        return "Give 10 Mastery to every ally.";
    case AB_BLOFAEMYS_HASTE:
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Give an entity ", strnum);
        result = CombineStrings(result, " (30 + 30% Mastery) Speed.");
        return result;
    case AB_BLOFAEMYS_MOCK:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Mock an enemy, delaying their turn by ", strnum);
        result = CombineStrings(result, " (100 + 200% Mastery) ticks.");
        return result;
    case AB_STEVENANT_SIPHON:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (20 + 100% Mastery) Exhaustion to an enemy and gain that much Speed.");
        return result;
    case AB_STEVENANT_PHASING_STRIKE:
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (20 + 100% Mastery) damage to an enemy and become Untargettable for 300 ticks.");
        return result;
    case AB_STEVENANT_ECTOPLASMIC_MANIFESTATION:
        result = "Gain 30 Mastery and Defense.";
        return result;
    default:
        return "Ability description missing, oopsie!";
    }
}

void CastAbility(ABILITY id, short cost, CreatureStats *caster, CreatureStats **targets, short numberOfTargets)
{
    appState.stateData.gameState.stateData.battleState.takeAnotherTurn = false;
    char *message;
    char strnum[6];
    short primaryEffectValue;
    bool isCrit = (*caster).baseStats.critCounter >= CRIT_PROGRESS_MAX;
    bool dontResetCritProgress = false;

    caster->baseStats.currentStamina -= cost;

    switch (id)
    {
    case AB_WAIT:
        dontResetCritProgress = true;
        AddMessageToFeed(CombineStrings(caster->baseStats.name, " does nothing."));
        break;
    case AB_BERSERKER_SWING:
        short berserkerSwingRageGain = ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateEffectAmplification(caster, false);
        caster->statusEffects[SE_BERSERK] += berserkerSwingRageGain;
        primaryEffectValue = ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " hacks at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", shredding ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings(message, " Defense, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and gaining ");
        sprintf(strnum, "%d", berserkerSwingRageGain);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Berserk.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->encounterStats.defense -= primaryEffectValue;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_BERSERKER_BASH:
        primaryEffectValue = CalculateDamage((((caster->baseStats.armor + caster->encounterStats.armor + caster->itemStats.armor) * (caster->statusEffects[SE_BERSERK] + 1))) * CalculateEffectAmplification(caster, false), targets[0]);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " slams ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with a shield, delaying their turn by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " ticks.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.ticksUntilNextTurn += primaryEffectValue;
        break;
    case AB_BERSERKER_BATTLECRY:
        primaryEffectValue = (((caster->baseStats.currentStamina) * 0.2)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " roars a mighty battlecry, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Target Priority and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Berserk.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->statusEffects[SE_BERSERK] += primaryEffectValue;
        caster->encounterStats.targetPriority += primaryEffectValue;
        caster->baseStats.currentStamina /= 2;
        appState.stateData.gameState.stateData.battleState.takeAnotherTurn = true;
        break;
    case AB_BERSERKER_BRACE:
        short berserkerBraceArmorGain = ((1 + (caster->statusEffects[SE_BERSERK]) * 0.05)) * CalculateEffectAmplification(caster, false);
        primaryEffectValue = ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " raises his shield, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Armor and ");
        sprintf(strnum, "%d", berserkerBraceArmorGain);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Defense until next turn.");
        StatBonuses berserkerBraceStatBonus = CreateEmptyStatBonuses();
        berserkerBraceStatBonus.armor = berserkerBraceArmorGain;
        berserkerBraceStatBonus.defense = primaryEffectValue;
        StatDebuff berserkerBraceStatBuff = (StatDebuff){CalculateNextTurnTicks(caster), berserkerBraceStatBonus};
        ApplyStatDebuff(caster, berserkerBraceStatBuff);
        appState.stateData.gameState.stateData.battleState.takeAnotherTurn = true;
        break;
    case AB_ASSASSIN_SLASH:
        primaryEffectValue = (30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6) * CalculateEffectAmplification(caster, true);
        short assassinSlashBleed = (15 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " slashes ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and applying ");
        sprintf(strnum, "%d", assassinSlashBleed);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Bleed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_BLEED] += assassinSlashBleed;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_ASSASSIN_PREPARE:
        primaryEffectValue = (((caster->baseStats.critRate + caster->encounterStats.critRate + caster->itemStats.critRate) * (1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.01)));
        short assassinPrepareCritMult = ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3));
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " prepares in the shadows, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Crit Progress and  ");
        sprintf(strnum, "%d", assassinPrepareCritMult);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, "% Crit Bonus.");
        caster->baseStats.critCounter += primaryEffectValue;
        caster->encounterStats.critBonus += assassinPrepareCritMult;
        dontResetCritProgress = true;
        AddMessageToFeed(message);
        break;
    case AB_ASSASSIN_CONCEAL:
        primaryEffectValue = (300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " applies a concealing hex to ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " ticks, making them untargettable.");
        targets[0]->statusEffects[SE_UNTARGETTABLE] = primaryEffectValue;
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        break;
    case AB_ASSASSIN_REND:
        primaryEffectValue = (4 * targets[0]->statusEffects[SE_BLEED]) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " rends ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " apart from the inside, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " unavoidable damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], true, caster);
        break;
    case AB_DUELIST_LUNGE:
        primaryEffectValue = ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        short duelistLungeSpeed = ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, false);
        message = CombineStrings((*caster).baseStats.name, " lunges at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and gaining ");
        sprintf(strnum, "%d", duelistLungeSpeed);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        caster->encounterStats.speed += duelistLungeSpeed;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_DUELIST_OPPORTUNITY:
        primaryEffectValue = ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8
            + (caster->baseStats.speed + caster->encounterStats.speed + caster->itemStats.speed) * 0.1)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " creates an opening, amplifying the potency of skills by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, "%, ");
        sprintf(strnum, "%d", appState.stateData.gameState.stateData.battleState.opportunitySkillCountdown);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " turns from now.");
        AddMessageToFeed(message);
        appState.stateData.gameState.stateData.battleState.opportunityMult = (float)primaryEffectValue / 100.0;
        break;
    case AB_DUELIST_PARRY:
        primaryEffectValue = ((5 + (caster->baseStats.speed + caster->encounterStats.speed + caster->itemStats.speed) * 0.05)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " assumes a defensive stance, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Armor until next turn.");
        AddMessageToFeed(message);
        StatBonuses duelistParryStatBonus = CreateEmptyStatBonuses();
        duelistParryStatBonus.armor = primaryEffectValue;
        StatDebuff duelistParryStatBuff = (StatDebuff){CalculateNextTurnTicks(caster), duelistParryStatBonus};
        LingeringEffect duelistParryEffect = (LingeringEffect){LE_ONHIT_DUELIST_PARRY, CalculateNextTurnTicks(caster), 0};
        ApplyLingeringEffect(caster, duelistParryEffect);
        ApplyStatDebuff(caster, duelistParryStatBuff);
        AddCreatureToFlicker(caster);
        break;
    case AB_DUELIST_BREATH:
        primaryEffectValue = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        caster->baseStats.currentStamina += primaryEffectValue;
        (*caster).baseStats.critCounter += (*caster).baseStats.critRate + (*caster).itemStats.critRate + (*caster).encounterStats.critRate;
        message = CombineStrings((*caster).baseStats.name, " takes a steady breath, regaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina and greatly increasing Crit Progress.");
        AddMessageToFeed(message);
        break;
    case AB_MONK_MEDITATE:
        dontResetCritProgress = true;
        (*caster).baseStats.critCounter += (*caster).baseStats.critRate + (*caster).itemStats.critRate + (*caster).encounterStats.critRate;
        primaryEffectValue = (10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = (100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " uses True Strike, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " unavoidable damage to ");
        for (int i = 0; i < numberOfTargets; i++)
        {
            if (i == numberOfTargets - 1)
            {
                if (i != 0)
                {
                    message = CombineStrings(message, " and ");
                }
                message = CombineStrings(message, targets[i]->baseStats.name);
                message = CombineStrings(message, ".");
            }
            else
            {
                if (i != 0)
                {
                    message = CombineStrings(message, ", ");
                }
                message = CombineStrings(message, targets[i]->baseStats.name);
            }
        }
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], true, caster);
        }
        break;
    case AB_MONK_ATTUNEMENT:
        primaryEffectValue = (150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0) * CalculateEffectAmplification(caster, false);
        for (int i = 0; i < numberOfTargets; i++)
        {
            targets[i]->encounterStats.shield += primaryEffectValue;
            AddCreatureToFlicker(targets[i]);
        }
        sprintf(strnum, "%d", primaryEffectValue);
        if (isCrit)
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
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            EmptyStatusEffects(targets[i]);
        }
        if (isCrit)
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
        if ((appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 0)) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 1;
        }
        primaryEffectValue = (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " slams ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with a mass of chaotic flesh, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        // >:3
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_FOLEM_EXPUNGE:
        if ((appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 1)) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 2;
        }
        primaryEffectValue = (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.5)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        short folemExpungeValue = (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.2)) * CalculateEffectAmplification(caster, true);
        message = CombineStrings((*caster).baseStats.name, " explodes violently with viscera, dealing ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", CalculateDamage(folemExpungeValue, caster));
        message = CombineStrings(message, " damage to ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage to self.");
        AddCreatureToFlicker(caster);
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        DealDamage(folemExpungeValue, caster, false, caster);
        break;
    case AB_FOLEM_EPIDERMIZE:
        if ((appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 2)) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 4;
        }
        primaryEffectValue = ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.9)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        short folemEpidermizeShield = (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health) * 0.1) +
            ((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health - caster->baseStats.currentHealth) * 0.1) + 
        (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, false);
        message = CombineStrings((*caster).baseStats.name, " rapidly hardens its epidermis into a carapace, gaining ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", folemEpidermizeShield);
        message = CombineStrings(message, " Defense and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Shield points.");
        AddCreatureToFlicker(caster);
        AddMessageToFeed(message);
        caster->encounterStats.defense += primaryEffectValue;
        caster->encounterStats.shield += folemEpidermizeShield;
        break;
    case AB_FOLEM_CRIPPLE:
        if ((appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 3)) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 8;
        }
        primaryEffectValue = (((caster->baseStats.maxHealth + caster->encounterStats.health + caster->itemStats.health - caster->baseStats.currentHealth) * 0.1)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " mauls ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with its exposed endoskeleton, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Bleed.");
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        targets[0]->statusEffects[SE_BLEED] += primaryEffectValue;
        break;
    case AB_FOLEM_MEND:
        if ((appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 4)) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 16;
        }
        primaryEffectValue = CalculateDamage(caster->baseStats.currentHealth*0.3, caster);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " loses ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " health while filling ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, "'s wounds with malleable tissue, restoring ");
        sprintf(strnum, "%d", (short)(primaryEffectValue * 0.6));
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Health and cleansing their ailments.");
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        DealDamage(primaryEffectValue, caster, true, caster);
        targets[0]->baseStats.currentHealth += (short)(primaryEffectValue * 0.6);
        if(targets[0]->baseStats.currentHealth > targets[0]->baseStats.maxHealth + targets[0]->itemStats.health + targets[0]->encounterStats.health)
        {
            targets[0]->baseStats.currentHealth = targets[0]->baseStats.maxHealth + targets[0]->itemStats.health + targets[0]->encounterStats.health;
        }
        EmptyStatusEffects(targets[0]);
    break;
    case AB_SHAPESHIFTER_SCRATCH:
        primaryEffectValue = (10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " scratches ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_SHAPESHIFTER_TRANSFORM:
        message = CombineStrings((*caster).baseStats.name, " becomes ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ".");
        AddMessageToFeed(message);
        char abCount = targets[0]->abilityCount + 1;
        ABILITY *abilities = malloc(abCount * sizeof(int));
        for (int i = 0; i < targets[0]->abilityCount - 1; i++)
        {
            abilities[i] = targets[0]->abilities[i].abilityId;
        }
        abilities[abCount - 2] = AB_SHAPESHIFTER_TRANSFORM;
        abilities[abCount - 1] = AB_WAIT;
        caster->baseStats = targets[0]->baseStats;
        caster->baseStats.color = BEIGE;
        caster->abilities = InitAbilities(abilities, abCount);
        caster->abilityCount = abCount;
        AddCreatureToFlicker(caster);
        AddCreatureToFlicker(targets[0]);
        break;case AB_MAGUS_DISINTEGRATE:
        primaryEffectValue = ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.2) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina) * 0.3) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " shoots a ray of concentrated arcane energy at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], true, caster);
        break;
    case AB_MAGUS_ARCANE_BLAST:
        primaryEffectValue = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina) * 0.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " projects an explosive wave of arcane energy, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage to all enemies.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break;
    case AB_MAGUS_TUTOR:
        primaryEffectValue = ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        short magusTutorCM = ((10+(caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina) * 0.01)) * CalculateEffectAmplification(caster, false);
        if(targets[0] != caster)
        {
            message = CombineStrings((*caster).baseStats.name, " gives ");
            message = CombineStrings(message, targets[0]->baseStats.name);
            message = CombineStrings(message, " a chaotic lecture mid-battle, granting them ");
        }
        else
        {
            message = CombineStrings((*caster).baseStats.name, " takes a moment to study and practice, gaining ");
        }
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", magusTutorCM);
        message = CombineStrings(message, " Mastery and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Crit Multiplier.");
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        targets[0]->encounterStats.mastery += primaryEffectValue;
        targets[0]->encounterStats.critBonus += magusTutorCM;
        break;
    case AB_MAGUS_SARCOPHAGUS:
        primaryEffectValue = (((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina)) * CalculateEffectAmplification(caster, true);
        short magusSarcophagusArmor = (((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.05) + 
            (caster->baseStats.maxStamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina) * 0.1) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " entombs ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " in a sarcophagus, granting them ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Defense and ");
        sprintf(strnum, "%d", magusSarcophagusArmor);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Armour until their next turn, but delaying it by a whole another turn.");
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        targets[0]->baseStats.ticksUntilNextTurn += CalculateNextTurnTicks(targets[0]);
        StatBonuses magusSarcophagusBonus = CreateEmptyStatBonuses();
        magusSarcophagusBonus.armor = magusSarcophagusArmor;
        magusSarcophagusBonus.defense = primaryEffectValue;
        StatDebuff magusSarcophagusBuff = (StatDebuff){CalculateNextTurnTicks(caster), magusSarcophagusBonus};
        ApplyStatDebuff(caster, magusSarcophagusBuff);
        break;
    case AB_RIPPER_REND:
        primaryEffectValue = (10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.15) * CalculateEffectAmplification(caster, true);
        short ripperRendShred = (2 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.25) * CalculateEffectAmplification(caster, true);
        short ripperRendPain = (ripperRendShred > targets[0]->baseStats.armor + targets[0]->encounterStats.armor + targets[0]->itemStats.armor) ? 
        ripperRendShred - (targets[0]->baseStats.armor + targets[0]->encounterStats.armor + targets[0]->itemStats.armor) : 0;
        sprintf(strnum, "%d", primaryEffectValue, targets[0]);
        message = CombineStrings((*caster).baseStats.name, " hacks at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", applying ");
        message = CombineStrings(message, strnum);
        if(ripperRendPain != 0)
        {
            message = CombineStrings(message, " Bleed, removing ");
            sprintf(strnum, "%d", targets[0]->baseStats.armor + targets[0]->encounterStats.armor + targets[0]->itemStats.armor);
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Armour and applying ");
            sprintf(strnum, "%d", ripperRendPain * 5);
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Pain.");
        }
        else
        {
            message = CombineStrings(message, " Bleed and removing ");
            sprintf(strnum, "%d", ripperRendShred);
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Armour.");
        }
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_BLEED] += primaryEffectValue;
        targets[0]->encounterStats.armor -= ripperRendShred - ripperRendPain;
        targets[0]->statusEffects[SE_PAIN] += ripperRendPain * 5;
        DealDamage(0, targets[0], true, caster);
        break;
    case AB_RIPPER_EVISCERATE:
        primaryEffectValue = (2 * (targets[0]->statusEffects[SE_BLEED] + targets[0]->statusEffects[SE_BERSERK] + targets[0]->statusEffects[SE_EXHAUSTION] 
            + targets[0]->statusEffects[SE_PAIN])) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " tears ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " apart from the inside, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " unavoidable damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], true, caster);
        break;
    case AB_RIPPER_STALK:
        primaryEffectValue = (20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " conceals ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", making them Untargettable for 500 ticks and giving them ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, "% Crit Bonus.");
        targets[0]->statusEffects[SE_UNTARGETTABLE] = 500;
        targets[0]->encounterStats.critBonus += primaryEffectValue;
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        dontResetCritProgress = true;
        break;
    case AB_RIPPER_CHASE:
        if(targets[0]->baseStats.currentHealth <= 0)
        {
            AddMessageToFeed("Chasing the dead didn't prove very exciting.");
            break;
        }
        primaryEffectValue = (60 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        short ripperChaseMastery = ((targets[0]->baseStats.maxHealth + targets[0]->encounterStats.health + targets[0]->itemStats.health - targets[0]->baseStats.currentHealth) * 0.2);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " chases ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " maniacally, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Exhaustion and gaining ");
        sprintf(strnum, "%d", ripperChaseMastery);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Mastery through the excitement.");
        targets[0]->statusEffects[SE_EXHAUSTION] += primaryEffectValue;
        caster->encounterStats.mastery += ripperChaseMastery;
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
    break;
    case AB_CULTIST_MADDENING_TOUCH:
        primaryEffectValue = (40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        short cultistMadTouchConfusion = (200 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.2) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " invades ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, "'s mind with incomprehensible visions of the divine, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and applying ");
        sprintf(strnum, "%d", cultistMadTouchConfusion);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Confusion.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_CONFUSION] = cultistMadTouchConfusion;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_CULTIST_MANIA:
        primaryEffectValue = (150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0) * CalculateEffectAmplification(caster, true);
        short cultistManiaMastery = (100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue, targets[0]);
        message = CombineStrings((*caster).baseStats.name, " imbues ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with eldritch euphoria, granting ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina Regen, ");
        sprintf(strnum, "%d", cultistManiaMastery);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Mastery and inflicts Confusion for 500 ticks.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        StatBonuses cultistManiaBonus = CreateEmptyStatBonuses();
        cultistManiaBonus.mastery = cultistManiaMastery;
        cultistManiaBonus.staminaRegen = primaryEffectValue;
        StatDebuff cultistManiaBuff = (StatDebuff){500, cultistManiaBonus};
        ApplyStatDebuff(caster, cultistManiaBuff);
        targets[0]->statusEffects[SE_CONFUSION] = 500;
        break;
    case AB_CULTIST_SUMMON:
        if(targets[0]->baseStats.currentHealth>0)
        {
            caster->baseStats.currentStamina += InitAbility(id).staminaCost;
            appState.stateData.gameState.stateData.battleState.takeAnotherTurn = true;
            (*caster).baseStats.critCounter -= (*caster).baseStats.critRate + (*caster).itemStats.critRate + (*caster).encounterStats.critRate;
            ShowPopupMessage("Can't summon on an occupied space.");
        }
        else
        {
            for(int i = 0; i < 3; i++)
            {
                if(&appState.stateData.gameState.stateData.battleState.enemies[i].stats != targets[0]) continue;

                appState.stateData.gameState.stateData.battleState.enemies[i] = InitEnemyData(EN_CULTIST_SUMMON);
                appState.stateData.gameState.stateData.battleState.enemies[i].stats.baseStats.mastery = 
                    (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5 * CalculateEffectAmplification(caster, true);
                message = CombineStrings((*caster).baseStats.name, " summons the spawn of The Mindless One, blessing the earth with its presence.");
                AddCreatureToFlicker(targets[0]);
                AddMessageToFeed(message);
            }
        }
        break;
    case AB_CULTIST_PRAY:
        message = CombineStrings((*caster).baseStats.name, " prays to The Mindless One, causing fate to change.");
        AddMessageToFeed(message);
        appState.stateData.gameState.stateData.battleState.prayerFatigue++;
        if(rng_next_u32(&appState.stateData.gameState.stateData.battleState.battleRng) % (500 + (caster->baseStats.mastery + 
            caster->encounterStats.mastery + caster->itemStats.mastery)*4 + appState.stateData.gameState.stateData.battleState.prayerFatigue * 15) < 400 * CalculateEffectAmplification(caster, true))
        {
            CreatureStats* target;
            switch(rand() % 8)
            {
                case 0:
                target = &appState.stateData.gameState.playerTeam[rand()%3].stats;
                (*target).encounterStats.shield += 50;
                message = CombineStrings("The Mindless One grants ", target->baseStats.name);
                message = CombineStrings(message, " 50 Shield.");
                AddMessageToFeed(message);
                break;
                case 1:
                target = &appState.stateData.gameState.stateData.battleState.enemies[rand()%3].stats;
                message = CombineStrings("The Mindless One smites ", target->baseStats.name);
                message = CombineStrings(message, " for 100 damage.");
                AddMessageToFeed(message);
                DealDamage(100, target,false, NULL);
                break;
                case 2:
                Item cultistPrayItem = InitItem(itemPoolTier1[rand() % ITEM_POOL_TIER_ONE_SIZE]);
                message = CombineStrings("The Mindless One conjures ", cultistPrayItem.name);
                message = CombineStrings(message, " inside your shared inventory.");
                AddItemToInventory(cultistPrayItem);
                AddMessageToFeed(message);
                break;
                case 3:
                Character* cultistPrayUpgradeCharacter = &appState.stateData.gameState.playerTeam[rand()%3];
                Item* cultistPrayUpgradeItem = &(*cultistPrayUpgradeCharacter).items[rand()%4];
                if(cultistPrayUpgradeItem->itemId == ITEM_NONE) break;
                cultistPrayUpgradeItem->statBonuses.mastery += 5;
                cultistPrayUpgradeCharacter->stats.itemStats.mastery += 5;
                message = CombineStrings("The Mindless One upgrades ", (*cultistPrayUpgradeItem).name);
                message = CombineStrings(message, " used by ");
                message = CombineStrings(message, (*cultistPrayUpgradeCharacter).stats.baseStats.name);
                message = CombineStrings(message, ", imbuing it with 5 additional Mastery.");
                AddMessageToFeed(message);
                break;
                case 4:
                target = &appState.stateData.gameState.playerTeam[rand()%3].stats;
                (*target).baseStats.currentStamina += 100;
                message = CombineStrings("The Mindless One grants ", target->baseStats.name);
                message = CombineStrings(message, " 100 Stamina.");
                AddMessageToFeed(message);
                break;
                case 5:
                AddMessageToFeed("The Mindless One chirps an alien melody.");
                break;
                case 6:
                target = &appState.stateData.gameState.playerTeam[rand()%3].stats;
                dontResetCritProgress = true;
                (*target).baseStats.critCounter += 50;
                message = CombineStrings("The Mindless One blesses ", target->baseStats.name);
                message = CombineStrings(message, " with luck, granting them 50 Crit Progress.");
                AddMessageToFeed(message);
                break;
                case 7:
                AddMessageToFeed("The Mindless One cackles like mad. Something seems to be different...");
                appState.stateData.gameState.playerTeam[0].stats.baseStats.color = GREEN;
                appState.stateData.gameState.playerTeam[1].stats.baseStats.color = DARKPURPLE;
                appState.stateData.gameState.playerTeam[2].stats.baseStats.color = RED;
                break;
                default:
                AddMessageToFeed("The Mindless One laughs jubilantly, the otherworldly voice echoing across the cave system.");
                break;
            }
        }
        break;
    case AB_WOLF_BITE:
        primaryEffectValue = (50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) * CalculateEffectAmplification(caster, true);
        short wolfBiteBerserk = (20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " bites ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and aggravating the wound.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        LingeringEffect wolfBiteEffect = (LingeringEffect){LE_ONHIT_WOLF_BITE, CalculateNextTurnTicks(caster), 0, wolfBiteBerserk};
        ApplyLingeringEffect(targets[0], wolfBiteEffect);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_WOLF_HUNT:
        primaryEffectValue = (300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 3.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " springs at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with a lethal attack, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and losing 50 Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        caster->encounterStats.speed -= 50;
        DealDamage(primaryEffectValue, targets[0], false, caster);
    case AB_WOLF_FERAL_AURA:
        primaryEffectValue = (20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " stands ominously still, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Berserk to everyone else.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            if(targets[i] == caster) continue;
            AddCreatureToFlicker(targets[i]);
            targets[i]->statusEffects[SE_BERSERK] += primaryEffectValue;
        }
        break;
    case AB_WOLF_PURSUE:
        primaryEffectValue = targets[0]->statusEffects[SE_BERSERK];
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " steadily paces behind ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", removing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Berserk and ");
        sprintf(strnum, "%d", primaryEffectValue*2);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_BERSERK] -= primaryEffectValue;
        targets[0]->encounterStats.speed -= primaryEffectValue * 2;
        break;
    case AB_GUIDE_RESONANT_STRIKE:
        primaryEffectValue = (10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2 +
            MAX(0, (caster->baseStats.defense + caster->encounterStats.defense + caster->itemStats.defense) - 
            (targets[0]->baseStats.defense + targets[0]->encounterStats.defense + targets[0]->itemStats.defense))) * CalculateEffectAmplification(caster, true);
        short guideResonantStrikeShield = (50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " strikes ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with their bell, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and gaining ");
        sprintf(strnum, "%d", guideResonantStrikeShield);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Shield points.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        caster->encounterStats.shield += guideResonantStrikeShield;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_GUIDE_HARMONIZE:
        primaryEffectValue = (40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " plays a somber note with their bell, attuning the team to its resonance, granting all allies ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Defense.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            targets[i]->encounterStats.defense += primaryEffectValue;
        }
        break;
    case AB_GUIDE_DISTRACT:
        primaryEffectValue = (300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " creates an aggravating dissonance near ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", making them the  centre of attention for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " ticks.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        appState.stateData.gameState.stateData.battleState.distractionTimer = primaryEffectValue;
        for(int i = 0; i < 3; i++)
        {
            if(&appState.stateData.gameState.playerTeam[i].stats == targets[0])
            {
                appState.stateData.gameState.stateData.battleState.distractionTarget = i;
                break;
            }
            if(&appState.stateData.gameState.stateData.battleState.enemies[i].stats == targets[0])
            {
                appState.stateData.gameState.stateData.battleState.distractionTarget = i + 3;
                break;
            }
        }
        break;
    case AB_GUIDE_DANCE_OF_THE_DESPERATE:
        primaryEffectValue = (100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6) * CalculateEffectAmplification(caster, true);
        short guideDancePain = (30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue, targets[0]);
        message = CombineStrings((*caster).baseStats.name, " enters a feverish dance, granting every ally ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina and Max Stamina, while applying ");
        sprintf(strnum, "%d", guideDancePain);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Pain to self.");
        AddMessageToFeed(message);
        caster->statusEffects[SE_PAIN] += guideDancePain;
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            targets[i]->encounterStats.stamina += primaryEffectValue;
            targets[i]->baseStats.currentStamina += primaryEffectValue;
        }
        break;
    case AB_CULTIST_SPAWN_ENROOT:
        primaryEffectValue = (50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " expands its roots, piercing and strangling its allies for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            if(targets[i] == caster) continue;
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break;
    case AB_CULTIST_SPAWN_SPORES:
        primaryEffectValue = (100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " explodes with its spores, confusing its allies for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " ticks.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            if(targets[i] == caster) continue;
            AddCreatureToFlicker(targets[i]);
            targets[i]->statusEffects[SE_CONFUSION] = primaryEffectValue;
        }
        break;
    case AB_CULTIST_SPAWN_GROW:
        primaryEffectValue = ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        caster->encounterStats.speed += primaryEffectValue;
        message = CombineStrings((*caster).baseStats.name, " rapidly grows in complexity, giving itself ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        break;
        case AB_CULTIST_SPAWN_INVIGORATE:
        primaryEffectValue = (40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " radiates a nurturing aura, granting ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.currentStamina += primaryEffectValue;
        break;
    case AB_MIMIC_CHOMP:
        primaryEffectValue = (100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4) * CalculateEffectAmplification(caster, true);
        short mimicChompStaminaLoss = (150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " bites down on ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage. ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " expends ");
        sprintf(strnum, "%d", mimicChompStaminaLoss);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina in their struggle to get free.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.currentStamina -= mimicChompStaminaLoss;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_MIMIC_IMPALE:
        primaryEffectValue = (150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.2) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " opens its maw in an explosion of sharp spikes, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage to all enemies.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break;
    case AB_MIMIC_PETRIFY:
        primaryEffectValue = (50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " stings ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", injecting them with a petrifying toxin. They gain ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Defense, and lose ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        targets[0]->encounterStats.defense += primaryEffectValue;
        targets[0]->encounterStats.speed -= primaryEffectValue;
        AddCreatureToFlicker(targets[0]);
        break;
    case AB_BLOFAEWAR_CUT:
        primaryEffectValue = CalculateDamage(((1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true), targets[0]);
        primaryEffectValue = primaryEffectValue < 0 ? 0 : primaryEffectValue;
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " cuts ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Bleed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(0, targets[0], true, caster); // 0 damage proc to cause on-hit reactions
        targets[0]->statusEffects[SE_BLEED] += primaryEffectValue;
        break;
    case AB_BLOFAEMYS_INSPIRE:
        primaryEffectValue = 10 * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        for (int i = 0; i < numberOfTargets; i++)
        {
            targets[i]->encounterStats.mastery += primaryEffectValue;
        }
        message = CombineStrings((*caster).baseStats.name, " sings an ancient fae hymn, increasing Mastery by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " for each team member.");
        AddMessageToFeed(message);
        break;
    case AB_BLOFAEMYS_HASTE:
        primaryEffectValue = (30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " spurs ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " on, granting ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->encounterStats.speed += primaryEffectValue;
        break;
    case AB_BLOFAEMYS_MOCK:
        primaryEffectValue = ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " giggles at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", delaying their turn by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " ticks out of shyness.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.ticksUntilNextTurn += primaryEffectValue;
        break;
    case AB_STEVENANT_SIPHON:
        primaryEffectValue = (20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " siphons ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, "'s life force, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Exhaustion and gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        targets[0]->statusEffects[SE_EXHAUSTION] += primaryEffectValue;
        caster->encounterStats.speed += primaryEffectValue;
        AddCreatureToFlicker(targets[0]);
        break;
    case AB_STEVENANT_PHASING_STRIKE:
        primaryEffectValue = (30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " passes through ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with malice, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and becoming untargettable for 300 ticks.");
        AddMessageToFeed(message);
        caster->statusEffects[SE_UNTARGETTABLE] = 300;
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_STEVENANT_ECTOPLASMIC_MANIFESTATION:
        primaryEffectValue = 30 * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " stirs its ectoplasm, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Mastery and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Defense.");
        AddMessageToFeed(message);
        caster->encounterStats.mastery += primaryEffectValue;
        caster->encounterStats.defense += primaryEffectValue;
        AddCreatureToFlicker(caster);
        break;
    default:
        message = CombineStrings((*caster).baseStats.name, " uses an ability that wasn't implemented yet, how embarassing!");
        AddMessageToFeed(message);
        break;
    }

    for(int i = 0; i<numberOfTargets;i++)
    {
        HandlePain(targets[i]);
    }

    if (!dontResetCritProgress)
    {
        (*caster).baseStats.critCounter = (*caster).baseStats.critCounter % CRIT_PROGRESS_MAX;
    }
    (*caster).baseStats.critCounter += (*caster).baseStats.critRate + (*caster).itemStats.critRate + (*caster).encounterStats.critRate;

    // Monk's quirk
    if (strcmp(caster->baseStats.name, "Monk") == 0)
    {
        if ((*caster).baseStats.critCounter >= CRIT_PROGRESS_MAX)
        {
            if (!DoesAbilityHaveFlag(caster->abilities[1], AF_AOE))
            {
                caster->abilities[1].abilityFlags += AF_AOE;
                caster->abilities[2].abilityFlags += AF_AOE;
                caster->abilities[3].abilityFlags += AF_AOE;
            }
        }
        else
        {
            if (DoesAbilityHaveFlag(caster->abilities[1], AF_AOE))
            {
                caster->abilities[1].abilityFlags -= AF_AOE;
                caster->abilities[2].abilityFlags -= AF_AOE;
                caster->abilities[3].abilityFlags -= AF_AOE;
            }
        }
    }
}