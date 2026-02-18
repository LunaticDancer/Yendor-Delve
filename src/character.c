#include "character.h"
#include <stdlib.h>

Character InitCharacterData(CHARACTER_ID id)
{
    switch (id)
    {
        case CHAR_BERSERKER:
        return (Character){
            id,
            "As reckless as he is effective, the Berserker dominates his opponents \nthrough sheer force and rage, endangering himself beyond reason just to \nchase the thrill of murder.",
            (CreatureStats){
                (CreatureBaseStats){
                    "Berserker",
                    TL_CREATURES,
                    (Vector2){6, 0},
                    RED,

                    0,
                    1400,
                    1400,
                    1000,
                    1000,
                    150,
                    50,
                    10,
                    1,
                    100,
                    5,
                    0,
                    0,
                },
                {0,0,0,0},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
            CreateEmptyStatBonuses(),
            6,      // carry
        };
        break;
        case CHAR_ASSASSIN:
        return (Character){
            id,
            "Mysterious and elusive, the Assassin is a frail but tactical fighter, waiting \nfor the right opportunity to deal a decisive strike.",
            (CreatureStats){
                (CreatureBaseStats){
                    "Assassin",
                    TL_CREATURES,
                    (Vector2){1, 1},
                    GRAY,

                    0,
                    600,
                    600,
                    1200,
                    1200,
                    100,
                    80,
                    20,
                    0,
                    125,
                    25,
                    0,
                    0,
                },
                {0,0,0,0},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
            CreateEmptyStatBonuses(),
            3,      // carry
        };
        break;
        case CHAR_DUELIST:
        return (Character){
            id,
            "Speed and precission is what the Duelist is known for. \nThis highly trained fighter becomes more deadly the longer the fight lasts.",
            (CreatureStats){
                (CreatureBaseStats){
                    "Duelist",
                    TL_CREATURES,
                    (Vector2){7, 0},
                    GOLD,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    100,
                    100,
                    0,
                    0,
                    100,
                    5,
                    0,
                    0,
                },
                {0,0,0,0},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
            CreateEmptyStatBonuses(),
            4,      // carry
        };
        break;
        case CHAR_MONK:
        return (Character){
            id,
            "The Monk is a practitioner of wisdom and restraint. Less is more.",
            (CreatureStats){
                (CreatureBaseStats){
                    "Monk",
                    TL_CREATURES,
                    (Vector2){2, 7},
                    DARKGREEN,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    60,
                    120,
                    100,
                    0,
                    100,
                    5,
                    0,
                    0,
                },
                {0,0,0,0},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
            CreateEmptyStatBonuses(),
            4,      // carry
        };
        break;
        case CHAR_FLESH_GOLEM:
        return (Character){
            id,
            "A newly born abominaton, an unlikely aliance. \nThe Flesh Golem is weak, but it absorbs every creature it kills, growing \nwith limitless potential.",
            (CreatureStats){
                (CreatureBaseStats){
                    "Flesh Golem",
                    TL_CREATURES,
                    (Vector2){5, 4},
                    RED,

                    0,
                    400,
                    400,
                    1000,
                    1000,
                    100,
                    100,
                    0,
                    0,
                    100,
                    5,
                    0,
                    0,
                },
                {0,0,0,0},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            {
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
                InitItem(ITEM_NONE),
            },
            CreateEmptyStatBonuses(),
            0,      // carry
        };
        break;
    }
}