#ifndef creature
#define creature

#include "constants.h"
#include "raylib.h"
#include "ability.h"

typedef enum STATUS_EFFECT
{
    SE_BLEED,           // deal X damage at start of turn
    SE_BERSERK,         // remove X Def, add X% damage output
    SE_EXHAUSTION,      // remove X speed, abilities cost X more stamina
    SE_UNTARGETTABLE,   // can't be targetted by enemy abilities for X ticks
    SE_LENGTH,
} STATUS_EFFECT;

// diminishing stat influence formula: 100 / (100 + stat)

typedef struct CreatureBaseStats
{
    char* name;
    enum TILESET tileset;
    Vector2 tileLookupPosition;
    Color color;

    short ticksUntilNextTurn;
    short currentHealth;
    short maxHealth;
    short currentStamina;
    short maxStamina;
    short staminaRegen;               // stamina gained per 1000 ticks of turn timer
    short speed;                      // diminishing formula, at speed 0 the wait time is 1000 ticks
    short defense;                      // diminishing formula
    short armor;                        // flat damage reduction
    float critMultiplier;             // damage amplification percent on top of baseline 100%
    short critRate;                     // crit can exceed 100%, making it proc mustiple times, applying the multiplier each time
    short critCounter;                  // crit occurence accumulation, persists between battles
    short mastery;                      // ability scaling
} CreatureBaseStats;

typedef struct StatBonuses
{
    short health;
    short stamina;
    short speed;
    short defense;
    short armor;
    float critMultiplier;
    short critRate;
    short mastery;
    float damageMultiplier;
} StatBonuses;

typedef struct StatDebuff
{
    short tickDuration;
    StatBonuses debuff;
} StatDebuff;

typedef struct CreatureStats
{
    CreatureBaseStats baseStats;
    int statusEffects[SE_LENGTH];       // stores intensity values for each status effect
    StatBonuses encounterStats;         // resets after each encounter
    Ability* abilities;
    char abilityCount;
} CreatureStats;

StatBonuses* CreateEmptyStatBonuses();

#endif