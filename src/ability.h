#ifndef ability_h
#define ability_h
#include "raylib.h"

#define MAGUS_DISINTEGRATE_COST 300
#define MAGUS_ARCANE_BLAST_COST 350
#define MAGUS_TUTOR_COST 300
#define MAGUS_SARCOPHAGUS_COST 50

typedef enum ABILITY
{
    AB_WAIT,
    AB_BERSERKER_SWING,
    AB_BERSERKER_BASH,
    AB_BERSERKER_BATTLECRY,
    AB_BERSERKER_BRACE,
    AB_ASSASSIN_SLASH,
    AB_ASSASSIN_CONCEAL,
    AB_ASSASSIN_PREPARE,
    AB_ASSASSIN_REND,
    AB_DUELIST_LUNGE,
    AB_DUELIST_OPPORTUNITY,
    AB_DUELIST_PARRY,
    AB_DUELIST_BREATH,
    AB_MONK_MEDITATE,
    AB_MONK_TRUE_STRIKE,
    AB_MONK_ATTUNEMENT,
    AB_MONK_CLEANSE,
    AB_FOLEM_STRIKE,
    AB_FOLEM_EPIDERMIZE,
    AB_FOLEM_EXPUNGE,
    AB_FOLEM_CRIPPLE,
    AB_SHAPESHIFTER_SCRATCH,
    AB_SHAPESHIFTER_TRANSFORM,
    AB_MAGUS_DISINTEGRATE,      // stupidly strong single target attack
    AB_MAGUS_ARCANE_BLAST,      // aoe attack
    AB_MAGUS_TUTOR,                 // give ally mastery and stamina
    AB_MAGUS_SARCOPHAGUS,   // give an ally a massive boost of defense but considerably delay their next turn
    AB_MIMIC_PETRIFY,
    AB_MIMIC_CHOMP,
    AB_MIMIC_IMPALE,
    AB_BLOFAEWAR_CUT,
    AB_BLOFAEMYS_INSPIRE,
    AB_BLOFAEMYS_HASTE,
    AB_BLOFAEMYS_MOCK,
    AB_CAUCUBE_NIBBLE,
    AB_CAUCUBE_SPLIT,
    AB_CAUCUBE_METABOLIZE,
    AB_TOOFAIRY_SUMMON,
    AB_TOOFAIRY_CALCINIZE,
    AB_TOOFAIRY_BONEDUST,
    AB_TOOTHOLEM_STRIKE,
    AB_TOOTHOLEM_HARDEN,
    AB_TOOTHOLEM_PROTECT,
    AB_STEVENANT_SIPHON,
    AB_STEVENANT_PHASING_STRIKE,
    AB_STEVENANT_ECTOPLASMIC_MANIFESTATION,
} ABILITY;

enum ABILITY_FLAGS
{
    AF_PASSIVE = 1,             // prevents ability from appearing in ability choice
    AF_TARGETS_SELF = 2,        // skips user input to autotarget
    AF_TARGETS_ALLIES = 4,
    AF_TARGETS_ENEMIES = 8,
    AF_AOE = 16,
};

typedef struct Ability
{
    ABILITY abilityId;
    char* name;
    short staminaCost;
    char abilityFlags;
} Ability;

Ability InitAbility(ABILITY id);
bool DoesAbilityHaveFlag(Ability ab, enum ABILITY_FLAGS flag);


#endif