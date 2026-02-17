#ifndef ability
#define ability

typedef enum ABILITY
{
    AB_WAIT,
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
    short staminaCost;
    short cooldown;
    short cooldownTimer;
    char abilityFlags;
} Ability;



#endif