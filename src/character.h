#ifndef character
#define character

#include "creature.h"
#include "item.h"
#include "ability.h"

typedef enum CHARACTER_ID
{
    CHAR_BERSERKER,     // benefits from the Berserk status
    CHAR_ASSASSIN,      // crit amplification and bleed, untargetability
    CHAR_DUELIST,       // speed amplification
    CHAR_MONK,          // high stamina costs, great mastery scaling
    CHAR_FLESH_GOLEM,   // starts off weak but gets stronger with each combat encounter
} CHARACTER_ID;

typedef struct Character
{
    CHARACTER_ID characterId;
    char* description;
    CreatureStats stats;
    Item items[4];
    StatBonuses itemStats;
    short carryCapacity;            // contribution to shared inventory slots
} Character;

Character InitCharacterData(CHARACTER_ID);

#endif