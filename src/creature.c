#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "creature.h"
#include "state.h"
#include "colors.h"
#include "drawing.h"
#include "audio.h"

extern struct AppState appState;
extern ITEM_ID itemPoolTier1[];
extern ITEM_ID itemPoolTier2[];

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
        if(c->lingeringEffects[i].isPermanent) continue;
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
        if (c->lingeringEffects[i].tickDuration < 1 && c->lingeringEffects[i].triggerLimit < 1 && c->lingeringEffects[i].isPermanent == false)
        {
            ShowLingeringEffectPassedMessage(c, c->lingeringEffects[i].effectId);
            c->lingeringEffects[i].effectId = LE_NONE;
        }
    }
}

void ShowLingeringEffectPassedMessage(CreatureStats* c, LINGERING_EFFECT e)
{
    if((*c).baseStats.currentHealth <= 0) return;

    char *message;
    switch(e)
    {
        case LE_ONHIT_WOLF_BITE:
        message = CombineStrings((*c).baseStats.name, "'s bite mark stops itching.");
        AddMessageToFeed(message);
        break;
    }
}

char* GetLingeringEffectDescription(LINGERING_EFFECT e)
{
    switch(e)
    {
        case LE_AMULET_OF_YENDOR: return "Whoever holds it is an equal to Gods.";
        case LE_ONABIL_COMFORTABLE_SHOES: return "When using an ability, regain 50 stamina.";
        case LE_ONABIL_WANDERER_HAT: return "When using an ability, gain half of the Stamina spent as Shield.";
        case LE_ONABIL_LIVING_CHAINMAIL: return "When using an ability, gain 30 Defense.";
        case LE_ONABIL_ASSASSIN_SHOES: return "When using Wait, gain 100 Crit Progress.";
        case LE_ONABIL_SCRYER_BALL: return "The sole target of your ability gains 241 Untargetable.";
        case LE_ONABIL_ANTLER_HELMET: return "When using an ability which targets only allies, gain 30 Mastery.";
        case LE_ONABIL_MASTER_ROBE: return "When targetting allies, transfer your Crit Progress to them.";
        case LE_ONATT_HAMMER_OF_GLORY: return "When attacking, apply 10 Pain and Exhaust.";
        case LE_ONATT_BALL_ON_CHAIN: return "When attacking, delay the target's turn by 100 moments.";
        case LE_ONATT_MORNINGSTAR: return "When attacking, apply 10 Bleed.";
        case LE_ONATT_MALEVOLENCE: return "When attacking, apply all the negative status you carry.";
        case LE_ONDTH_VENGEFUL_SPIRIT: return "On death, deal 1000 unavoidable damage to opposite enemy.";
        case LE_ONDTH_JAR_BREAK_GOODIES: return "On death, drop a random tier 2 item.";
        case LE_ONENC_COIN: return "At the start of combat, you move first.";
        case LE_ONENC_ILLUSION_PROTECT: return "At the start of combat, you gain 1 Protection.";
        case LE_ONHIT_PENITENT_MAIL: return "When hit, gain 2 Bleed and 10 Speed.";
        case LE_ONHIT_CRYSTAL_MAIL: return "When hit, this item loses 1 Armour permanently.";
        case LE_ONHIT_FOOT_FUNGUS: return "When hit, Pray.";
        case LE_ONTURN_AMBROSIA: return "You're always confused.";
        default: return NULL;
    }
}

void HandleOnHitEffects(CreatureStats *c, short damage, CreatureStats *caster)
{
    char strnum[8];
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
            message = CombineStrings(message, " Speed and applying ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Pain to the assailant.");
            AddMessageToFeed(message);
            c->encounterStats.speed += primaryValue;
            if(caster != NULL)
                caster->statusEffects[SE_PAIN] += primaryValue;
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
        case LE_ONHIT_PENITENT_MAIL:
            if(caster == NULL) break;
            message = CombineStrings("The impact of the blow causes the thorns inside ", (*c).baseStats.name);
            message = CombineStrings(message, "'s armor to puncture them, applying 2 Bleed and sobering them with 10 additional Speed.");
            AddMessageToFeed(message);
            c->encounterStats.speed += 10;
            c->statusEffects[SE_BLEED] += 2;
            break;
        case LE_ONHIT_CRYSTAL_MAIL:
            AddMessageToFeed("The Crystal Mail chips and shatters, losing 1 Armour.");
            c->encounterStats.armor -= 1;
            for(int j = 0; j < 3; j++)
            {
                if(&appState.stateData.gameState.playerTeam[j].stats == c)
                appState.stateData.gameState.playerTeam[j].items[1].statBonuses.armor -= 1;
            }
            break;
        case LE_ONHIT_FOOT_FUNGUS:
            CreatureStats footFungusDummy;
            footFungusDummy.baseStats.name = "The fungal colony, distressed by its host's hurt,";
            footFungusDummy.baseStats.mastery = c->baseStats.mastery + c->encounterStats.mastery + c->itemStats.mastery - 50;
            CastAbility(AB_CULTIST_PRAY,0,&footFungusDummy,(CreatureStats *[0]){}, 0);
        }
    }
}

void HandleOnAttackEffects(CreatureStats *c, short damage, CreatureStats *caster)
{
    if(caster == NULL) return;
    char strnum[8];
    char *message;
    short primaryValue;
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (caster->lingeringEffects[i].effectId == LE_NONE)
            continue;
        switch (caster->lingeringEffects[i].effectId)
        {
            case LE_ONATT_HAMMER_OF_GLORY:
                AddMessageToFeed("The magical hammer applies 10 Pain through the power of flame and 10 Exhaust through the power of frost.");
                c->statusEffects[SE_PAIN]+=10;
                c->statusEffects[SE_EXHAUSTION]+=10;
                break;
            case LE_ONATT_BALL_ON_CHAIN:
                AddMessageToFeed("The Ball on Chain swings with the attack, hitting the target, stunning them for 100 moments and delaying their turn.");
                c->baseStats.ticksUntilNextTurn += 100;
                break;
            case LE_ONATT_MORNINGSTAR:
                AddMessageToFeed("The spike-covered ball of the Morningstar mauls its target, applying 10 Bleed.");
                c->statusEffects[SE_BLEED]+=10;
                break;
            case LE_ONATT_MALEVOLENCE:
                message = CombineStrings("As ", (*caster).baseStats.name);
                message = CombineStrings(message, " puts all their hatred into the swing, it carries with it all the afflictions they carry.");
                AddMessageToFeed(message);
                c->statusEffects[SE_BLEED]+=caster->statusEffects[SE_BLEED];
                c->statusEffects[SE_BERSERK]+=caster->statusEffects[SE_BERSERK];
                c->statusEffects[SE_EXHAUSTION]+=caster->statusEffects[SE_EXHAUSTION];
                c->statusEffects[SE_PAIN]+=caster->statusEffects[SE_PAIN];
                break;
        }
    }
}

void HandleStartOfTurnEffects(CreatureStats* c)
{
    char strnum[8];
    char *message;
    short primaryValue;
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (c->lingeringEffects[i].effectId == LE_NONE)
            continue;
        switch (c->lingeringEffects[i].effectId)
        {
            case LE_ONTURN_AMBROSIA:
            if(c->statusEffects[SE_CONFUSION] < 1)
            c->statusEffects[SE_CONFUSION] = 1;
            break;
        }
    }
}

void HandleStartOfCombatEffects(CreatureStats* c)
{
    char strnum[8];
    char *message;
    short primaryValue;
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (c->lingeringEffects[i].effectId == LE_NONE)
            continue;
        switch (c->lingeringEffects[i].effectId)
        {
            case LE_ONENC_COIN:
            message = CombineStrings(c->baseStats.name, " wins the coin flip and acts first.");
            AddMessageToFeed(message);
            c->baseStats.ticksUntilNextTurn = 0;
            break;

            case LE_ONENC_ILLUSION_PROTECT:
            message = CombineStrings(c->baseStats.name, "'s armor swirls in a strange pattern, granting 1 Protection.");
            AddMessageToFeed(message);
            c->statusEffects[SE_PROTECTION] +=1;
            break;
        }
    }
}

void HandleOnDeathEffects(CreatureStats *c)
{
    // Death sound
    PlaySFX(SFX_Death);

    char strnum[8];
    char *message;
    short primaryValue;
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (c->lingeringEffects[i].effectId == LE_NONE)
            continue;
        switch (c->lingeringEffects[i].effectId)
        {
            case LE_ONDTH_VENGEFUL_SPIRIT:
            for(int j = 0; j < 3; j++)
            {
                if(&appState.stateData.gameState.playerTeam[j].stats != c) continue;
                message = CombineStrings("The Vengeful Spirit rushes at ", appState.stateData.gameState.stateData.battleState.enemies[2-j].stats.baseStats.name);
                message = CombineStrings(message, " furiously, dealing 1000 unavoidable damage.");
                AddMessageToFeed(message);
                DealDamage(1000, &appState.stateData.gameState.stateData.battleState.enemies[2-j].stats, true, NULL);
            }
            break;
            case LE_ONDTH_JAR_BREAK_GOODIES:
                Item jarBreakItem = InitItem(itemPoolTier1[rand() % ITEM_POOL_TIER_ONE_SIZE]);
                message = CombineStrings("Out of the jar drops ", jarBreakItem.name);
                message = CombineStrings(message, "! You swiftly pick it up and add it to your shared inventory.");
                AddItemToInventory(jarBreakItem);
                AddMessageToFeed(message);
            break;
            case LE_ONDTH_MANRAT:
            bool hasAllies = false;
            for(int j = 0; j < 3; j++)
            {
                if(appState.stateData.gameState.stateData.battleState.enemies[j].stats.baseStats.currentHealth < 1) continue;
                appState.stateData.gameState.stateData.battleState.enemies[j].stats.statusEffects[SE_BERSERK]+=100;
                appState.stateData.gameState.stateData.battleState.enemies[j].stats.baseStats.ticksUntilNextTurn = 0;
                hasAllies = true;
            }
            if(hasAllies == false) continue;
            message = CombineStrings((*c).baseStats.name, "'s death is avenged by its remaining allies, who gain 100 Berserk and immediately take a turn.");
            AddMessageToFeed(message);
            break;
        }
    }
    
    // Ripper reset
    for(int i = 0; i<3;i++)
    {
        if(appState.stateData.gameState.playerTeam[i].characterId != CHAR_RIPPER) continue;
        if(appState.stateData.gameState.playerTeam[i].stats.baseStats.currentHealth < 1) continue;
        appState.stateData.gameState.playerTeam[i].stats.baseStats.ticksUntilNextTurn = 0;
        appState.stateData.gameState.playerTeam[i].stats.baseStats.critCounter += 100;
        AddMessageToFeed("The Ripper gets excited and acts out of turn.");
    }
}

