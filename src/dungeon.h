#ifndef dungeon
#define dungeon

#include "enemy.h"
#include "item.h"

typedef enum ENCOUNTER_ID
{
    ENC_NONE,
    ENC_BONFIRE,
    ENC_CHEST,
    ENC_MIMIC,
    ENC_BLOOD_FAERIES,
} ENCOUNTER_ID;

extern ENCOUNTER_ID encounterPoolTier1[];
extern ENCOUNTER_ID encounterPoolTier2[];
extern ENCOUNTER_ID encounterPoolTier3[];

typedef struct Encounter
{
    ENCOUNTER_ID id;
    char* name;
    enum TILESET tileset;
    Vector2 tileLookupPosition;
    Color color;
    char* description;
    ENEMY_ID enemies[3];
    bool hidesLoot;
} Encounter;

ENCOUNTER_ID* SelectRandomEncounters(char floor);
Encounter GetEncounterData(ENCOUNTER_ID);
void InitEncounterPools();
ITEM_ID MatchRewardToEncounter(ENCOUNTER_ID, char);
void InitItemPools();

#endif