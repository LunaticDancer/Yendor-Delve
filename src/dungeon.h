#ifndef dungeon
#define dungeon

#include "enemy.h"

typedef enum ENCOUNTER_ID
{
    ENC_NONE,
    ENC_BONFIRE,
    ENC_CHEST,
    ENC_MIMIC,
} ENCOUNTER_ID;

typedef struct Encounter
{
    char* name;
    enum TILESET tileset;
    Vector2 tileLookupPosition;
    Color color;
    char* description;
    ENEMY_ID enemies[3];
    bool hidesLoot;
} Encounter;

Encounter GetEncounterData(ENCOUNTER_ID);

#endif