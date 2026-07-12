#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>

Enemy InitEnemyData(ENEMY_ID id)
{
    switch(id)
    {
        case EN_NONE:
        return (Enemy){
            id,
            (CreatureStats){
                (CreatureBaseStats){
                    "None",
                    TL_GENERAL,
                    (Vector2){1,0},
                    GRAY,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    100,
                    100,
                    50,
                    1,
                    100,
                    5,
                    0,
                    0,
                },
                {},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            TG_TRUE_RANDOM,
            100,
        };
        break;
        
        case EN_MIMIC:
        return (Enemy){
            id,
            (CreatureStats){
                (CreatureBaseStats){
                    "Mimic",
                    TL_CREATURES,
                    (Vector2){5,14},
                    BROWN,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    100,
                    100,
                    50,
                    1,
                    100,
                    5,
                    0,
                    0,
                },
                {},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            TG_WEIGHTED_RANDOM,
            100,
        };
        break;
        
        case EN_BLOOD_FAE_WARRIOR:
        return (Enemy){
            id,
            (CreatureStats){
                (CreatureBaseStats){
                    "Blood Fae Warrior",
                    TL_CREATURES,
                    (Vector2){12,3},
                    RED,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    100,
                    100,
                    50,
                    1,
                    100,
                    5,
                    0,
                    0,
                },
                {},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            TG_WEIGHTED_RANDOM,
            100,
        };
        break;
        
        case EN_BLOOD_FAE_MYSTIC:
        return (Enemy){
            id,
            (CreatureStats){
                (CreatureBaseStats){
                    "Blood Fae Mystic",
                    TL_CREATURES,
                    (Vector2){13,3},
                    RED,

                    0,
                    1000,
                    1000,
                    1000,
                    1000,
                    100,
                    100,
                    50,
                    1,
                    100,
                    5,
                    0,
                    0,
                },
                {},
                CreateEmptyStatBonuses(),
                (Ability*)malloc(0),
                0,
            },
            TG_WEIGHTED_RANDOM,
            100,
        };
        break;
    }
}