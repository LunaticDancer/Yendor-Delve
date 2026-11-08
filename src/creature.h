#ifndef creature_h
#define creature_h

#include <math.h>
#include "constants.h"
#include "raylib.h"
#include "ability.h"

#define GET_SCALING(creature, stat) ((*creature).baseStats.stat + (*creature).itemStats.stat  + (*creature).encounterStats.stat)

typedef enum STATUS_EFFECT 
{
    SE_PROTECTION,      // become entirely immune to single target abilities for X instances
    SE_BLEED,           // deal X damage at start of turn
    SE_BERSERK,         // remove X Def, add X% damage output
    SE_EXHAUSTION,      // remove X speed, abilities cost X more stamina
    SE_UNTARGETABLE,    // can't be targetted by enemy abilities for X ticks
    SE_PAIN,            // deal X damage each time the creature becomes a target of ability
    SE_CONFUSION,       // confused creatures cast spells at random targets
    SE_BAGGED,          // Quickpig uses this to track whether or not it has stolen an item
    SE_LENGTH,
} STATUS_EFFECT;

typedef enum LINGERING_EFFECT
{
    LE_NONE,
    LE_AMULET_OF_YENDOR,

    LE_ONABIL_WANDERER_HAT,
    LE_ONABIL_COMFORTABLE_SHOES,
    LE_ONABIL_LIVING_CHAINMAIL,
    LE_ONABIL_ASSASSIN_SHOES,
    LE_ONABIL_SCRYER_BALL,
    LE_ONABIL_ANTLER_HELMET,
    LE_ONABIL_MASTER_ROBE,

    LE_ONATT_HAMMER_OF_GLORY,
    LE_ONATT_BALL_ON_CHAIN,
    LE_ONATT_MORNINGSTAR,
    LE_ONATT_MALEVOLENCE,

    LE_ONDTH_VENGEFUL_SPIRIT,
    LE_ONDTH_JAR_BREAK_GOODIES,
    LE_ONDTH_MANRAT,

    LE_ONENC_COIN,
    LE_ONENC_ILLUSION_PROTECT,

    LE_ONHIT_DUELIST_PARRY,
    LE_ONHIT_WOLF_BITE,
    LE_ONHIT_PENITENT_MAIL,
    LE_ONHIT_CRYSTAL_MAIL,
    LE_ONHIT_FOOT_FUNGUS,

    LE_ONTURN_AMBROSIA,
} LINGERING_EFFECT;

// diminishing stat influence formula: 100 / (100 + stat)

typedef struct CreatureBaseStats
{
    char* name;
    enum TILESET tileset;
    Vector2 tileLookupPosition;
    Color *color;

    short ticksUntilNextTurn;
    short currentHealth;
    short health;
    short currentStamina;
    short stamina;
    short staminaRegen;               // stamina gained per 1000 ticks of turn timer
    short speed;                      // diminishing formula, at speed 0 the wait time is 1000 ticks
    short defense;                      // diminishing formula
    short armor;                        // flat damage reduction
    float critBonus;             // damage amplification percent on top of baseline 100%
    short critRate;                     // crit can exceed 100%, making it proc mustiple times, applying the multiplier each time
    short critCounter;                  // crit occurence accumulation, persists between battles
    short mastery;                      // ability scaling
    float damageMultiplier;     // dummy stat for macro, do not use
    short targetPriority;
} CreatureBaseStats;

typedef struct StatBonuses
{
    short health;
    short stamina;
    short staminaRegen;
    short speed;
    short defense;
    short armor;
    float critBonus;
    short critRate;
    short mastery;
    float damageMultiplier;
    short targetPriority;
    short shield;
} StatBonuses;

typedef struct StatDebuff
{
    short tickDuration;
    StatBonuses debuff;
} StatDebuff;

typedef struct LingeringEffect
{
    LINGERING_EFFECT effectId;
    short tickDuration;
    short triggerLimit;
    short storedValue;
    bool isPermanent;
} LingeringEffect;

typedef struct CreatureStats
{
    CreatureBaseStats baseStats;
    int statusEffects[SE_LENGTH];       // stores intensity values for each status effect
    StatBonuses encounterStats;         // resets after each encounter
    StatBonuses itemStats;
    StatDebuff temporaryStats[STAT_DEBUFFS];
    LingeringEffect lingeringEffects[LINGERING_EFFECTS];
    LINGERING_EFFECT creatureEffect;
    Ability* abilities;
    char abilityCount;
} CreatureStats;

StatBonuses CreateEmptyStatBonuses();
void EmptyStatDebuffs(CreatureStats*);
void ApplyStatDebuff(CreatureStats*, StatDebuff);
void EmptyLingeringEffects(CreatureStats*);
void ApplyLingeringEffect(CreatureStats*, LingeringEffect);
void ProgressLingeringEffects(CreatureStats*, short);
void ShowLingeringEffectPassedMessage(CreatureStats*, LINGERING_EFFECT);
char* GetLingeringEffectDescription(LINGERING_EFFECT);
void HandleOnHitEffects(CreatureStats*, short, CreatureStats*);
void HandleOnAttackEffects(CreatureStats*, short, CreatureStats*);
void HandleStartOfTurnEffects(CreatureStats*);
void HandleStartOfCombatEffects(CreatureStats*);
void HandleOnDeathEffects(CreatureStats*);
void HandleOnAbilityEffects(CreatureStats*, ABILITY,CreatureStats **targets, short numberOfTargets);
void EmptyStatusEffects(CreatureStats*);
void ResetTurnClock(CreatureStats*);
short CalculateNextTurnTicks(CreatureStats* _creature);
short CalculateDamage(short baseDamage, CreatureStats* target);
float GetDamageMult(CreatureStats* c);
void DealDamage(short damage, CreatureStats* target, bool trueDamage, CreatureStats* dealer);
void HandlePain(CreatureStats*,short);
float CalculateEffectAmplification(CreatureStats*, bool);
char* GetAbilityDescription(ABILITY id, CreatureStats* caster);
Ability* InitAbilities(ABILITY abilities[], short count);
void CastAbility(ABILITY id, short cost, CreatureStats* caster, CreatureStats** targets, short numberOfTargets);

#endif