void HandleOnAbilityEffects(CreatureStats *c, ABILITY a, CreatureStats **targets, short numberOfTargets)
{
    char strnum[8];
    char *message;
    short primaryValue;
    for (int i = 0; i < LINGERING_EFFECTS; i++)
    {
        if (c->lingeringEffects[i].effectId == LE_NONE)
            continue;
        switch (c->lingeringEffects[i].effectId)
        {
            case LE_ONABIL_COMFORTABLE_SHOES:
            message = CombineStrings("The comfort of the shoes helps ", (*c).baseStats.name);
            message = CombineStrings(message, " regain their strength, granting them 50 Stamina.");
            AddMessageToFeed(message);
            c->baseStats.currentStamina += 50;
            break;
            case LE_ONABIL_WANDERER_HAT:
            primaryValue = InitAbility(a).staminaCost / 2;
            if(primaryValue == 0) break;
            c->encounterStats.shield += primaryValue;
            message = CombineStrings("As ", (*c).baseStats.name);
            message = CombineStrings(message, " uses the ability, their hat conjures ");
            sprintf(strnum, "%d", primaryValue);
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Shield.");
            AddMessageToFeed(message);
            break;
            case LE_ONABIL_LIVING_CHAINMAIL:
            message = CombineStrings("The chainmail shifts and adapts, granting ", (*c).baseStats.name);
            message = CombineStrings(message, " 30 additional Defense.");
            AddMessageToFeed(message);
            c->encounterStats.defense += 30;
            break;
            case LE_ONABIL_ASSASSIN_SHOES:
            if(a != AB_WAIT) break;
            message = CombineStrings("Through patience and focus ", (*c).baseStats.name);
            message = CombineStrings(message, " empowers their next move to be critical.");
            AddMessageToFeed(message);
            c->baseStats.critCounter += 100;
            break;
            case LE_ONABIL_SCRYER_BALL:
            if (numberOfTargets == 1) {
                targets[0]->statusEffects[SE_UNTARGETABLE] = 241;
                message = CombineStrings((*c).baseStats.name, " uses the ball to predict the future, helping ");
                message = CombineStrings(message, targets[0]->baseStats.name);
                message = CombineStrings(message, " avoid attacks.");
                AddMessageToFeed(message);
            }
            break;
            case LE_ONABIL_ANTLER_HELMET:
            if (numberOfTargets > 0) {
                bool only_allies = true;
                for (int j = 0; j < numberOfTargets; j++) {
                    bool is_ally = false;
                    for (int k = 0; k < 3; k++) {
                        if (targets[j] == &appState.stateData.gameState.playerTeam[k].stats) {
                            is_ally = true;
                            break;
                        }
                    }
                    if (!is_ally) {
                        only_allies = false;
                        break;
                    }
                }

                if (only_allies) {
                    c->encounterStats.mastery += 30;
                    message = CombineStrings("The enchanted helmet approves of ", (*c).baseStats.name);
                    message = CombineStrings(message, "'s comradery, granting 30 additional Mastery.");
                    AddMessageToFeed(message);
                }
            }
            break;
            case LE_ONABIL_MASTER_ROBE:
                if (numberOfTargets < 1)
                    return;
                bool masterRobeGave = false;
                for (int j = 0; j < numberOfTargets; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        if (targets[j] == &appState.stateData.gameState.playerTeam[k].stats)
                        {
                            if (targets[j] == c)
                                continue;
                            masterRobeGave = true;
                            targets[j]->baseStats.critCounter += c->baseStats.critCounter;
                        }
                    }
                }
                if(!masterRobeGave) break;
                sprintf(strnum, "%d", c->baseStats.critCounter);
                message = CombineStrings((*c).baseStats.name, " distributes ");
                message = CombineStrings(message, strnum);
                message = CombineStrings(message, " Crit Progress among affected allies.");
                c->baseStats.critCounter = 0;
                AddMessageToFeed(message);
                break;
        }
    }
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
        double logScale = 36.7879441234;
        result = (short)((float)baseDamage * log((100-effectiveDef)/logScale));
    }
    if(effectiveArmor>0) result -= effectiveArmor;

    return result;
}

void DealDamage(short damage, CreatureStats *target, bool trueDamage, CreatureStats *dealer)
{
    bool wasAlreadyDead = (*target).baseStats.currentHealth <= 0;
    short finalValue = 0;
    finalValue = (trueDamage ? (short)damage : CalculateDamage((short)damage, target));
    if(finalValue < 0)
    {
        finalValue = 0;
        AddMessageToFeed("The attack was too weak to deal any damage.");
    }
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
    HandleOnAttackEffects(target, finalValue, dealer);

    if ((*target).baseStats.currentHealth <= 0)
    {
        (*target).baseStats.currentHealth = 0;
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
            char *message;
            bool shownAltMessage = false;
            for (int i = 0; i < 3; i++)
            {
                if(&appState.stateData.gameState.stateData.battleState.enemies[i].stats != target) continue;
                if(strlen(appState.stateData.gameState.stateData.battleState.enemies[i].customDeathMessage)<2) continue;

                shownAltMessage = true;
                message = appState.stateData.gameState.stateData.battleState.enemies[i].customDeathMessage;
            }
            (*target).baseStats.currentHealth = 0;
            if(!shownAltMessage) message = CombineStrings((*target).baseStats.name, " was slain!");
            AddMessageToFeed(message);
            HandleOnDeathEffects(target);
        }
    } else {PlaySFX(SFX_Hit);}
}

float GetDamageMult(CreatureStats* c)
{
    return 1 + ((*c).itemStats.damageMultiplier + (*c).encounterStats.damageMultiplier);
}

void HandlePain(CreatureStats* c, short multihit)
{
    if(c->statusEffects[SE_PAIN] <= 0) return;
    if(c->baseStats.currentHealth <= 0) return;

    if(CalculateDamage(c->statusEffects[SE_PAIN] * multihit, c) > 0)
    {
    char* message = CombineStrings(c->baseStats.name, " winces in pain, receiving ");
    char strnum[8];
    sprintf(strnum, "%d", CalculateDamage(c->statusEffects[SE_PAIN] * multihit, c));
    message = CombineStrings(message, strnum);
    message = CombineStrings(message, " damage from their wounds.");
    AddMessageToFeed(message);
    DealDamage(c->statusEffects[SE_PAIN] * multihit, c, false, NULL);
    }
    else
    {
    char* message = CombineStrings(c->baseStats.name, " ignores the pain, receiving no damage.");
    AddMessageToFeed(message);
    }
}

