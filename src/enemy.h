#ifndef enemy_h
#define enemy_h

#include "creature.h"
#include "item.h"
#include "ability.h"

typedef enum ENEMY_ID
{
    EN_NONE,
    EN_MIMIC,
    EN_BLOOD_FAE_WARRIOR,
    EN_BLOOD_FAE_MYSTIC,
    EN_CAUSTIC_CUBE,
    EN_CAUSTIC_CUBE_CLONE,
    EN_TOOTH_FAIRY,
    EN_TOOTH_GOLEM,
    EN_STARVING_REVENANT,
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
    ABILITY lastUsedAbility;
} Enemy;

Enemy InitEnemyData(ENEMY_ID id);

#endif