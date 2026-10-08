#ifndef character_h
#define character_h

#include "creature.h"
#include "item.h"
#include "ability.h"

typedef enum CHARACTER_ID
{
    CHAR_BERSERKER,     // benefits from the Berserk status
    CHAR_DUELIST,       // speed amplification
    CHAR_MONK,          // high stamina costs, great mastery scaling
    CHAR_SHAPESHIFTER,  // can turn into enemies
    CHAR_FLESH_GOLEM,   // starts off weak but gets stronger with each combat encounter
    CHAR_MAGUS,         // one HP but uniquely uses stamina as a shield
    CHAR_RIPPER,            // takes a turn immediately when something dies, status effect merchant
    CHAR_CULTIST,       // summons friendly monsters, can gamble
    CHAR_WOLF,          // applies Berserk,  slows enemies
    CHAR_GUIDE,      // aoe buffs, tanking
    CHAR_LENGTH,
} CHARACTER_ID;

typedef struct Character
{
    CHARACTER_ID characterId;
    char* description;
    CreatureStats stats;
    Item items[ITEM_SLOTS]; 
} Character;

Character InitCharacterData(CHARACTER_ID);
void EquipItem(Character* ch, ITEM_ID it, char slot);
void UnequipItem(Character* ch, char slot);
char* GetCharacterStatsRundown(Character ch);

#endif