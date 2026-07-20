#ifndef item
#define item

#include "raylib.h"
#include "constants.h"
#include "creature.h"

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
    ITEM_TEST,
    ITEM_MERCENARY_HELMET,
    ITEM_MERCENARY_CUIRAS,
    ITEM_MERCENARY_GREAVES,
    ITEM_MERCENARY_AXE,
    ITEM_THIEF_HOOD,
    ITEM_THIEF_CAPE,
    ITEM_THIEF_PANTS,
    ITEM_THIEF_DAGGER,
    ITEM_APPRENTICE_TIARA,
    ITEM_APPRENTICE_ROBE,
    ITEM_APPRENTICE_BOOTS,
    ITEM_APPRENTICE_STAFF,
    ITEM_JUGGERNAUT_HELM,
    ITEM_JUGGERNAUT_ARMOR,
    ITEM_JUGGERNAUT_LEGGINS,
    ITEM_JUGGERNAUT_GREATHAMMER,
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

char* GetItemStatSpread(Item it);
Item InitItem(ITEM_ID);

#endif