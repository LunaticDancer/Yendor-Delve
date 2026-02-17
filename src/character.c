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
    }
}