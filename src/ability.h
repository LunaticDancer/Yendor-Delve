#ifndef ability
#define ability
#include "raylib.h"

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
    AB_BLOFAEWAR_CUT,
    AB_BLOFAEMYS_INSPIRE,
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