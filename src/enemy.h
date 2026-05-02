#ifndef enemy
#define enemy

#include "creature.h"
#include "item.h"
#include "ability.h"

typedef enum ENEMY_ID
{
    EN_NONE,
    EN_MIMIC,
    EN_BLOOD_FAE_WARRIOR,
    EN_BLOOD_FAE_MYSTIC,
} ENEMY_ID;

typedef enum TARGETTING_TYPE
{
    TG_FRONT,
    TG_BACK,
    TG_WEIGHTED_RANDOM,
    TG_TRUE_RANDOM,
} TARGETTING_TYPE;

typedef struct Enemy
{
    ENEMY_ID enemyId;
    CreatureStats stats;
    TARGETTING_TYPE targettingBehavior;
    short goldReward;
} Enemy;

#endif