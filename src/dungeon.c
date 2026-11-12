#include <stdlib.h>
#include "enemy.h"
#include "dungeon.h"
#include "state.h"
#include "colors.h"

extern struct AppState appState;

ENCOUNTER_ID bossPoolTier1[4];
ENCOUNTER_ID bossPoolTier2[1];

ENCOUNTER_ID encounterPoolTier1[11];
ENCOUNTER_ID encounterPoolTier2[5];
ENCOUNTER_ID encounterPoolTier3[4];

ITEM_ID itemPoolTier1[ITEM_POOL_TIER_ONE_SIZE];
ITEM_ID itemPoolTier2[ITEM_POOL_TIER_TWO_SIZE];
ITEM_ID itemPoolTier3[ITEM_POOL_TIER_THREE_SIZE];


ENCOUNTER_ID* SelectRandomEncounters(char floor)
{
    ENCOUNTER_ID* result = (ENCOUNTER_ID*)malloc(3 * sizeof(ENCOUNTER_ID));

    if(floor == 24)
    {
        result[0] = ENC_NONE;
        result[1] = ENCB_DUNGEON_MASTER;
        result[2] = ENC_NONE;
    }
    else if(floor == 8)
    {
        for(int i = 0; i < 3; i++)
        {
            result[i] = bossPoolTier1[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(bossPoolTier1)];
        }
    }
    else if(floor == 16)
    {
        for(int i = 0; i < 3; i++)
        {
            result[i] = bossPoolTier2[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(bossPoolTier2)];
        }
    }
    else if(floor > 8 && floor < 16) 
    {
        for(int i = 0; i < 3; i++)
        {
            result[i] = encounterPoolTier2[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(encounterPoolTier2)];
        }
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            result[i] = encounterPoolTier1[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(encounterPoolTier1)];
            // duplicate prevention
            for(int j = 0; j < i; j++)
            {
                if(result[i] == result[j])
                {
                    result[i] = ARR_SIZE(encounterPoolTier1) - j - 1;
                }
            }
        }
    }
    if(floor % 8 == 5)
    {
        result[1] = ENC_BONFIRE;
    }

    return result;
}

ITEM_ID MatchRewardToEncounter(ENCOUNTER_ID enc, char floor)
{
    ITEM_ID result = ITEM_NONE;
    switch(floor)
    {
        case 24:
        result = ITEM_AMULET_OF_YENDOR;
        break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        {
            switch(enc)
            {
                case ENC_BONFIRE: case ENC_NONE:
                result = ITEM_NONE;
                break;

                case ENC_CHEST: case ENC_MIMIC:
                result  = itemPoolTier2[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(itemPoolTier2)];
                break;

                default:
                result  = itemPoolTier1[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(itemPoolTier1)];
                break;
            }
            break;
        }
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: default:
        {
            switch(enc)
            {
                case ENC_BONFIRE: case ENC_NONE:
                result = ITEM_NONE;
                break;

                default:
                result  = itemPoolTier2[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(itemPoolTier2)];
                break;
            }
            break;
        }
    }
    return result;
}

void InitEncounterPools()
{
    bossPoolTier1[0] = ENCB_GREEN_DRAGON;
    bossPoolTier1[1] = ENCB_CHOIR;
    bossPoolTier1[2] = ENCB_CAT_WIZARD;
    bossPoolTier1[3] = ENCB_CAT_WIZARD;
    
    bossPoolTier2[0] = ENCB_BLACK_DRAGON;

    encounterPoolTier1[0] = ENC_BLOOD_FAERIES;
    encounterPoolTier1[1] = ENC_MIMIC;
    encounterPoolTier1[2] = ENC_CHEST;
    encounterPoolTier1[3] = ENC_NONE;
    encounterPoolTier1[4] = ENC_CAUSTIC_CUBE;
    encounterPoolTier1[5] = ENC_TOOTH_FAERIES;
    encounterPoolTier1[6] = ENC_GHOSTS_EASY;
    encounterPoolTier1[7] = ENC_BANDITS_EASY;
    encounterPoolTier1[8] = ENC_GORLAK;
    encounterPoolTier1[9] = ENC_ELEMENTAL_DUO;
    encounterPoolTier1[10] = ENC_MANRAT_TROUPE;

    encounterPoolTier2[0] = ENC_GIANT_FROG;
    encounterPoolTier2[1] = ENC_JAR_CAMP;
    encounterPoolTier2[2] = ENC_CHEST;
    encounterPoolTier2[3] = ENC_MIMIC;
    encounterPoolTier2[4] = ENC_TUNNEL_WYRMS;
}

