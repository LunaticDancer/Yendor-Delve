#ifndef ability
#define ability
#include "raylib.h"

typedef enum ABILITY
{
    AB_WAIT,
    AB_MONK_MEDITATE,
    AB_MONK_TRUE_STRIKE,
    AB_MONK_ATTUNEMENT,
    AB_MONK_CLEANSE,
    AB_SHAPESHIFTER_SCRATCH,
    AB_SHAPESHIFTER_TRANSFORM,
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