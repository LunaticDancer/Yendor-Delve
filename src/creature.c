#include "creature.h"

StatBonuses CreateEmptyStatBonuses()
{
    return (StatBonuses)
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,
    };
}

void CreateEmptyStatusEffects(int statusEffects[])
{
    for (int i = 0; i < SE_LENGTH; i++)
    {
        statusEffects[i] = 0;
    }
}