void InitItemPools()
{
    itemPoolTier1[0] = ITEM_APPRENTICE_BOOTS;
    itemPoolTier1[1] = ITEM_APPRENTICE_ROBE;
    itemPoolTier1[2] = ITEM_APPRENTICE_STAFF;
    itemPoolTier1[3] = ITEM_APPRENTICE_TIARA;
    itemPoolTier1[4] = ITEM_JUGGERNAUT_ARMOR;
    itemPoolTier1[5] = ITEM_JUGGERNAUT_GREATHAMMER;
    itemPoolTier1[6] = ITEM_JUGGERNAUT_HELM;
    itemPoolTier1[7] = ITEM_JUGGERNAUT_LEGGINS;
    itemPoolTier1[8] = ITEM_MERCENARY_AXE;
    itemPoolTier1[9] = ITEM_MERCENARY_CUIRAS;
    itemPoolTier1[10] = ITEM_MERCENARY_GREAVES;
    itemPoolTier1[11] = ITEM_MERCENARY_HELMET;
    itemPoolTier1[12] = ITEM_THIEF_CAPE;
    itemPoolTier1[13] = ITEM_THIEF_DAGGER;
    itemPoolTier1[14] = ITEM_THIEF_HOOD;
    itemPoolTier1[15] = ITEM_THIEF_PANTS;
    itemPoolTier1[16] = ITEM_SOFUS_HAT;
    itemPoolTier1[17] = ITEM_SOFUS_LEGS;
    itemPoolTier1[18] = ITEM_SOFUS_TORSO;
    itemPoolTier1[19] = ITEM_TOWER_SHIELD;
    itemPoolTier1[20] = ITEM_JESTER_HAT;
    itemPoolTier1[21] = ITEM_HALLOWED_FLESH;
    itemPoolTier1[22] = ITEM_TRAVELER_SHOES;
    itemPoolTier1[23] = ITEM_BATTLE_SHIELD;
    itemPoolTier1[24] = ITEM_STONE_BOOTS;
    itemPoolTier1[25] = ITEM_TOY_DAGGER;

    itemPoolTier2[0] = ITEM_ELEMANCER_CROWN;
    itemPoolTier2[1] = ITEM_ELEMANCER_CAPE;
    itemPoolTier2[2] = ITEM_ELEMANCER_BELT;
    itemPoolTier2[3] = ITEM_ELEMANCER_AXE;
    itemPoolTier2[4] = ITEM_WANDERER_HAT;
    itemPoolTier2[5] = ITEM_PENITENT_MAIL;
    itemPoolTier2[6] = ITEM_COMFORTABLE_SHOES;
    itemPoolTier2[7] = ITEM_HAMMER_OF_GLORY;
    itemPoolTier2[8] = ITEM_CROWN_OF_FLOWERS;
    itemPoolTier2[9] = ITEM_LIVING_CHAINMAIL;
    itemPoolTier2[10] = ITEM_BALL_ON_CHAIN;
    itemPoolTier2[11] = ITEM_MORNINGSTAR;
    itemPoolTier2[12] = ITEM_MALEVOLENCE;
    itemPoolTier2[13] = ITEM_CRYSTAL_MAIL;
    itemPoolTier2[14] = ITEM_ASSASSIN_SHOES;
    itemPoolTier2[15] = ITEM_AMBROSIA;
    itemPoolTier2[16] = ITEM_HOLLOW_SKULL;
    itemPoolTier2[17] = ITEM_EIGHT_EYED_MASK;
    itemPoolTier2[18] = ITEM_SCRYER_BALL;
    itemPoolTier2[19] = ITEM_ANTLER_HELMET;
    itemPoolTier2[20] = ITEM_GOLDEN_CAPE;
    itemPoolTier2[21] = ITEM_GOLDEN_SKIRT;
    itemPoolTier2[22] = ITEM_COIN;
    itemPoolTier2[23] = ITEM_ILLUSION_PANTS;
    itemPoolTier2[24] = ITEM_FOOT_FUNGUS;
    itemPoolTier2[25] = ITEM_MASTER_ROBE;
    itemPoolTier2[26] = ITEM_VENGEFUL_SPIRIT;
    itemPoolTier2[27] = ITEM_ROSE_SIGNET;
    itemPoolTier2[28] = ITEM_TRUMPET;
}

