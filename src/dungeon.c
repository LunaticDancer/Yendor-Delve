#include <stdlib.h>
#include "enemy.h"
#include "dungeon.h"

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )


ENCOUNTER_ID encounterPoolTier1[4];
ENCOUNTER_ID encounterPoolTier2[4];
ENCOUNTER_ID encounterPoolTier3[4];


ENCOUNTER_ID* SelectRandomEncounters(char floor)
{
    ENCOUNTER_ID* result = (ENCOUNTER_ID*)malloc(3 * sizeof(ENCOUNTER_ID));

    for(int i = 0; i < 3; i++)
    {
        result[i] = encounterPoolTier1[rand() % ARR_SIZE(encounterPoolTier1)];
    }

    return result;
}

void InitEncounterPools()
{
    encounterPoolTier1[0] = ENC_BLOOD_FAERIES;
    encounterPoolTier1[1] = ENC_MIMIC;
    encounterPoolTier1[2] = ENC_CHEST;
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