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

void ResetTurnClock(CreatureStats _creature, short itemSpeedBonus)
{
    float value = 100.0 / (100 + _creature.baseStats.speed + _creature.encounterStats.speed + itemSpeedBonus);
    _creature.baseStats.ticksUntilNextTurn = (short)value;
}