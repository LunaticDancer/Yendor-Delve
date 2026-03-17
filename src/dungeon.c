#include "enemy.h"
#include "dungeon.h"

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
                {EN_MIMIC, EN_NONE, EN_NONE},
                true,
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