Encounter GetEncounterData(ENCOUNTER_ID id)
{
    switch(id)
    {
        case ENCB_DUNGEON_MASTER:
            return (Encounter)
            {
                id,
                "Dungeon Master",
                "They await you, calmly and expectingly.",
                TL_BIG_CREATURES,
                (Vector2){5,3},
                gold,
                "One above all.",
                {EN_NONE, ENB_DUNGEON_MASTER, EN_NONE},
                false,
            };
        case ENCB_BLACK_DRAGON:
            return (Encounter)
            {
                id,
                "Black Dragon",
                "\0",
                TL_BIG_COMMUNITY,
                (Vector2){6,0},
                darkgray,
                "The winged deathbringer, guarding the entrance to the city.",
                {EN_NONE, ENB_BLACK_DRAGON, EN_NONE},
                false,
            };
        case ENCB_CAT_WIZARD:
            return (Encounter)
            {
                id,
                "Archmage Nebulon",
                "The little cat hisses in hostility. You can tell by its intent stare, that if it could speak to you, it would.",
                TL_CREATURES,
                (Vector2){0,9},
                purple,
                "A small beast stands in your way, unshaken in its resolve.",
                {EN_NONE, ENB_CAT_WIZARD, ENB_CAT_WIZARD_PILLAR},
                false,
            };
        case ENCB_GREEN_DRAGON:
            return (Encounter)
            {
                id,
                "Green Dragon",
                "\0",
                TL_BIG_COMMUNITY,
                (Vector2){2,0},
                green,
                "The legendary winged beast stands in your way.",
                {EN_NONE, ENB_GREEN_DRAGON, EN_NONE},
                false,
            };
        case ENCB_CHOIR:
            return (Encounter)
            {
                id,
                "Mass",
                "Please, take a seat. You are just in time for mass.",
                TL_BIG_COMMUNITY,
                (Vector2){5,0},
                orange,
                "The air grows heavy with song. All else falls silent.",
                {ENB_CHOIR_RIGHT, ENB_CHOIR_PREACHER, ENB_CHOIR_LEFT},
                false,
            };
        case ENC_BONFIRE:
            return (Encounter)
            {
                id,
                "Bonfire",
                "\0",
                TL_GENERAL,
                (Vector2){1, 1},
                orange,
                "A safe resting spot to regenerate one's strength.",
                {EN_NONE, EN_NONE, EN_NONE},
                false,
            };
        case ENC_CHEST:
            return (Encounter)
            {
                id,
                "Chest",
                "\0",
                TL_GENERAL,
                (Vector2){10, 1},
                brown,
                "A tightly locked treasure chest. Whatever is inside must be valuable.",
                {EN_NONE, EN_NONE, EN_NONE},
                true,
            };
        case ENC_MIMIC:
            return (Encounter)
            {
                id,
                "Chest",
                "The chest was a trap! Prepare to face trouble.",
                TL_GENERAL,
                (Vector2){10, 1},
                brown,
                "A tightly locked treasure chest. Whatever is inside must be valuable.",
                {EN_NONE, EN_MIMIC, EN_NONE},
                true,
            };
        case ENC_BLOOD_FAERIES:
            return (Encounter)
            {
                id,
                "Blood Fae",
                "\0",
                TL_CREATURES,
                (Vector2){12, 3},
                red,
                "A group of quick and tiny pests. While their scratches are barely enough to draw blood, the tiny wounds add up.",
                {EN_BLOOD_FAE_WARRIOR, EN_BLOOD_FAE_MYSTIC, EN_BLOOD_FAE_WARRIOR},
                false,
            };
        case ENC_CAUSTIC_CUBE:
            return (Encounter)
            {
                id,
                "Caustic Cube",
                "\0",
                TL_CREATURES,
                (Vector2){6, 14},
                green,
                "A large and durable monster, capable of splitting into lesser copies of itself and dulling your weapons with its acid.",
                {EN_NONE, EN_CAUSTIC_CUBE, EN_NONE},
                false,
            };
        case ENC_TOOTH_FAERIES:
            return (Encounter)
            {
                id,
                "Tooth Fae",
                "\0",
                TL_QUIEK,
                (Vector2){2,0},
                raywhite,
                "Where are they getting all these teeth from, and what do they plan to do with them?",
                {EN_TOOTH_FAIRY, EN_NONE, EN_TOOTH_FAIRY},
                false,
            };
        case ENC_GHOSTS_EASY:
            return (Encounter)
            {
                id,
                "Revenant Souls",
                "\0",
                TL_CREATURES,
                (Vector2){4,6},
                skyblue,
                "Sorrowful dead, intangible and cold. Stay too long in their presence and you'll become motionless as well.",
                {EN_STARVING_REVENANT, EN_NONE, EN_STARVING_REVENANT},
                false,
            };
        case ENC_BANDITS_EASY:
            return (Encounter)
            {
                id,
                "Bandit Troupe",
                "You hear a holler behind you: \"Get'em boys!\"",
                TL_CREATURES,
                (Vector2){0,1},
                gray,
                "An adventuring party gone rogue, more coordinated than your average group of monsters.",
                {EN_BANDIT_JESTER, EN_BANDIT_STRONGMAN, EN_BANDIT_CUTTHROAT},
                false,
            };
        case ENC_GORLAK:
            return (Encounter)
            {
                id,
                "Gorlak",
                "Gorlak strolls into battle.",
                TL_CREATURES,
                (Vector2){8,15},
                magenta,
                "A small, mysterious creature, wearing adventuring gear.",
                {EN_NONE, EN_GORLAK, EN_NONE},
                false,
            };
        case ENC_ELEMENTAL_DUO:
            return (Encounter)
            {
                id,
                "Elemental Duo",
                "\0",
                TL_CREATURES,
                (Vector2){11, 6},
                skyblue,
                "Elementals of ice and fire. An unlikely pairing.",
                {EN_ICE_ELEMENTAL, EN_NONE, EN_FIRE_ELEMENTAL},
                false,
            };
        case ENC_MANRAT_TROUPE:
            return (Encounter)
            {
                id,
                "Manrat Troupe",
                "\0",
                TL_CREATURES,
                (Vector2){14, 3},
                lightgray,
                "A nest of manrats lies in your way. Nothing of value will be lost tearing it down.",
                {EN_MANRAT, EN_MANRAT, EN_MANRAT},
                false,
            };
        case ENC_GIANT_FROG:
            return (Encounter)
            {
                id,
                "Giant Frog",
                "A giant frog, alerted by your approach, pauses its hunt for stinkflies.",
                TL_JTAD,
                (Vector2){4, 2},
                darkblue,
                "This ambush predator usually just goes for bugs, but anything that fits in its mouth is fair game.",
                {EN_FROG_STINKFLY, EN_GIANT_FROG, EN_FROG_STINKFLY},
                false,
            };
        case ENC_JAR_CAMP:
            return (Encounter)
            {
                id,
                "Abandoned Hideout",
                "A Quickpig dashes between the jars.",
                TL_JTAD,
                (Vector2){3, 0},
                brown,
                "A camp filled with jars, probably with goodies around. Beware of scavengers!",
                {EN_PIGJAR, EN_QUICKPIG, EN_PIGJAR},
                false,
            };
        case ENC_TUNNEL_WYRMS:
            return (Encounter)
            {
                id,
                "Wyrm Warren",
                "A pair of wyrms slithers out from the darkness. Although aggressive, they never fully leave their hiding spots.",
                TL_PHOLIDIA,
                (Vector2){8, 0},
                maroon,
                "The cave surface is covered in countless perfectly circular holes.",
                {EN_TUNNEL_WYRM, EN_NONE, EN_TUNNEL_WYRM},
                false,
            };
        default:
            return (Encounter)
            {
                id,
                "No encounter",
                "\0",
                TL_GENERAL,
                (Vector2){1, 0},
                darkgray,
                "This path is safe to traverse, neither traps nor monsters lurk there.",
                {EN_NONE, EN_NONE, EN_NONE},
                false,
            };
    }
}
