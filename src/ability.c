#include <stdlib.h>
#include "ability.h"

Ability InitAbility(ABILITY id)
{
    switch(id)
    {
        case AB_WAIT:
        return (Ability)
        {
            id,
            "Wait",
            0,
            AF_TARGETS_SELF,
        };
        break;
        case AB_MONK_MEDITATE:
        return (Ability)
        {
            id,
            "Meditate",
            0,
            AF_TARGETS_SELF,
        };
        break;
        case AB_MONK_TRUE_STRIKE:
        return (Ability)
        {
            id,
            "True Strike",
            700,
            AF_TARGETS_ENEMIES,
        };
        break;
        case AB_MONK_ATTUNEMENT:
        return (Ability)
        {
            id,
            "Attunement",
            600,
            AF_TARGETS_ALLIES,
        };
        break;
        case AB_MONK_CLEANSE:
        return (Ability)
        {
            id,
            "Cleanse",
            600,
            AF_TARGETS_ALLIES + AF_TARGETS_ENEMIES,
        };
        break;
    }
}

bool DoesAbilityHaveFlag(Ability ab, enum ABILITY_FLAGS flag)
{
	return (ab.abilityFlags & (char)flag) != 0;
}