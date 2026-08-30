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
        case AB_BERSERKER_SWING:
        return (Ability)
        {
            id,
            "Wild Swing",
            10,
            AF_TARGETS_ENEMIES,
        };
        case AB_BERSERKER_BASH:
        return (Ability)
        {
            id,
            "Shield Bash",
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_BERSERKER_BATTLECRY:
        return (Ability)
        {
            id,
            "Battlecry",
            0,
            AF_TARGETS_SELF,
        };
        case AB_BERSERKER_BRACE:
        return (Ability)
        {
            id,
            "Brace",
            150,
            AF_TARGETS_SELF,
        };
        case AB_ASSASSIN_SLASH:
        return (Ability)
        {
            id,
            "Shiv",
            50,
            AF_TARGETS_ENEMIES,
        };
        case AB_ASSASSIN_PREPARE:
        return (Ability)
        {
            id,
            "Prepare",
            200,
            AF_TARGETS_SELF,
        };
        case AB_ASSASSIN_CONCEAL:
        return (Ability)
        {
            id,
            "Conceal",
            150,
            AF_TARGETS_ALLIES,
        };
        case AB_ASSASSIN_REND:
        return (Ability)
        {
            id,
            "Rend",
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_DUELIST_LUNGE:
        return (Ability)
        {
            id,
            "Lunge",
            70,
            AF_TARGETS_ENEMIES,
        };
        case AB_DUELIST_OPPORTUNITY:
        return (Ability)
        {
            id,
            "Opportunity",
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_DUELIST_PARRY:
        return (Ability)
        {
            id,
            "Parry",
            200,
            AF_TARGETS_SELF,
        };
        case AB_DUELIST_BREATH:
        return (Ability)
        {
            id,
            "Steady Breath",
            0,
            AF_TARGETS_SELF,
        };
        case AB_MONK_MEDITATE:
        return (Ability)
        {
            id,
            "Meditate",
            0,
            AF_TARGETS_SELF,
        };
        case AB_MONK_TRUE_STRIKE:
        return (Ability)
        {
            id,
            "True Strike",
            700,
            AF_TARGETS_ENEMIES,
        };
        case AB_MONK_ATTUNEMENT:
        return (Ability)
        {
            id,
            "Attunement",
            600,
            AF_TARGETS_ALLIES,
        };
        case AB_MONK_CLEANSE:
        return (Ability)
        {
            id,
            "Cleanse",
            600,
            AF_TARGETS_ALLIES + AF_TARGETS_ENEMIES,
        };
        case AB_SHAPESHIFTER_SCRATCH:
        return (Ability)
        {
            id,
            "Scratch",
            10,
            AF_TARGETS_ENEMIES,
        };
        case AB_SHAPESHIFTER_TRANSFORM:
        return (Ability)
        {
            id,
            "Transform",
            10,
            AF_TARGETS_ENEMIES,
        };
    }
}

bool DoesAbilityHaveFlag(Ability ab, enum ABILITY_FLAGS flag)
{
	return (ab.abilityFlags & (char)flag) != 0;
}