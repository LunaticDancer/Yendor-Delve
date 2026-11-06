#ifndef enemy_h
#define enemy_h

#include "creature.h"
#include "item.h"
#include "ability.h"

typedef enum ENEMY_ID
{
    EN_NONE,
    ENB_DUNGEON_MASTER,
    ENB_GREEN_DRAGON,
    ENB_CHOIR_LEFT,
    ENB_CHOIR_PREACHER,
    ENB_CHOIR_RIGHT,
    ENB_BLACK_DRAGON,
    EN_CULTIST_SUMMON,
    EN_MIMIC,
    EN_BLOOD_FAE_WARRIOR,
    EN_BLOOD_FAE_MYSTIC,
    EN_CAUSTIC_CUBE,
    EN_CAUSTIC_CUBE_CLONE,
    EN_TOOTH_FAIRY,
    EN_TOOTH_GOLEM,
    EN_STARVING_REVENANT,
    EN_BANDIT_STRONGMAN,
    EN_BANDIT_CUTTHROAT,
    EN_BANDIT_JESTER,
    EN_GORLAK,
    EN_ICE_ELEMENTAL,
    EN_FIRE_ELEMENTAL,
    EN_MANRAT,
    EN_GIANT_FROG,
    EN_FROG_STINKFLY,
    EN_QUICKPIG,
    EN_PIGJAR,
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
    char* customDeathMessage;
} Enemy;

bool IsEnemyIrrelevant(Enemy e);
Enemy InitEnemyData(ENEMY_ID id);

#endif
