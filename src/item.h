#ifndef item
#define item

#include "raylib.h"
#include "constants.h"
#include "creature.h"

extern enum TILESET;
extern struct StatBonuses;

typedef enum EQUIPMENT_SLOT
{
    ES_EVERY,
    ES_HEAD,
    ES_TORSO,
    ES_LEGS,
    ES_WEAPON,
} EQUIPMENT_SLOT;

typedef enum ITEM_ID
{
    ITEM_NONE,
} ITEM_ID;

typedef struct Item
{
    ITEM_ID itemId;
    char* name;
    enum TILESET tileset;
    Vector2 tileLookupPosition;
    Color color;
    
    EQUIPMENT_SLOT slot;
    StatBonuses statBonuses;
} Item;

Item InitItem(ITEM_ID);

#endif