float CalculateEffectAmplification(CreatureStats *caster, bool affectedByBerserk)
{
    return 1 + ((caster->baseStats.critCounter / CRIT_PROGRESS_MAX) * (((*caster).baseStats.critBonus + (*caster).itemStats.critBonus + (*caster).encounterStats.critBonus) * 0.01)) 
        + (affectedByBerserk ? caster->statusEffects[SE_BERSERK] * 0.01 : 0)
        + ((appState.stateData.gameState.stateData.battleState.opportunitySkillCountdown == 0) ? appState.stateData.gameState.stateData.battleState.opportunityMult : 0);
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
    char strnum[8];
    char *result;
    short damage = 0;
    switch (id)
    {
    case AB_WAIT:
        return "Inaction. Let the opportunity pass.";
    case AB_BERSERKER_SWING:
        sprintf(strnum, "%.0f", ((10 + GET_SCALING(caster, mastery) * 0.1)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Bring the battle axe down in a wild swing, gaining ", strnum);
        sprintf(strnum, "%.0f", ((10 + GET_SCALING(caster, mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (10 + 10% Mastery) Berserk, removing ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 40% Mastery) Defense from the target and dealing ");
        damage = ((40 + GET_SCALING(caster, mastery) * 0.8)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((40 + GET_SCALING(caster, mastery) * 0.8)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (40 + 120% Mastery) damage.");
        return result;
    case AB_BERSERKER_BASH:
        sprintf(strnum, "%.0f", MIN(9999,((GET_SCALING(caster, armor) * (caster->statusEffects[SE_BERSERK] + 1))) * CalculateEffectAmplification(caster, false)));
        result = CombineStrings("Stun an enemy with a powerful shield strike, delaying their next turn by ", strnum);
        result = CombineStrings(result, " (Armour x Berserk) moments of time.");
        return result;
    case AB_BERSERKER_BATTLECRY:
        sprintf(strnum, "%.0f", (((caster->baseStats.currentStamina) * 0.5)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Perform a mighty cry, expending half of your current Stamina, gaining ", strnum);
        result = CombineStrings(result, " (100% of expended Stamina) Berserk and Target Priority. Take an additional turn immediately after.");
        return result;
    case AB_BERSERKER_BRACE:
        sprintf(strnum, "%.0f", ((1 + (caster->statusEffects[SE_BERSERK]) * 0.05)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Raise the shield in a defensive stance, gaining ", strnum);
        result = CombineStrings(result, " (1 + 5% Berserk) Armour and ");
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (50 + 80% Mastery) Defense until next turn. Take an additional turn immediately after.");
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
        result = CombineStrings("Target ally becomes Untargetable for ", strnum);
        result = CombineStrings(result, " (300 + 100% Mastery) moments of time.");
        return result;
    case AB_ASSASSIN_REND:
        result = "Performs a brutal finisher, dealing four times the amount of Bleed points the target enemy has as unavoidable damage.";
        return result;
    case AB_DUELIST_LUNGE:
        damage = ((60 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%d", damage);
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (60 + 50% Mastery) damage and gain ");
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
        result = CombineStrings(result, " (40 + 50% Mastery) Speed and apply as much Pain to the attacker.");
        return result;
    case AB_DUELIST_BREATH:
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Regain ", strnum);
        result = CombineStrings(result, " (50 + 100% Mastery) stamina and gain double the Crit Progress. Halve all of your status effects.");
        return result;
    case AB_MONK_MEDITATE:
        sprintf(strnum, "%.0f", (10 + ((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Gain ", strnum);
        result = CombineStrings(result, " (10 + 20% Mastery) Mastery and double the Crit Progress this turn.");
        return result;
    case AB_MONK_TRUE_STRIKE:
        damage = ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, true);
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
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
        damage = ((GET_SCALING(caster, health) * 0.2 + 
        GET_SCALING(caster, mastery) * 0.3)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%d", damage);
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (20% Health + 30% Mastery) damage and apply ");
        sprintf(strnum, "%.0f", ((GET_SCALING(caster, health) - caster->baseStats.currentHealth) * 0.01 + GET_SCALING(caster, armor)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (100% Armour + 1% missing Health) Pain.");
        return result;
    case AB_FOLEM_EXPUNGE:
        damage = ((GET_SCALING(caster, health) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%d", damage);
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", ((GET_SCALING(caster, health) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (100% Health) damage to an enemy and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20% Health) damage to self.");
        return result;
    case AB_FOLEM_EPIDERMIZE:
        sprintf(strnum, "%.0f", ((10 + GET_SCALING(caster, mastery) * 0.9)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Gain ", strnum);
        sprintf(strnum, "%.0f", ((GET_SCALING(caster, health) * 0.1) +
            ((GET_SCALING(caster, health) - caster->baseStats.currentHealth) * 0.2) + 
        GET_SCALING(caster, mastery) * 1.0) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (10 + 90% Mastery) Defense and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (100% Mastery + 10% Health + 20% missing Health) Shield points.");
        return result;
    case AB_FOLEM_CRIPPLE:
        sprintf(strnum, "%.0f", (((GET_SCALING(caster, health) - caster->baseStats.currentHealth) * 0.1)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (10% missing Health) Bleed to an enemy.");
        return result;
    case AB_FOLEM_MEND:
        sprintf(strnum, "%.0f", (((caster->baseStats.currentHealth) * 0.3)));
        result = CombineStrings("Sacrifice ", strnum);
        result = CombineStrings(result, " (30% current Health) Health. Heal an ally by 60% of the Health lost and cleanse their status effects.");
        return result;
    case AB_SHAPESHIFTER_SCRATCH:
        damage = ((10 + GET_SCALING(caster, mastery) * 2.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%d", damage);
        result = CombineStrings("Scratch an enemy for ", strnum);
        result = CombineStrings(result, " (10 + 200% Mastery) damage.");
        return result;
    case AB_SHAPESHIFTER_TRANSFORM:
        return "Become an exact copy of target enemy, retaining your ability to change shapes.";
    case AB_MAGUS_DISINTEGRATE:
        damage = ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.5) + 
            (caster->baseStats.stamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_DISINTEGRATE_COST) * 0.6) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.5) + 
            (caster->baseStats.stamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_DISINTEGRATE_COST) * 0.6) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (20 + 120% Mastery + 50% missing Stamina) damage to an enemy.");
        return result;
    case AB_MAGUS_ARCANE_BLAST:
        damage = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) + 
            (caster->baseStats.stamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_ARCANE_BLAST_COST) * 0.2) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8) + 
            (caster->baseStats.stamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_ARCANE_BLAST_COST) * 0.2) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 80% Mastery + 20% missing Stamina) damage to all enemies.");
        return result;
    case AB_MAGUS_TUTOR:
        sprintf(strnum, "%.0f", (40+(caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4) * CalculateEffectAmplification(caster, false));
        result = CombineStrings("Give an ally ", strnum);
        sprintf(strnum, "%.0f", ((10+(caster->baseStats.stamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
        +MAGUS_TUTOR_COST) * 0.01)) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (40 + 40% Mastery) Mastery and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (10 + 1% missing Stamina) Crit Multiplier.");
        return result;
    case AB_MAGUS_SARCOPHAGUS:
        sprintf(strnum, "%.0f", (((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0) + 
            (caster->baseStats.stamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_SARCOPHAGUS_COST)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give an ally ", strnum);
        sprintf(strnum, "%.0f", (((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.05) + 
            (caster->baseStats.stamina + caster->encounterStats.stamina + caster->itemStats.stamina-caster->baseStats.currentStamina
            +MAGUS_SARCOPHAGUS_COST) * 0.1) * CalculateEffectAmplification(caster, false));
        result = CombineStrings(result, " (100% Mastery + 100% missing Stamina) Defense and ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (5% Mastery + 10% missing Stamina) Armor until their next turn, while delaying it by another turn.");
        return result;
    case AB_RIPPER_REND:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.15)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (10 + 15% Mastery) Bleed and Pain to an enemy.");
        return result;
    case AB_RIPPER_EVISCERATE:
        for(int i=0;i<3;i++)
        {
            DrawEnemyDamageOverlay(i, CalculateDamage( (appState.stateData.gameState.stateData.battleState.enemies[i].stats.statusEffects[SE_BERSERK] + appState.stateData.gameState.stateData.battleState.enemies[i].stats.statusEffects[SE_BLEED]
                + appState.stateData.gameState.stateData.battleState.enemies[i].stats.statusEffects[SE_EXHAUSTION] + appState.stateData.gameState.stateData.battleState.enemies[i].stats.statusEffects[SE_PAIN])
                * 2 * GetDamageMult(caster) * CalculateEffectAmplification(caster, true), &appState.stateData.gameState.stateData.battleState.enemies[i].stats));
        }
        result = "Deal damage to an enemy equal to twice the sum of negative effects they carry.";
        return result;
    case AB_RIPPER_STALK:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)));
        result = CombineStrings("Make an ally Untargetable for 500 moments and give them ", strnum);
        result = CombineStrings(result, " (20 + 30% Mastery) Crit Bonus. This ability doesn't Crit.");
        return result;
    case AB_RIPPER_CHASE:
        sprintf(strnum, "%.0f", (60 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (60 + 60% Mastery) Exhaustion to an enemy and gain Mastery equal to 20% of their missing Health.");
        return result;
    case AB_CULTIST_MADDENING_TOUCH:
        damage = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        sprintf(strnum, "%.0f", ((200 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (50 + 100% Mastery) Damage and apply Confusion for ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (100 + 120% Mastery) moments to an enemy.");
        return result;
    case AB_CULTIST_MANIA:
        sprintf(strnum, "%.0f", ((250 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 2.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give an ally ", strnum);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (250 + 200% Mastery) Stamina Regen, ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (50 + 150% Mastery) Mastery and apply Confusion for 500 moments.");
        return result;
    case AB_CULTIST_SUMMON:
        return "Create a helpful monster in an empty enemy spot.";
    case AB_CULTIST_PRAY:
        return "Re-randomize enemy intent. Sometimes might result in an additional boon (likeliness diminished by Mastery).";
    case AB_WOLF_BITE:
        damage = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Target enemy gains ", strnum);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, " (20 + 40% Mastery) Berserk each time they get hit for the next 400 moments. Deal ");
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (50 + 80% Mastery) damage to that enemy.");
        return result;
    case AB_WOLF_HUNT:
        damage = ((300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 5.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((300 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 5.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (300 + 500% Mastery) damage to an enemy, lose 50 Speed.");
        return result;
    case AB_WOLF_FERAL_AURA:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give everyone else ", strnum);
        result = CombineStrings(result, " (20 + 20% Mastery) Berserk. Gain Armour equal to 1% of everyone's Berserk.");
        return result;
    case AB_WOLF_PURSUE:
        return "Target enemy loses Speed equal to their Berserk.";
    case AB_GUIDE_RESONANT_STRIKE:
        for(int i=0;i<3;i++)
        {
            DrawEnemyDamageOverlay(i,  CalculateDamage( (10 + GET_SCALING(caster, mastery) * 0.2
                + MAX(0, GET_SCALING(caster, defense) - 
                GET_SCALING(&appState.stateData.gameState.stateData.battleState.enemies[i].stats, defense))) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true), 
                &appState.stateData.gameState.stateData.battleState.enemies[i].stats));
        }

        //primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 0.2 +
        //    MAX(0, GET_SCALING(caster, defense) - 
         //   GET_SCALING(targets[0], defense))) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
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
        result = CombineStrings(result, " (300 + 150% Mastery) moments.");
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
        damage = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 40% Mastery) damage to all allies.");
        return result;
    case AB_CULTIST_SPAWN_SPORES:
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Confuse all allies for ", strnum);
        result = CombineStrings(result, " (100 + 100% Mastery) moments.");
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
        damage = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Attack an enemy for ", strnum);
        result = CombineStrings(result, " (50 + 20% Mastery) unavoidable damage, and remove ");
        sprintf(strnum, "%.0f", ((150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (150 + 80% Mastery) of their Stamina.");
        return result;
    case AB_MIMIC_IMPALE:
        damage = ((150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((150 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (150 + 80% Mastery) damage to all enemies.");
        return result;
    case AB_MIMIC_PETRIFY:
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give an enemy ", strnum);
        result = CombineStrings(result, " (50 + 100% Mastery) Defense, and remove the same amount of Speed.");
        return result;
    case AB_BLOFAEWAR_CUT:
        sprintf(strnum, "%.0f", ((1 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Attack an enemy for ", strnum);
        result = CombineStrings(result, " (1 + 10% Mastery) damage, then apply the unmitigated damage as Bleed points.");
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
        result = CombineStrings(result, " (100 + 200% Mastery) moments.");
        return result;
    case AB_CAUCUBE_NIBBLE:
        damage = ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (30 + 60% Mastery) damage to an enemy and remove ");
        sprintf(strnum, "%.0f", ((2 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (2 + 20% Mastery) of their Mastery and Defense.");
        return result;
    case AB_CAUCUBE_SPLIT:
        return "Create a Cubeling.";
    case AB_CAUCUBE_METABOLIZE:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Gain 10 Mastery and gain ", strnum);
        result = CombineStrings(result, " (40 + 20% Mastery) Shield.");
        return result;
    case AB_TOOFAIRY_SUMMON:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Summon a Tooth Golem. If it already exists, expand its Health by ", strnum);
        result = CombineStrings(result, " (40 + 50% Mastery) points instead. If a different creature occupies that spot, deal that much unavoidable damage instead.");
        return result;
    case AB_TOOFAIRY_CALCINIZE:
        damage = ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (10 + 20% Mastery) damage to an enemy, and delay their turn by ");
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (100 + 140% Mastery) moments.");
        return result;
    case AB_TOOFAIRY_BONEDUST:
        return "Give your allies 10 Mastery and 1 Armour.";
    case AB_TOOTHOLEM_STRIKE:
        damage = ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (100 + 50% Mastery) damage and apply ");
        sprintf(strnum, "%.0f", (((caster->baseStats.defense + caster->encounterStats.defense + caster->itemStats.defense) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20% Defense) Pain.");
        return result;
    case AB_TOOTHOLEM_PROTECT:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give all allies 10 Defense and ", strnum);
        result = CombineStrings(result, " (40 + 100% Mastery) Shield.");
        return result;
    case AB_TOOTHOLEM_HARDEN:
        return "Gain 100 Defense and 3 Armour.";
    case AB_STEVENANT_SIPHON:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.2)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (10 + 20% Mastery) Exhaustion to an enemy and gain that much Speed.");
        return result;
    case AB_STEVENANT_PHASING_STRIKE:
        damage = ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (30 + 100% Mastery) damage to an enemy and become Untargetable for 200 moments.");
        return result;
    case AB_STEVENANT_ECTOPLASMIC_MANIFESTATION:
        result = "Gain 20 Mastery and Defense.";
        return result;
    case AB_BANDIT_STRONGMAN_SLAM:
        damage = ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((100 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (100 + 50% Mastery) damage.");
        return result;
    case AB_BANDIT_JESTER_MOCK:
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.4)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (30 + 40% Mastery) Berserk to an enemy.");
        return result;
    case AB_BANDIT_JESTER_BACKFLIP:
        return "Gain Untargetable for 300 moments.";
    case AB_BANDIT_JESTER_PRANK:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (20 + 60% Mastery) Pain to an enemy.");
        return result;
    case AB_BANDIT_CUTPURSE_SHIV:
        sprintf(strnum, "%.0f", ((10 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (10 + 30% Mastery) Bleed to an enemy.");
        return result;
    case AB_BANDIT_CUTPURSE_DISCOMBOBULATE:
        damage = ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((30 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.6)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (30 + 60% Mastery) damage to an enemy and apply Confusion for 300 moments.");
        return result;
    case AB_BANDIT_CUTPURSE_INVIGORATING_SMOKE:
        sprintf(strnum, "%.0f", ((20 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.3)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Give all allies ", strnum);
        result = CombineStrings(result, " (20 + 40% Mastery) Shield and 20 Speed.");
        return result;
    case AB_GORLAK_FLURRY:
        sprintf(strnum, "%d", (short)appState.stateData.gameState.floor);
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (current dungeon level) damage ");
        sprintf(strnum, "%.0f", (2+((caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (2 + 10% Mastery) times.");
        return result;
    case AB_GORLAK_CLAP:
        sprintf(strnum, "%.0f", ((5 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.1)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (5 + 10% Mastery) Pain and ");
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (50 + 50% Mastery) Confusion.");
        return result;
    case AB_GORLAK_EQUIP:
        return "Find a random piece of equipment and gain its bonuses.";
    case AB_GORLAK_POSE:
        sprintf(strnum, "%.0f", ((40 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Gain ", strnum);
        result = CombineStrings(result, " (40 + 10% Mastery) Shield and 10 Speed.");
        return result;
    case AB_ICE_ELEMENTAL_FREEZE:
        sprintf(strnum, "%.0f", ((400 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 1.0)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Delay an enemy's turn by ", strnum);
        result = CombineStrings(result, " (400 + 100% Mastery) moments.");
        return result;
    case AB_ICE_ELEMENTAL_SHARDS:
        damage = ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((50 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 50% Mastery) damage and apply ");
        sprintf(strnum, "%.0f", ((200 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.5)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (200 + 50% Mastery) Exhaustion.");
        return result;
    case AB_ICE_ELEMENTAL_AURA:
        sprintf(strnum, "%.0f", ((60 + (caster->baseStats.mastery + caster->encounterStats.mastery + caster->itemStats.mastery) * 0.8)) * CalculateEffectAmplification(caster, true));

        result = CombineStrings("All enemies lose ", strnum);
        result = CombineStrings(result, " (60 + 80% Mastery) Speed.");
        return result;
    case AB_FIRE_ELEMENTAL_FIREBALL:
        damage = ((80 + GET_SCALING(caster, mastery) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((80 + GET_SCALING(caster, mastery) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (80 + 100% Mastery) damage to an enemy.");
        return result;
    case AB_FIRE_ELEMENTAL_FLARE:
        sprintf(strnum, "%.0f", ((20 + GET_SCALING(caster, mastery) * 0.3)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Gain ", strnum);
        result = CombineStrings(result, " (20 + 30% Mastery) Speed and Mastery.");
        return result;
    case AB_FIRE_ELEMENTAL_INFERNO:
        damage = ((30 + GET_SCALING(caster, mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((30 + GET_SCALING(caster, mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (30 + 50% Mastery) damage to all enemies.");
        return result;
    case AB_MANRAT_RALLYING_ASSAULT:
        damage = ((50 + GET_SCALING(caster, mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((50 + GET_SCALING(caster, mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 50% Mastery) damage to an enemy and give all allies ");
        sprintf(strnum, "%.0f", ((20 + GET_SCALING(caster, mastery) * 0.3)) * CalculateEffectAmplification(caster, true));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (20 + 30% Mastery) Speed.");
        return result;
    case AB_MANRAT_STRUGGLE:
        damage = ((30 + GET_SCALING(caster, mastery) * 0.3)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        ShowEnemyDamageOverlays(damage, false);
        sprintf(strnum, "%.0f", ((30 + GET_SCALING(caster, mastery) * 0.3)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (30 + 30% Mastery) damage to all enemies.");
        return result;
    case AB_MANRAT_PINPOINT:
        result = "Apply Pain to an enemy equal to 5 + 5% of their Defense.";
        return result;
    case AB_FROG_FLY_STINK:
        sprintf(strnum, "%.0f", ((50 + GET_SCALING(caster, mastery) * 0.3)));
        result = CombineStrings("Apply ", strnum);
        result = CombineStrings(result, " (50 + 30\% Mastery) Exhaustion to an enemy and give them half that much Target Priority.");
        return result;
    case AB_FROG_FLY_BUZZ:
        sprintf(strnum, "%.0f", ((20 + GET_SCALING(caster, speed) * 0.1)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (20 + 10\% Speed) damage to an enemy and become Untargetable for 200 moments.");
        return result;
    case AB_FROG_DEVOUR:
        sprintf(strnum, "%.0f", ((50 + GET_SCALING(caster, mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 50\% Mastery) damage to an enemy. ");
        result = CombineStrings(result, "Then, if the sum of its health and stamina are less than 500, swallow it whole.");
        return result;
    case AB_FROG_STOMP:
        sprintf(strnum, "%.0f", ((25 + 
            (GET_SCALING(caster, mastery) * 0.25)+
            (GET_SCALING(caster, defense) * 0.10)
            )* GetDamageMult(caster) * CalculateEffectAmplification(caster, true)));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (25 + 25\% Mastery + 10\% Mastery) damage to an enemy.");
        return result;
    case AB_FROG_SING:
        sprintf(strnum, "%.0f",(20 + GET_SCALING(caster, defense) * 0.1));
        result = CombineStrings("Give every ally ", strnum);
        result = CombineStrings(result, " (20 + 10% Defense) Mastery.");
        return result;
    case AB_QPIG_ROB:
        sprintf(strnum, "%.0f",(100 + (GET_SCALING(caster, mastery) * 0.30) + (GET_SCALING(caster, speed) * 0.1))* GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (50 + 30% Mastery + 10% Speed) Damage, then permanently steal an item from their person if you haven't done so already.");
        return result;
    case AB_QPIG_TACKLE:
        sprintf(strnum, "%.0f",(150 + (GET_SCALING(caster, mastery) * 0.40))* GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        result = CombineStrings("Deal ", strnum);
        result = CombineStrings(result, " (70 + 40% Mastery) Damage and delay the target's turn by ");
        sprintf(strnum, "%d", GET_SCALING(caster, speed));
        result = CombineStrings(result, strnum);
        result = CombineStrings(result, " (100% Speed) moments.");
        return result;
    case AB_QPIG_SPRINT:
        sprintf(strnum, "%.0f",(GET_SCALING(caster, mastery) * 0.4));
        result = CombineStrings("Gain ", strnum);
        result = CombineStrings(result, " (40% Mastery) Speed.");
        return result;
        
    default:
        return "Ability description missing, oopsie!";
    }
}

void CastAbility(ABILITY id, short cost, CreatureStats *caster, CreatureStats **targets, short numberOfTargets)
{
    appState.stateData.gameState.stateData.battleState.takeAnotherTurn = false;
    char *message;
    char strnum[8];
    short primaryEffectValue;
    short multihit = 1;
    bool isCrit = (*caster).baseStats.critCounter >= CRIT_PROGRESS_MAX;
    bool dontResetCritProgress = false;
    bool skipAbility = false;

    caster->baseStats.currentStamina -= cost + ((id == AB_WAIT) ? 0 : caster->statusEffects[SE_EXHAUSTION]);

    if(numberOfTargets == 1)
    {
        if(targets[0]->statusEffects[SE_PROTECTION]>0)
        {
            skipAbility = true;
            targets[0]->statusEffects[SE_PROTECTION] -= 1;
            message = CombineStrings(caster->baseStats.name, "'s ");
            message = CombineStrings(message, InitAbility(id).name);
            message = CombineStrings(message, " was entirely negated by ");
            message = CombineStrings(message, targets[0]->baseStats.name);
            message = CombineStrings(message, "'s Protection.");
            AddMessageToFeed(message);
        }
    }

    if(skipAbility == false)
    {
    switch (id)
    {
    case AB_WAIT:
        dontResetCritProgress = true;
        AddMessageToFeed(CombineStrings(caster->baseStats.name, " does nothing."));
        break;
    case AB_BERSERKER_SWING:
        short berserkerSwingRageGain = ((10 + GET_SCALING(caster, mastery) * 0.1)) * CalculateEffectAmplification(caster, false);
        caster->statusEffects[SE_BERSERK] += berserkerSwingRageGain;
        primaryEffectValue = ((10 + GET_SCALING(caster, mastery) * 0.2)) * CalculateEffectAmplification(caster, true);
        targets[0]->encounterStats.defense -= primaryEffectValue;
        short berserkerSwingDamage =  ((40 + GET_SCALING(caster, mastery) * 0.8)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " hacks at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", shredding ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", CalculateDamage(berserkerSwingDamage, targets[0]));
        message = CombineStrings(message, " Defense, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and gaining ");
        sprintf(strnum, "%d", berserkerSwingRageGain);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Berserk.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(berserkerSwingDamage, targets[0], false, caster);
        break;
    case AB_BERSERKER_BASH:
        primaryEffectValue = MIN(9999, CalculateDamage(MIN(30000, MAX(0,(GET_SCALING(caster, armor) * (caster->statusEffects[SE_BERSERK] + 1))) * CalculateEffectAmplification(caster, false)
            * (200.0/(200 + targets[0]->baseStats.ticksUntilNextTurn))), targets[0]));
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " slams ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with a shield, delaying their turn by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " moments.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.ticksUntilNextTurn += primaryEffectValue;
        break;
    case AB_BERSERKER_BATTLECRY:
        primaryEffectValue = (((caster->baseStats.currentStamina) * 0.5)) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = ((50 + GET_SCALING(caster, mastery) * 0.8)) * CalculateEffectAmplification(caster, false);
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
        message = CombineStrings(message, " moments, making them Untargetable.");
        targets[0]->statusEffects[SE_UNTARGETABLE] = primaryEffectValue;
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
        primaryEffectValue = ((60 + GET_SCALING(caster, mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        short duelistLungeSpeed = ((20 + GET_SCALING(caster, mastery) * 0.2)) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = ((100 + GET_SCALING(caster, mastery) * 0.8
            + GET_SCALING(caster, speed) * 0.1)) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = ((5 + GET_SCALING(caster, speed) * 0.05)) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = ((50 + GET_SCALING(caster, mastery) * 1.0)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        caster->baseStats.currentStamina += primaryEffectValue;
        (*caster).baseStats.critCounter += GET_SCALING(caster, critRate);
        message = CombineStrings((*caster).baseStats.name, " takes a steady breath, regaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina and greatly increasing Crit Progress. Their status ailments are partially cleansed.");
        for(int i = 0; i < SE_LENGTH; i++)
        {
            caster->statusEffects[i] /= 2;
        }
        AddMessageToFeed(message);
        break;
    case AB_MONK_MEDITATE:
        dontResetCritProgress = true;
        (*caster).baseStats.critCounter += GET_SCALING(caster, critRate);
        primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 0.2) * CalculateEffectAmplification(caster, false);
        (*caster).encounterStats.mastery += primaryEffectValue;
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " meditates, gaining ");
        message = CombineStrings(message, strnum);
        sprintf(strnum, "%d", GET_SCALING(caster, mastery));
        message = CombineStrings(message, " Mastery, for a total of ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, ".");
        AddMessageToFeed(message);
        break;
    case AB_MONK_TRUE_STRIKE:
        primaryEffectValue = (100 + GET_SCALING(caster, mastery) * 2.0) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (150 + GET_SCALING(caster, mastery) * 2.0) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = ((GET_SCALING(caster, health) * 0.2 
            + GET_SCALING(caster, mastery) * 0.3)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short folemStrikePain = ((GET_SCALING(caster, health) - caster->baseStats.currentHealth) * 0.01 + GET_SCALING(caster, armor)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " slams ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with a mass of chaotic flesh, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and applying ");
        sprintf(strnum, "%d",  folemStrikePain);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Pain.");
        AddCreatureToFlicker(targets[0]);
        AddMessageToFeed(message);
        // >:3
        targets[0]->statusEffects[SE_PAIN] += folemStrikePain;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_FOLEM_EXPUNGE:
        if ((appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask & (1 << 1)) == false)
        {
            appState.stateData.gameState.stateData.battleState.fleshGolemSkillMask += 2;
        }
        primaryEffectValue = ((GET_SCALING(caster, health) * 1.0)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        short folemExpungeValue = ((GET_SCALING(caster, health) * 0.2)) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = ((10 + GET_SCALING(caster, mastery) * 0.9)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        short folemEpidermizeShield = ((GET_SCALING(caster, health) * 0.1) +
            ((GET_SCALING(caster, health) - caster->baseStats.currentHealth) * 0.2) + 
        GET_SCALING(caster, mastery) * 1.0) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = (((GET_SCALING(caster, health) - caster->baseStats.currentHealth) * 0.1)) * CalculateEffectAmplification(caster, true);
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
        caster->baseStats.currentHealth -= primaryEffectValue;
        targets[0]->baseStats.currentHealth += (short)(primaryEffectValue * 0.6);
        if(targets[0]->baseStats.currentHealth > GET_SCALING(targets[0], health))
        {
            targets[0]->baseStats.currentHealth = GET_SCALING(targets[0], health);
        }
        EmptyStatusEffects(targets[0]);
    break;
    case AB_SHAPESHIFTER_SCRATCH:
        primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 2.0) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
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
        if(strcmp(targets[0]->baseStats.name, "Empty Space") == 0)
        {
        message = CombineStrings((*caster).baseStats.name, " becomes an incidental little critter found on the dungeon floor.");
        }
        else if(appState.stateData.gameState.floor%8 == 0 || targets[0]->baseStats.tileset == TL_BIG_CREATURES || targets[0]->baseStats.tileset == TL_BIG_COMMUNITY)
        {
            AddMessageToFeed("Despite best efforts, the Shapeshifter fails to assume that form.");
            AddCreatureToFlicker(caster);
            break;
        }
        else
        {
        message = CombineStrings((*caster).baseStats.name, " becomes ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ".");
        }
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
        caster->baseStats.color = beige;
        caster->abilities = InitAbilities(abilities, abCount);
        caster->abilityCount = abCount;
        if(targets[0]->creatureEffect != LE_NONE)
        {
            ApplyLingeringEffect(caster,(LingeringEffect){targets[0]->creatureEffect,0,0,0,true});
        }
        if(strcmp(targets[0]->baseStats.name, "Empty Space") == 0)
        {
            caster->baseStats.health = 1;
            caster->baseStats.currentHealth = 1;
            caster->baseStats.name = "Critter";
            switch(rand()%4)
            {
                case 0: caster->baseStats.tileLookupPosition = (Vector2){8,9}; caster->baseStats.tileset = TL_CREATURES; break;
                case 1: caster->baseStats.tileLookupPosition = (Vector2){10,9}; caster->baseStats.tileset = TL_CREATURES; break;
                case 2: caster->baseStats.tileLookupPosition = (Vector2){0,10}; caster->baseStats.tileset = TL_CREATURES; break;
                case 3: caster->baseStats.tileLookupPosition = (Vector2){1,1}; caster->baseStats.tileset = TL_COMMUNITY; break;
            }
        }
        AddCreatureToFlicker(caster);
        AddCreatureToFlicker(targets[0]);
        break;
    case AB_MAGUS_DISINTEGRATE:
        primaryEffectValue = ((100 + GET_SCALING(caster, mastery) * 2.5) + 
            (GET_SCALING(caster, stamina)-caster->baseStats.currentStamina) * 0.6) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " shoots a ray of concentrated arcane energy at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_MAGUS_ARCANE_BLAST:
        primaryEffectValue = ((50 + GET_SCALING(caster, mastery) * 0.8) + 
            (GET_SCALING(caster, stamina)-caster->baseStats.currentStamina) * 0.2) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = ((40 + GET_SCALING(caster, mastery) * 0.4)) * CalculateEffectAmplification(caster, false);
        sprintf(strnum, "%d", primaryEffectValue);
        short magusTutorCM = ((10+(GET_SCALING(caster, stamina)-caster->baseStats.currentStamina) * 0.01)) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = ((GET_SCALING(caster, mastery) * 1.0) + 
            (GET_SCALING(caster, stamina)-caster->baseStats.currentStamina)) * CalculateEffectAmplification(caster, true);
        short magusSarcophagusArmor = ((GET_SCALING(caster, mastery) * 0.05) + 
            (GET_SCALING(caster, stamina)-caster->baseStats.currentStamina) * 0.1) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 0.15) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue, targets[0]);
        message = CombineStrings((*caster).baseStats.name, " hacks at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Bleed and Pain.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_BLEED] += primaryEffectValue;
        targets[0]->statusEffects[SE_PAIN] += primaryEffectValue;
        DealDamage(0, targets[0], true, caster);
        if(targets[0]->baseStats.currentHealth <= 0) 
        {
            appState.stateData.gameState.stateData.battleState.takeAnotherTurn = true;
            dontResetCritProgress = true;
        }
        break;
    case AB_RIPPER_EVISCERATE:
        primaryEffectValue = (2 * (targets[0]->statusEffects[SE_BLEED] + targets[0]->statusEffects[SE_BERSERK] + targets[0]->statusEffects[SE_EXHAUSTION] 
            + targets[0]->statusEffects[SE_PAIN])) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " tears ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " apart from the inside, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " unavoidable damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        if(targets[0]->baseStats.currentHealth <= 0) 
        {
            appState.stateData.gameState.stateData.battleState.takeAnotherTurn = true;
            dontResetCritProgress = true;
        }
        break;
    case AB_RIPPER_STALK:
        primaryEffectValue = (20 + GET_SCALING(caster, mastery) * 0.3) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " conceals ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", making them Untargetable for 500 moments and giving them ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, "% Crit Bonus.");
        targets[0]->statusEffects[SE_UNTARGETABLE] = 500;
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
        primaryEffectValue = (60 + GET_SCALING(caster, mastery) * 0.6) * CalculateEffectAmplification(caster, true);
        short ripperChaseMastery = ((GET_SCALING(targets[0], health) - targets[0]->baseStats.currentHealth) * 0.2);
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
        if(targets[0]->baseStats.currentHealth <= 0) appState.stateData.gameState.stateData.battleState.takeAnotherTurn = true;
    break;
    case AB_CULTIST_MADDENING_TOUCH:
        primaryEffectValue = (50 + GET_SCALING(caster, mastery) * 1.0) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short cultistMadTouchConfusion = (200 + GET_SCALING(caster, mastery) * 1.2) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (250 + GET_SCALING(caster, mastery) * 2.0) * CalculateEffectAmplification(caster, true);
        short cultistManiaMastery = (100 + GET_SCALING(caster, mastery) * 1.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue, targets[0]);
        message = CombineStrings((*caster).baseStats.name, " imbues ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with eldritch euphoria, granting ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina Regen, ");
        sprintf(strnum, "%d", cultistManiaMastery);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Mastery and inflicts Confusion for 500 moments.");
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
            Enemy cultistAbortedSummon = InitEnemyData(EN_CULTIST_SUMMON);
            char cultistSummonAbCount = targets[0]->abilityCount + 1;
            ABILITY *cultistSummonAbilities = malloc(cultistSummonAbCount * sizeof(int));
            for (int i = 0; i < targets[0]->abilityCount - 1; i++)
            {
                cultistSummonAbilities[i] = targets[0]->abilities[i].abilityId;
            }
            cultistSummonAbilities[cultistSummonAbCount - 2] = cultistAbortedSummon.stats.abilities[rand()%cultistAbortedSummon.stats.abilityCount-1].abilityId;
            cultistSummonAbilities[cultistSummonAbCount - 1] = AB_WAIT;
            targets[0]->abilities = InitAbilities(cultistSummonAbilities, cultistSummonAbCount);
            targets[0]->abilityCount = cultistSummonAbCount;
            targets[0]->baseStats.color = darkpurple;
            message = CombineStrings("As ", caster->baseStats.name);
            message = CombineStrings(message, "'s summoning ritual fails, ");
            message = CombineStrings(message, targets[0]->baseStats.name);
            message = CombineStrings(message, " is altered by divine influence, becoming more... fungal.");
            AddMessageToFeed(message);
        }
        else
        {
            for(int i = 0; i < 3; i++)
            {
                if(&appState.stateData.gameState.stateData.battleState.enemies[i].stats != targets[0]) continue;

                appState.stateData.gameState.stateData.battleState.enemies[i] = InitEnemyData(EN_CULTIST_SUMMON);
                appState.stateData.gameState.stateData.battleState.enemies[i].stats.baseStats.mastery = 
                    GET_SCALING(caster, mastery) * CalculateEffectAmplification(caster, true);
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
        if(rng_next_u32(&appState.stateData.gameState.stateData.battleState.battleRng) % (450 + GET_SCALING(caster, mastery)*4 
            + appState.stateData.gameState.stateData.battleState.prayerFatigue * 15) < 400 * CalculateEffectAmplification(caster, true))
        {
            CreatureStats* target;
            switch(rand() % 15)
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
                appState.stateData.gameState.playerTeam[0].stats.baseStats.color = green;
                appState.stateData.gameState.playerTeam[1].stats.baseStats.color = darkpurple;
                appState.stateData.gameState.playerTeam[2].stats.baseStats.color = red;
                break;
                case 8:
                AddMessageToFeed("The Mindless One grants everyone 20 Mastery.");
                appState.stateData.gameState.playerTeam[0].stats.encounterStats.mastery += 20;
                appState.stateData.gameState.playerTeam[1].stats.encounterStats.mastery += 20;
                appState.stateData.gameState.playerTeam[2].stats.encounterStats.mastery += 20;
                appState.stateData.gameState.stateData.battleState.enemies[0].stats.encounterStats.mastery += 20;
                appState.stateData.gameState.stateData.battleState.enemies[1].stats.encounterStats.mastery += 20;
                appState.stateData.gameState.stateData.battleState.enemies[2].stats.encounterStats.mastery += 20;
                break;
                case 9:
                appState.stateData.gameState.stateData.battleState.takeAnotherTurn = true;
                message = CombineStrings("The Mindless One yanks ", caster->baseStats.name);
                message = CombineStrings(message, " outside the flow of time, granting them an additional turn.");
                AddMessageToFeed(message);
                break;
                case 10:
                target = &appState.stateData.gameState.playerTeam[rand()%3].stats;
                dontResetCritProgress = true;
                EmptyStatusEffects(target);
                message = CombineStrings("The Mindless One cures ", target->baseStats.name);
                message = CombineStrings(message, " of all ailments.");
                AddMessageToFeed(message);
                break;
                case 11:
                AddMessageToFeed("The Mindless One elevates the battleground. You've seen those surroundings before.");
                appState.stateData.gameState.floor -= 1;
                if(appState.stateData.gameState.floor < 1) appState.stateData.gameState.floor = 1;
                break;
                case 12:
                target = &appState.stateData.gameState.playerTeam[rand()%3].stats;
                (*target).itemStats.health += 100;
                message = CombineStrings("The Mindless One makes ", target->baseStats.name);
                message = CombineStrings(message, " a little fatter.");
                AddMessageToFeed(message);
                break;
                case 13:
                target = &appState.stateData.gameState.playerTeam[rand()%3].stats;
                if((*target).baseStats.currentHealth > 0) break;
                (*target).baseStats.currentHealth = 200;
                (*target).baseStats.color = orange;
                (*target).encounterStats.speed -= 50;
                message = CombineStrings("The Mindless One raises ", target->baseStats.name);
                message = CombineStrings(message, " from the dead. Something's off...");
                AddMessageToFeed(message);
                break;
                default:
                AddMessageToFeed("The Mindless One laughs jubilantly, the otherworldly voice echoing across the cave system.");
                break;
            }

            if(appState.stateData.gameState.stateData.battleState.enemies[1].enemyId == EN_GORLAK)
            {
                if(rand()%10<4)
                {
                    AddMessageToFeed("The Mindless One bargains with the Gorlak, asking for mercy. The Gorlak smirks at your adventuring party, walking away while leaving a little gift.");
                    appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.currentHealth = 0;
                }
            }
        }
        break;
    case AB_WOLF_BITE:
        primaryEffectValue = (50 + GET_SCALING(caster, mastery) * 0.8) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short wolfBiteBerserk = (20 + GET_SCALING(caster, mastery) * 0.4) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " bites ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and aggravating the wound.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        LingeringEffect wolfBiteEffect = (LingeringEffect){LE_ONHIT_WOLF_BITE, 400, 0, wolfBiteBerserk};
        ApplyLingeringEffect(targets[0], wolfBiteEffect);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_WOLF_HUNT:
        primaryEffectValue = (300 + GET_SCALING(caster, mastery) * 5.0) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
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
        break;
    case AB_WOLF_FERAL_AURA:
        primaryEffectValue = (20 + GET_SCALING(caster, mastery) * 0.2) * CalculateEffectAmplification(caster, true);
        short wolfAuraSummaryBerserk = 0;
        for (int i = 0; i < numberOfTargets; i++)
        {
            if(targets[i]->baseStats.currentHealth <= 0) continue;
            wolfAuraSummaryBerserk += targets[i]->statusEffects[SE_BERSERK];
            if(targets[i] == caster) continue;
            AddCreatureToFlicker(targets[i]);
            targets[i]->statusEffects[SE_BERSERK] += primaryEffectValue;
        }
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " stands ominously still, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Berserk to everyone else and gaining ");
        sprintf(strnum, "%d", (short)(wolfAuraSummaryBerserk * 0.01));
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Armour.");
        AddMessageToFeed(message);
        caster->encounterStats.armor += (short)(wolfAuraSummaryBerserk * 0.01);
        break;
    case AB_WOLF_PURSUE:
        primaryEffectValue = targets[0]->statusEffects[SE_BERSERK];
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " steadily paces behind ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", removing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed in their overexertion.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->encounterStats.speed -= primaryEffectValue;
        break;
    case AB_GUIDE_RESONANT_STRIKE:
        primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 0.2 +
            MAX(0, GET_SCALING(caster, defense) - 
            GET_SCALING(targets[0], defense))) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short guideResonantStrikeShield = (50 + GET_SCALING(caster, mastery) * 0.8) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (40 + GET_SCALING(caster, mastery) * 0.8) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (300 + GET_SCALING(caster, mastery) * 1.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " creates an aggravating dissonance near ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", making them the  centre of attention for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " moments.");
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
        primaryEffectValue = (100 + GET_SCALING(caster, mastery) * 0.6) * CalculateEffectAmplification(caster, true);
        short guideDancePain = (30 + GET_SCALING(caster, mastery) * 1.0) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (50 + GET_SCALING(caster, mastery) * 0.4) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (100 + GET_SCALING(caster, mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " explodes with its spores, confusing its allies for ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " moments.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            if(targets[i] == caster) continue;
            AddCreatureToFlicker(targets[i]);
            targets[i]->statusEffects[SE_CONFUSION] = primaryEffectValue;
        }
        break;
    case AB_CULTIST_SPAWN_GROW:
        primaryEffectValue = ((40 + GET_SCALING(caster, mastery) * 0.8)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        caster->encounterStats.speed += primaryEffectValue;
        message = CombineStrings((*caster).baseStats.name, " rapidly grows in complexity, giving itself ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        break;
        case AB_CULTIST_SPAWN_INVIGORATE:
        primaryEffectValue = (40 + GET_SCALING(caster, mastery) * 0.4) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = (50 + GET_SCALING(caster, mastery) * 0.2) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short mimicChompStaminaLoss = (150 + GET_SCALING(caster, mastery) * 0.8) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " bites down on ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " unavoidable damage. ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " expends ");
        sprintf(strnum, "%d", mimicChompStaminaLoss);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Stamina in their struggle to get free.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.currentStamina -= mimicChompStaminaLoss;
        DealDamage(primaryEffectValue, targets[0], true, caster);
        break;
    case AB_MIMIC_IMPALE:
        primaryEffectValue = (150 + GET_SCALING(caster, mastery) * 0.8) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (50 + GET_SCALING(caster, mastery) * 1.0) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = CalculateDamage(((1 + GET_SCALING(caster, mastery) * 0.1)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true), targets[0]);
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
        primaryEffectValue = (30 + GET_SCALING(caster, mastery) * 0.3) * CalculateEffectAmplification(caster, false);
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
        primaryEffectValue = ((100 + GET_SCALING(caster, mastery) * 2.0)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " giggles at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", delaying their turn by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " moments out of shyness.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.ticksUntilNextTurn += primaryEffectValue;
        break;
    case AB_CAUCUBE_NIBBLE:
        primaryEffectValue = (30 + GET_SCALING(caster, mastery) * 0.6) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short caucubeNibbleMasteryLoss = (2 + GET_SCALING(caster, mastery) * 0.2) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " covers ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with digestive oozes, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and degrading their equipment, lowering their Mastery and Defense by ");
        sprintf(strnum, "%d", caucubeNibbleMasteryLoss);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, ".");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->encounterStats.mastery -= caucubeNibbleMasteryLoss;
        targets[0]->encounterStats.defense -= caucubeNibbleMasteryLoss;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_CAUCUBE_SPLIT:
        bool hasSummonedCubeling;
        for(int i = 0; i < 3; i++)
            {
                if(appState.stateData.gameState.stateData.battleState.enemies[i].stats.baseStats.currentHealth > 0) continue;

                appState.stateData.gameState.stateData.battleState.enemies[i] = InitEnemyData(EN_CAUSTIC_CUBE_CLONE);
                message = CombineStrings((*caster).baseStats.name, " spawns a Cubeling next to itself.");
                AddCreatureToFlicker(&appState.stateData.gameState.stateData.battleState.enemies[i].stats);
                AddMessageToFeed(message);
                hasSummonedCubeling = true;
                break;
            }
            if(!hasSummonedCubeling)
            {
                message = CombineStrings((*caster).baseStats.name, " attempted to produce a cubeling but there's no more room.");
                AddMessageToFeed(message);
            }
        break;
    case AB_CAUCUBE_METABOLIZE:
        primaryEffectValue = (40 + GET_SCALING(caster, mastery) * 0.2) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " makes its acid more potent and voluminous, gaining 10 Mastery and ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Shield.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->encounterStats.shield += primaryEffectValue;
        caster->encounterStats.mastery += 10;
        break;
    case AB_TOOFAIRY_SUMMON:
        primaryEffectValue = (40 + GET_SCALING(caster, mastery) * 0.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        if(appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.currentHealth > 0)
        {
            if(strcmp( appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.name, "Tooth Golem") == 0)
            {
                message = CombineStrings((*caster).baseStats.name, " nurtures the dental abomination, expanding its Health by ");
                message = CombineStrings(message, strnum);
                message = CombineStrings(message, ".");
                AddMessageToFeed(message);
                AddCreatureToFlicker(&appState.stateData.gameState.stateData.battleState.enemies[1].stats);
                appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.health += primaryEffectValue;
                appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.currentHealth += primaryEffectValue;
            }
            else
            {
                message = CombineStrings((*caster).baseStats.name, " causes a sudden spurt of growing teeth inside ");
                message = CombineStrings(message, appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.name);
                message = CombineStrings(message, "'s body, dealing ");
                message = CombineStrings(message, strnum);
                message = CombineStrings(message, " unavoidable internal damage.");
                AddMessageToFeed(message);
                AddCreatureToFlicker(&appState.stateData.gameState.stateData.battleState.enemies[1].stats);
                DealDamage(primaryEffectValue, &appState.stateData.gameState.stateData.battleState.enemies[1].stats,true, caster);
            }
        }
        else
        {
            message = CombineStrings((*caster).baseStats.name, " creates an animate golem out of teeth.");
            AddMessageToFeed(message);
            appState.stateData.gameState.stateData.battleState.enemies[1] = InitEnemyData(EN_TOOTH_GOLEM);
        }
        break;
    case AB_TOOFAIRY_CALCINIZE:
        primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 0.2) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short toothfairyCalcinizeDelay = (100 + GET_SCALING(caster, mastery) * 1.4) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " calcifies ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, "'s bones, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and delaying their turn by ");
        sprintf(strnum, "%d", toothfairyCalcinizeDelay);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " moments.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.ticksUntilNextTurn += toothfairyCalcinizeDelay;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
        case AB_TOOFAIRY_BONEDUST:
            primaryEffectValue = 10 * CalculateEffectAmplification(caster, true);
            sprintf(strnum, "%d", primaryEffectValue);
            for (int i = 0; i < numberOfTargets; i++)
            {
                targets[i]->encounterStats.mastery += primaryEffectValue;
                targets[i]->encounterStats.armor += 1;
                AddCreatureToFlicker(targets[i]);
            }
            message = CombineStrings((*caster).baseStats.name, " sprinkles bone dust over its allies, granting ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Mastery and 1 Armour to each.");
            AddMessageToFeed(message);
            break;
        case AB_TOOTHOLEM_STRIKE:
            primaryEffectValue = (100 + GET_SCALING(caster, mastery) * 0.5) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
            short toothgolemStrikePain = (GET_SCALING(caster, defense) * 0.2) * CalculateEffectAmplification(caster, true);
            sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
            message = CombineStrings((*caster).baseStats.name, " slams its jagged appendage into ");
            message = CombineStrings(message, targets[0]->baseStats.name);
            message = CombineStrings(message, ", dealing ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " damage and applying ");
            sprintf(strnum, "%d", toothgolemStrikePain);
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Pain.");
            AddMessageToFeed(message);
            AddCreatureToFlicker(targets[0]);
            targets[0]->statusEffects[SE_PAIN] += toothgolemStrikePain;
            DealDamage(primaryEffectValue, targets[0], false, caster);
            break;
        case AB_TOOTHOLEM_PROTECT:
            primaryEffectValue = (40 + GET_SCALING(caster, mastery) * 1.0) * CalculateEffectAmplification(caster, true);
            sprintf(strnum, "%d", primaryEffectValue);
            for (int i = 0; i < numberOfTargets; i++)
            {
                targets[i]->encounterStats.defense += 10;
                targets[i]->encounterStats.shield += primaryEffectValue;
                AddCreatureToFlicker(targets[i]);
            }
            message = CombineStrings((*caster).baseStats.name, " creates a protective barrier from excess bone mass, granting 10 Defense and ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " Shield to each.");
            AddMessageToFeed(message);
            break;
        case AB_TOOTHOLEM_HARDEN:
        primaryEffectValue = 100 * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " reinforces its exterior, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Defense and 3 Armour.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->encounterStats.defense += primaryEffectValue;
        caster->encounterStats.armor += 3;
        break;
    case AB_STEVENANT_SIPHON:
        primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 0.2) * CalculateEffectAmplification(caster, true);
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
        primaryEffectValue = (30 + GET_SCALING(caster, mastery) * 1.0) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " passes through ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with malice, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and becoming Untargetable for 200 moments.");
        AddMessageToFeed(message);
        caster->statusEffects[SE_UNTARGETABLE] = 200;
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_STEVENANT_ECTOPLASMIC_MANIFESTATION:
        primaryEffectValue = 20 * CalculateEffectAmplification(caster, true);
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
    case AB_BANDIT_STRONGMAN_SLAM:
        primaryEffectValue = (100 + GET_SCALING(caster, mastery) * 0.5) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " strikes at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " with all their might, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_BANDIT_JESTER_MOCK:
        primaryEffectValue = (30 + GET_SCALING(caster, mastery) * 0.4) * CalculateEffectAmplification(caster, true);
        bool jesterMockMimic = false;
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " points at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " and says:\n");
        if(strcmp(targets[0]->baseStats.name, "Berserker") == 0)
        {
            switch(rand()%3)
            {
                case 0:
                message = CombineStrings(message, "\"Getting angry? Is that your special power?\"");
                break;
                case 1:
                message = CombineStrings(message, "\"Do they know what happened to your previous party?\"");
                break;
                case 2:
                message = CombineStrings(message, "\"You belong here.\" Cackles.");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Duelist") == 0)
        {
            switch(rand()%3)
            {
                case 0:
                message = CombineStrings(message, "\"You were handed the good life on a platter and you threw it all away just to chase adventure? And I thought I am the clown here.\"");
                break;
                case 1:
                message = CombineStrings(message, "\"What could you possibly be seeking here? Riches? Meaning?\" Snickers.");
                break;
                case 2:
                message = CombineStrings(message, "\"Feeling free yet?\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Monk") == 0)
        {
            switch(rand()%5)
            {
                case 0:
                message = CombineStrings(message, "\"Your facade is crumbling.\"");
                break;
                case 1:
                message = CombineStrings(message, "\"Ran out of breath already?\"");
                break;
                case 2:
                message = CombineStrings(message, "\"Something to hide?\"");
                break;
                case 3:
                message = CombineStrings(message, "\"How little you truly know...\"");
                break;
                case 4:
                message = CombineStrings(message, "\"Once a monster, always a monster.\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Shapeshifter") == 0)
        {
            switch(rand()%2)
            {
                case 0:
                message = CombineStrings(message, "\"Oh, did you forget how to change forms? Or are you being a burden on purpose?\"");
                break;
                case 1:
                message = CombineStrings(message, "\"Yes, you go, scratch us to death!\" Cackles.");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Flesh Golem") == 0)
        {
            switch(rand()%2)
            {
                case 0:
                message = CombineStrings(message, "\"How do I even insult a walking pile of viscera?\"");
                break;
                case 1:
                message = CombineStrings(message, "\"So are you the party's meal or...?\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Magus") == 0)
        {
            switch(rand()%4)
            {
                case 0:
                message = CombineStrings(message, "\"I've heard of the scrawny sorcerer stereotype, but you're really overdoing it.\"");
                break;
                case 1:
                message = CombineStrings(message, "\"Reduced just to your bones, huh? Weren't you supposed to be the smart one?\"");
                break;
                case 2:
                message = CombineStrings(message, "\"Yes, he's disappointed, I will save you the trip.\"");
                break;
                case 3:
                message = CombineStrings(message, "\"Think you're ready to face him again? You idiot.\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Ripper") == 0)
        {
            switch(rand()%4)
            {
                case 0:
                message = CombineStrings(message, "\"It's what's inside that matters, right?\" Giggles.");
                break;
                case 1:
                message = CombineStrings(message, "\"What exactly do you need the amulet for? To find yourself someone who won't run away screaming?\"");
                break;
                case 2:
                message = CombineStrings(message, "\"Aww look at you! Being... tolerated.\"");
                break;
                case 3:
                message = CombineStrings(message, "\"We both know they will betray you the moment you stop being useful.\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Cultist") == 0)
        {
            switch(rand()%3)
            {
                case 0:
                message = CombineStrings(message, "\"Worshipping a mushroom, huh? The innuendo writes itself.\"");
                break;
                case 1:
                message = CombineStrings(message, "\"My God can beat up your God.\" Snickers.");
                break;
                case 2:
                message = CombineStrings(message, "\"Blind idiot lead by a blind idiot.\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Wolf") == 0)
        {
            switch(rand()%4)
            {
                case 0:
                message = CombineStrings(message, "\"Who let the dogs out?\"");
                break;
                case 1:
                message = CombineStrings(message, "\"You will never see them again.\"");
                break;
                case 2:
                message = CombineStrings(message, "\"You don't belong. You'll never belong.\"");
                break;
                case 3:
                message = CombineStrings(message, "\"How does it feel to rely on your lesser? How does it feel to be mistaken for an equal?\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Guide") == 0)
        {
            switch(rand()%3)
            {
                case 0:
                message = CombineStrings(message, "\"Your saviour complex is nothing short of pathetic.\"");
                break;
                case 1:
                message = CombineStrings(message, "\"New lambs to a slaughter...\"");
                break;
                case 2:
                message = CombineStrings(message, "\"Are you ready to face their judgement? You really didn't think this through.\"");
                break;
            }
        }
        else if(strcmp(targets[0]->baseStats.name, "Mimic") == 0)
        {
                message = CombineStrings(message, "\"Why are you idiots hauling a whole treasure chest with you?\"");
                jesterMockMimic = true;
        }
        else if(strcmp(targets[0]->baseStats.name, "Strongman") == 0)
        {
                message = CombineStrings(message, "\"Of all the things you could choose to be...\"");
        }
        else if(strcmp(targets[0]->baseStats.name, "Cutpurse") == 0)
        {
                message = CombineStrings(message, "\"Oi Herc, I didn't know you had an ugly twin.\"");
        }
        else if(strcmp(targets[0]->baseStats.name, "Jester") == 0)
        {
                message = CombineStrings(message, "\"That's not funny!\"");
        }
        else if(strcmp(targets[0]->baseStats.name, "Gorlak") == 0)
        {
                message = CombineStrings(message, "\"G-Gorlak?!\"\n");
                message = CombineStrings(message, caster->baseStats.name);
                message = CombineStrings(message, " leaves the battle hastily.");
                caster->baseStats.currentHealth = 0;
        }
        else
        {
            message = CombineStrings(message, "\"What the hell even is that?\"");
        }
        message = CombineStrings(message, "\n");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " is enraged and gains ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Berserk.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        if(jesterMockMimic)
        {
            message = CombineStrings((*caster).baseStats.name, " approaches the Mimic chest and tries to open it. The Mimic accepts the easy meal.");
            AddMessageToFeed(message);
            DealDamage(1000, caster, true,targets[0]);
        }
        targets[0]->statusEffects[SE_BERSERK] += primaryEffectValue;
        break;
    case AB_BANDIT_JESTER_BACKFLIP:
        message = CombineStrings((*caster).baseStats.name, " performs a spontaeous backflip and yells \"Jester's immunity!\", gaining 300 moments of Untargettability in the process.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->statusEffects[SE_UNTARGETABLE] = 300;
        break;
    case AB_BANDIT_JESTER_PRANK:
        primaryEffectValue = (20 + GET_SCALING(caster, mastery) * 0.6) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " pulls a mean practical joke on ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", causing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Pain.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_PAIN] += primaryEffectValue;
        break;
    case AB_BANDIT_CUTPURSE_SHIV:
        primaryEffectValue = (10 + GET_SCALING(caster, mastery) * 0.3) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " cuts ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Bleed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_BLEED] += primaryEffectValue;
        break;
    case AB_BANDIT_CUTPURSE_DISCOMBOBULATE:
        primaryEffectValue = (30 + GET_SCALING(caster, mastery) * 0.6) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " elbows ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " to the head, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and applying 300 Confusion.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_CONFUSION] = 300;
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_BANDIT_CUTPURSE_INVIGORATING_SMOKE:
        primaryEffectValue = (20 + GET_SCALING(caster, mastery) * 0.3) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " throws a magical smoke grenade under their feet, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Shield to all allies and granting them 20 Speed.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            targets[i]->encounterStats.shield += primaryEffectValue;
            targets[i]->encounterStats.speed += 20;
        }
        break;
    case AB_GORLAK_FLURRY:
        primaryEffectValue = appState.stateData.gameState.floor * GetDamageMult(caster);
        multihit = (2 + GET_SCALING(caster, mastery) * 0.1) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " performs a flurry of stabs against ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage ");
        sprintf(strnum, "%d", multihit);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " times.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        for(int i = 0; i < multihit; i++)
        {
            DealDamage(primaryEffectValue, targets[0], false, caster);
        }
        break;
    case AB_GORLAK_CLAP:
        primaryEffectValue = (5 + GET_SCALING(caster, mastery) * 0.1) * CalculateEffectAmplification(caster, true);
        short gorlakClapConfusion = (50 + GET_SCALING(caster, mastery) * 0.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " performs a mighty, incredibly loud clap, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Pain and ");
        sprintf(strnum, "%d", gorlakClapConfusion);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Confusion to the opposing team.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            targets[i]->statusEffects[SE_PAIN] += primaryEffectValue;
            targets[i]->statusEffects[SE_CONFUSION] = gorlakClapConfusion;
        }
        break;
    case AB_GORLAK_EQUIP:
        Item gorlakItem = InitItem(itemPoolTier1[rand() % ITEM_POOL_TIER_ONE_SIZE]);
        message = CombineStrings((*caster).baseStats.name, " finds ");
        message = CombineStrings(message, gorlakItem.name);
        message = CombineStrings(message, " in their backpack and puts it on despite already being overdressed. They now seem stronger than before.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->encounterStats.armor += gorlakItem.statBonuses.armor;
        caster->encounterStats.critBonus += gorlakItem.statBonuses.critBonus;
        caster->encounterStats.critRate += gorlakItem.statBonuses.critRate;
        caster->encounterStats.health += gorlakItem.statBonuses.health;
        caster->encounterStats.stamina += gorlakItem.statBonuses.stamina;
        caster->encounterStats.staminaRegen += gorlakItem.statBonuses.staminaRegen;
        caster->encounterStats.speed += gorlakItem.statBonuses.speed;
        caster->encounterStats.defense += gorlakItem.statBonuses.defense;
        caster->encounterStats.mastery += gorlakItem.statBonuses.mastery;
        caster->encounterStats.targetPriority += gorlakItem.statBonuses.targetPriority;
        break;
    case AB_GORLAK_POSE:
        primaryEffectValue = (40 + GET_SCALING(caster, mastery) * 1.0) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " strikes an awe-inspiring pose, shining with renewed luster, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Shield and 10 Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->encounterStats.shield += primaryEffectValue;
        caster->encounterStats.speed += 10;
        switch(rand()%4)
        {
            case 0: caster->baseStats.color = magenta; break;
            case 1: caster->baseStats.color = raywhite; break;
            case 2: caster->baseStats.color = skyblue; break;
            case 3: caster->baseStats.color = gold; break;
        }
        break;
    case AB_ICE_ELEMENTAL_FREEZE:
        primaryEffectValue = ((400 + GET_SCALING(caster, mastery) * 1.0)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " freezes ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", delaying their turn by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " moments.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->baseStats.ticksUntilNextTurn += primaryEffectValue;
        break;
    case AB_ICE_ELEMENTAL_SHARDS:
        primaryEffectValue = (50 + GET_SCALING(caster, mastery) * 0.5) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        short iceElemShardsExhaustion = (200 + GET_SCALING(caster, mastery) * 0.5) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " shoots shards of ice at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and applying ");
        sprintf(strnum, "%d", iceElemShardsExhaustion);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Exhaustion.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_EXHAUSTION] += iceElemShardsExhaustion;
        DealDamage(primaryEffectValue, targets[0], true, caster);
        break;
    case AB_ICE_ELEMENTAL_AURA:
        primaryEffectValue = ((60 + GET_SCALING(caster, mastery) * 0.8)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " creates a chilling aura that engulfs the room, reducing the Speed of the opposing team by ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, ".");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            targets[i]->encounterStats.speed -= primaryEffectValue;
        }
        break;
    case AB_FIRE_ELEMENTAL_FIREBALL:
        primaryEffectValue = (80 + GET_SCALING(caster, mastery) * 1.0) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " launches a ball of fire at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        break;
    case AB_FIRE_ELEMENTAL_FLARE:
        primaryEffectValue = (20 + GET_SCALING(caster, mastery) * 0.3) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " flares up, gaining ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Mastery and Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->encounterStats.speed += primaryEffectValue;
        caster->encounterStats.mastery += primaryEffectValue;
        break;
    case AB_FIRE_ELEMENTAL_INFERNO:
        primaryEffectValue = (30 + GET_SCALING(caster, mastery) * 0.5) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " explodes into an inferno, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage to all enemies.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break;
    case AB_MANRAT_RALLYING_ASSAULT:
        primaryEffectValue = ((50 + GET_SCALING(caster, mastery) * 0.5)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        short rallyBoost = (((20 + GET_SCALING(caster, mastery) * 0.3)) * CalculateEffectAmplification(caster, true));
        bool isEnemy = true;
        message = CombineStrings((*caster).baseStats.name, " lets out a courageous cry and jabs forward towards ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and granting its team ");
        sprintf(strnum, "%d", rallyBoost);
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        DealDamage(primaryEffectValue, targets[0], false, caster);
        for (int i = 0; i < 3; i++)
        {
            if (caster == &appState.stateData.gameState.playerTeam[i].stats)
            {
                isEnemy = false;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            if (isEnemy == false)
            {
                appState.stateData.gameState.playerTeam[i].stats.encounterStats.speed+=rallyBoost;
            }
            else
            {
                appState.stateData.gameState.stateData.battleState.enemies[i].stats.encounterStats.speed+=rallyBoost;
            }
        }
        break;
    case AB_MANRAT_STRUGGLE:
        primaryEffectValue = ((30 + GET_SCALING(caster, mastery) * 0.3)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " lashes out desperately, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage to all enemies.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break;
    case AB_MANRAT_PINPOINT:
        primaryEffectValue = ((5 + GET_SCALING(targets[0], defense) * 0.05)) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " strikes at the weak points in ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, "'s armor, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Pain.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_PAIN]+=primaryEffectValue;
        break;
    case AB_FROG_FLY_STINK:
        primaryEffectValue = (50 + GET_SCALING(caster, mastery) * 0.3);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " emits a most foul odour, applying ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Exhaustion to ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " and raises their target priority by half that much.");
        AddMessageToFeed(message);

        AddCreatureToFlicker(targets[0]);
        targets[0]->statusEffects[SE_EXHAUSTION] += primaryEffectValue;
        targets[0]->baseStats.targetPriority += primaryEffectValue*0.5;
        break;
    case AB_FROG_FLY_BUZZ:
        primaryEffectValue = ((20 + GET_SCALING(caster, speed) * 0.1) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " frantically flies around, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage to ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " and becoming untargetable for 200 moments.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        caster->statusEffects[SE_UNTARGETABLE] = 200;
        break; 
    case AB_FROG_STOMP:
        primaryEffectValue = ((25 + 
            (GET_SCALING(caster, mastery) * 0.25)+
            (GET_SCALING(caster, defense) * 0.10)
            ) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " launches its full body weight at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, ", dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break; 
    case AB_FROG_DEVOUR:
        primaryEffectValue = ((50 + (GET_SCALING(caster, mastery) * 0.50)) * GetDamageMult(caster) * CalculateEffectAmplification(caster, true));
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " fires its tongue at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " at a blinding speed, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
            int healthcheck = targets[i]->baseStats.currentHealth;
            for (int tmbr = 0; tmbr < 3; tmbr++) {
                if (&appState.stateData.gameState.playerTeam[tmbr].stats == targets[0]) {
                    healthcheck += targets[0]->baseStats.currentStamina;
                }
            }

            if (healthcheck <= 500) {
            
            if (targets[0] != caster) {
                message = CombineStrings("Having sufficiently weakened its prey, ",(*caster).baseStats.name);
                message = CombineStrings(message, " swallows ");
                message = CombineStrings(message, targets[0]->baseStats.name);
                message = CombineStrings(message, " whole, never to be seen again.");
            } else {
                message = CombineStrings("In a curious error, ",(*caster).baseStats.name);
                message = CombineStrings(message, " disapears into its own mouth, never to be seen again.");
            }

            AddMessageToFeed(message);
                //replacement effect
                Enemy example_of_nothing = InitEnemyData(EN_NONE);
                targets[i]->baseStats = example_of_nothing.stats.baseStats;
            }
        }
        break; 
    case AB_FROG_SING:
        primaryEffectValue = (20 + (GET_SCALING(caster, defense) * 0.10));
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " croaks out a powerful song, granting each of its allies ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Mastery.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            targets[i]->encounterStats.mastery += primaryEffectValue;
        }
        break;
    case AB_QPIG_SPRINT:
        primaryEffectValue = (GET_SCALING(caster, mastery) * 0.4);
        sprintf(strnum, "%d", primaryEffectValue);
        message = CombineStrings((*caster).baseStats.name, " picks up the pace and gains ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " Speed.");
        AddMessageToFeed(message);
        AddCreatureToFlicker(caster);
        caster->encounterStats.speed += primaryEffectValue;
        break;
    case AB_QPIG_TACKLE:
        primaryEffectValue = (150 + (GET_SCALING(caster, mastery) * 0.40))* GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        message = CombineStrings((*caster).baseStats.name, " dashes at ");
        message = CombineStrings(message, targets[0]->baseStats.name);
        message = CombineStrings(message, " and tackles them, dealing ");
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " damage and delaying their turn by ");
        sprintf(strnum, "%d", GET_SCALING(caster, speed));
        message = CombineStrings(message, strnum);
        message = CombineStrings(message, " moments.");
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break; 
    case AB_QPIG_ROB:
        primaryEffectValue = (100 + (GET_SCALING(caster, mastery) * 0.30) + (GET_SCALING(caster, speed) * 0.1))* GetDamageMult(caster) * CalculateEffectAmplification(caster, true);
        sprintf(strnum, "%d", CalculateDamage(primaryEffectValue, targets[0]));
        bool stealing_item = caster->statusEffects[SE_BAGGED] == 0;
        if (stealing_item) {
            short theft_options = 0;
            int theft_slots[4];
            int theft_victim_index = -1;
            
            // find the target's index
            for (int i = 0; i < 3; i++) {
                if (&appState.stateData.gameState.playerTeam[i].stats == targets[0]) {
                    theft_victim_index = i;
                }
            }
            // find that target's gear slots
            if (theft_victim_index >= 0) {
                for (int i = 0; i < 4; i++) {
                    if (appState.stateData.gameState.playerTeam[theft_victim_index].items[i].itemId != ITEM_NONE){
                        theft_options +=1;
                        theft_slots[theft_options-1] = i;
                    }
                }
            }
            // if nothing to steal
            if (theft_options == 0) {
                message = CombineStrings((*caster).baseStats.name, " swiftly strikes at ");
                message = CombineStrings(message, targets[0]->baseStats.name);
                message = CombineStrings(message, ", dealing ");
                message = CombineStrings(message, strnum);
                message = CombineStrings(message, " damage, but fails to steal anything.");
            } else {
                // Choose an item slot
                int stolen_item_slot = theft_slots[rand() % theft_options];

                message = CombineStrings((*caster).baseStats.name, " swiftly strikes at ");
                message = CombineStrings(message, targets[0]->baseStats.name);
                message = CombineStrings(message, ", dealing ");
                message = CombineStrings(message, strnum);
                message = CombineStrings(message, " damage, stealing their ");
                message = CombineStrings(message, appState.stateData.gameState.playerTeam[theft_victim_index].items[stolen_item_slot].name);
                message = CombineStrings(message, " right off their person! The item is completely ruined in the process.");
                
                // steal the item for realsies
                
                // remove lingering effect
                Item removedItemExample = InitItem(appState.stateData.gameState.playerTeam[theft_victim_index].items[stolen_item_slot].itemId);
                for (int i = 0; i < LINGERING_EFFECTS; i++)
                { 
                    if (removedItemExample.triggerAbility == targets[0]->lingeringEffects[i].effectId) {
                        targets[0]->lingeringEffects[i].effectId = LE_NONE;
                        targets[0]->lingeringEffects[i].triggerLimit = 0;
                        targets[0]->lingeringEffects[i].tickDuration = 0;
                        break;
                    }
                }
                // remove stats and the actual item
                appState.stateData.gameState.playerTeam[theft_victim_index].items[stolen_item_slot].itemId = ITEM_NONE;
                UnequipItem(&appState.stateData.gameState.playerTeam[theft_victim_index], stolen_item_slot);
                caster->statusEffects[SE_BAGGED] = 1;
            }
        } else {
            message = CombineStrings((*caster).baseStats.name, " swiftly strikes at ");
            message = CombineStrings(message, targets[0]->baseStats.name);
            message = CombineStrings(message, ", dealing ");
            message = CombineStrings(message, strnum);
            message = CombineStrings(message, " damage, opting not to steal any more items.");
        }
        AddMessageToFeed(message);
        for (int i = 0; i < numberOfTargets; i++)
        {
            AddCreatureToFlicker(targets[i]);
            DealDamage(primaryEffectValue, targets[i], false, caster);
        }
        break; 
    
    default:
        message = CombineStrings((*caster).baseStats.name, " uses an ability that wasn't implemented yet, how embarassing!");
        AddMessageToFeed(message);
        break;
    }
    }

    for(int i = 0; i<numberOfTargets;i++)
    {
        HandlePain(targets[i], multihit);
    }

    HandleOnAbilityEffects(caster, id,targets,numberOfTargets);

    if (!dontResetCritProgress)
    {
        (*caster).baseStats.critCounter = (*caster).baseStats.critCounter % CRIT_PROGRESS_MAX;
    }
    (*caster).baseStats.critCounter += GET_SCALING(caster, critRate);

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
