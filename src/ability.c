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
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_BERSERKER_BASH:
        return (Ability)
        {
            id,
            "Shield Bash",
            200,
            AF_TARGETS_ENEMIES,
        };
        case AB_BERSERKER_BATTLECRY:
        return (Ability)
        {
            id,
            "Battlecry",
            1,
            AF_TARGETS_SELF,
        };
        case AB_BERSERKER_BRACE:
        return (Ability)
        {
            id,
            "Brace",
            250,
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
            500,
            AF_TARGETS_ENEMIES,
        };
        case AB_MONK_ATTUNEMENT:
        return (Ability)
        {
            id,
            "Attunement",
            500,
            AF_TARGETS_ALLIES,
        };
        case AB_MONK_CLEANSE:
        return (Ability)
        {
            id,
            "Cleanse",
            300,
            AF_TARGETS_ALLIES + AF_TARGETS_ENEMIES,
        };
        case AB_FOLEM_STRIKE:
        return (Ability)
        {
            id,
            "Slam",
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_FOLEM_EXPUNGE:
        return (Ability)
        {
            id,
            "Expunge",
            0,
            AF_TARGETS_ENEMIES,
        };
        case AB_FOLEM_EPIDERMIZE:
        return (Ability)
        {
            id,
            "Epidermize",
            200,
            AF_TARGETS_SELF,
        };
        case AB_FOLEM_CRIPPLE:
        return (Ability)
        {
            id,
            "Cripple",
            300,
            AF_TARGETS_ENEMIES,
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
        case AB_MAGUS_DISINTEGRATE:
        return (Ability)
        {
            id,
            "Disintegrate",
            MAGUS_DISINTEGRATE_COST,
            AF_TARGETS_ENEMIES,
        };
        case AB_MAGUS_ARCANE_BLAST:
        return (Ability)
        {
            id,
            "Arcane Blast",
            MAGUS_ARCANE_BLAST_COST,
            AF_TARGETS_ENEMIES+AF_AOE,
        };
        case AB_MAGUS_TUTOR:
        return (Ability)
        {
            id,
            "Tutor",
            MAGUS_TUTOR_COST,
            AF_TARGETS_ALLIES,
        };
        case AB_MAGUS_SARCOPHAGUS:
        return (Ability)
        {
            id,
            "Sarcophagus",
            MAGUS_SARCOPHAGUS_COST,
            AF_TARGETS_ALLIES,
        };
        case AB_MIMIC_CHOMP:
        return (Ability)
        {
            id,
            "Chomp",
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_MIMIC_IMPALE:
        return (Ability)
        {
            id,
            "Impale",
            120,
            AF_TARGETS_ENEMIES+AF_AOE,
        };
        case AB_MIMIC_PETRIFY:
        return (Ability)
        {
            id,
            "Petrify",
            80,
            AF_TARGETS_ENEMIES,
        };
        case AB_BLOFAEWAR_CUT:
        return (Ability)
        {
            id,
            "Cut",
            40,
            AF_TARGETS_ENEMIES,
        };
        case AB_BLOFAEMYS_INSPIRE:
        return (Ability)
        {
            id,
            "Inspire",
            10,
            AF_TARGETS_ALLIES + AF_AOE,
        };
        case AB_BLOFAEMYS_HASTE:
        return (Ability)
        {
            id,
            "Hasten",
            20,
            AF_TARGETS_ALLIES,
        };
        case AB_BLOFAEMYS_MOCK:
        return (Ability)
        {
            id,
            "Mock",
            5,
            AF_TARGETS_ENEMIES,
        };
        case AB_CAUCUBE_NIBBLE:
        return (Ability)
        {
            id,
            "Nibble",
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_CAUCUBE_SPLIT:
        return (Ability)
        {
            id,
            "Split",
            200,
            AF_TARGETS_SELF,
        };
        case AB_CAUCUBE_METABOLIZE:
        return (Ability)
        {
            id,
            "Metabolize",
            50,
            AF_TARGETS_SELF,
        };
        case AB_TOOFAIRY_SUMMON:
        return (Ability)
        {
            id,
            "Summon",
            80,
            AF_TARGETS_SELF,
        };
        case AB_TOOFAIRY_CALCINIZE:
        return (Ability)
        {
            id,
            "Calcinize",
            250,
            AF_TARGETS_ENEMIES+AF_AOE,
        };
        case AB_TOOFAIRY_BONEDUST:
        return (Ability)
        {
            id,
            "Bonedust",
            50,
            AF_TARGETS_ALLIES,
        };
        case AB_TOOTHOLEM_STRIKE:
        return (Ability)
        {
            id,
            "Strike",
            50,
            AF_TARGETS_ENEMIES,
        };
        case AB_TOOTHOLEM_HARDEN:
        return (Ability)
        {
            id,
            "Harden",
            50,
            AF_TARGETS_ENEMIES,
        };
        case AB_TOOTHOLEM_PROTECT:
        return (Ability)
        {
            id,
            "Protect",
            200,
            AF_TARGETS_ALLIES+AF_AOE,
        };
        case AB_STEVENANT_SIPHON:
        return (Ability)
        {
            id,
            "Siphon",
            100,
            AF_TARGETS_ENEMIES,
        };
        case AB_STEVENANT_PHASING_STRIKE:
        return (Ability)
        {
            id,
            "Phasing Strike",
            80,
            AF_TARGETS_ENEMIES,
        };
        case AB_STEVENANT_ECTOPLASMIC_MANIFESTATION:
        return (Ability)
        {
            id,
            "Ectoplasmic Manifestation",
            100,
            AF_TARGETS_SELF,
        };
    }
}

bool DoesAbilityHaveFlag(Ability ab, enum ABILITY_FLAGS flag)
{
	return (ab.abilityFlags & (char)flag) != 0;
}