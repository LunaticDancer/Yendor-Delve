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
    }
}

bool DoesAbilityHaveFlag(Ability ab, enum ABILITY_FLAGS flag)
{
	return (ab.abilityFlags & (char)flag) != 0;
}