#include <stdlib.h>
#include "enemy.h"
#include "dungeon.h"

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )


ENCOUNTER_ID encounterPoolTier1[8];
ENCOUNTER_ID encounterPoolTier2[4];
ENCOUNTER_ID encounterPoolTier3[4];

ITEM_ID itemPoolTier1[16];
ITEM_ID itemPoolTier2[8];
ITEM_ID itemPoolTier3[8];


ENCOUNTER_ID* SelectRandomEncounters(char floor)
{
    ENCOUNTER_ID* result = (ENCOUNTER_ID*)malloc(3 * sizeof(ENCOUNTER_ID));

    for(int i = 0; i < 3; i++)
    {
        result[i] = encounterPoolTier1[rand() % ARR_SIZE(encounterPoolTier1)];
    }

    return result;
}

ITEM_ID MatchRewardToEncounter(ENCOUNTER_ID enc, char floor)
{
    ITEM_ID result = ITEM_NONE;
    switch(floor)
    {
        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        {
            switch(enc)
            {
                case ENC_BONFIRE: case ENC_NONE:
                result = ITEM_NONE;
                break;

                default:
                result  = itemPoolTier1[rand() % ARR_SIZE(itemPoolTier1)];
                break;
            }
            break;
        }
    }
    return result;
}

void InitEncounterPools()
{
    encounterPoolTier1[0] = ENC_BLOOD_FAERIES;
    encounterPoolTier1[1] = ENC_MIMIC;
    encounterPoolTier1[2] = ENC_CHEST;
    encounterPoolTier1[3] = ENC_BLOOD_FAERIES;
    encounterPoolTier1[4] = ENC_BLOOD_FAERIES;
    encounterPoolTier1[5] = ENC_BLOOD_FAERIES;
    encounterPoolTier1[6] = ENC_BLOOD_FAERIES;
    encounterPoolTier1[7] = ENC_BLOOD_FAERIES;
}

void InitItemPools()
{
    itemPoolTier1[0] = ITEM_APPRENTICE_BOOTS;
    itemPoolTier1[1] = ITEM_APPRENTICE_ROBE;
    itemPoolTier1[2] = ITEM_APPRENTICE_STAFF;
    itemPoolTier1[3] = ITEM_APPRENTICE_TIARA;
    itemPoolTier1[4] = ITEM_JUGGERNAUT_ARMOR;
    itemPoolTier1[5] = ITEM_JUGGERNAUT_GREATHAMMER;
    itemPoolTier1[6] = ITEM_JUGGERNAUT_HELM;
    itemPoolTier1[7] = ITEM_JUGGERNAUT_LEGGINS;
    itemPoolTier1[8] = ITEM_MERCENARY_AXE;
    itemPoolTier1[9] = ITEM_MERCENARY_CUIRAS;
    itemPoolTier1[10] = ITEM_MERCENARY_GREAVES;
    itemPoolTier1[11] = ITEM_MERCENARY_HELMET;
    itemPoolTier1[12] = ITEM_THIEF_CAPE;
    itemPoolTier1[13] = ITEM_THIEF_DAGGER;
    itemPoolTier1[14] = ITEM_THIEF_HOOD;
    itemPoolTier1[15] = ITEM_THIEF_PANTS;
}

Encounter GetEncounterData(ENCOUNTER_ID id)
{
    switch(id)
    {
        case ENC_BONFIRE:
            return (Encounter)
            {
                "Bonfire",
                TL_GENERAL,
                (Vector2){1, 1},
                ORANGE,
                "A safe resting spot to regenerate one's strength.",
                {EN_NONE, EN_NONE, EN_NONE},
                false,
            };
        case ENC_CHEST:
            return (Encounter)
            {
                "Chest",
                TL_GENERAL,
                (Vector2){10, 1},
                BROWN,
                "A tightly locked treasure chest. Whatever is inside must be valuable.",
                {EN_NONE, EN_NONE, EN_NONE},
                true,
            };
        case ENC_MIMIC:
            return (Encounter)
            {
                "Chest",
                TL_GENERAL,
                (Vector2){10, 1},
                BROWN,
                "A tightly locked treasure chest. Whatever is inside must be valuable.",
                {EN_NONE, EN_MIMIC, EN_NONE},
                true,
            };
        case ENC_BLOOD_FAERIES:
            return (Encounter)
            {
                "Blood Fae",
                TL_CREATURES,
                (Vector2){12, 3},
                RED,
                "A group of quick and tiny pests. While their scratches are barely enough to draw blood, the tiny wounds add up.",
                {EN_BLOOD_FAE_WARRIOR, EN_BLOOD_FAE_MYSTIC, EN_BLOOD_FAE_WARRIOR},
                false,
            };
        default:
            return (Encounter)
            {
                "No encounter",
                TL_GENERAL,
                (Vector2){1, 0},
                DARKGRAY,
                "This path is safe to traverse, neither traps nor monsters lurk there.",
                {EN_NONE, EN_NONE, EN_NONE},
                false,
            };
    }
}