#include <stdlib.h>
#include "enemy.h"
#include "dungeon.h"
#include "state.h"
#include "colors.h"

extern struct AppState appState;

ENCOUNTER_ID bossPoolTier1[1];
ENCOUNTER_ID bossPoolTier2[1];

ENCOUNTER_ID encounterPoolTier1[9];
ENCOUNTER_ID encounterPoolTier2[4];
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
    else
    {
        for(int i = 0; i < 3; i++)
        {
            result[i] = encounterPoolTier1[rng_next_u32(&appState.stateData.gameState.runRng) % ARR_SIZE(encounterPoolTier1)];
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
                TL_BIG_CREATURES,
                (Vector2){3,0},
                darkgray,
                "The winged deathbringer, guarding the entrance to the city.",
                {EN_NONE, ENB_BLACK_DRAGON, EN_NONE},
                false,
            };
        case ENCB_GREEN_DRAGON:
            return (Encounter)
            {
                id,
                "Green Dragon",
                TL_BIG_COMMUNITY,
                (Vector2){2,0},
                green,
                "The legendary winged beast stands in your way.",
                {EN_NONE, ENB_GREEN_DRAGON, EN_NONE},
                false,
            };
        case ENC_BONFIRE:
            return (Encounter)
            {
                id,
                "Bonfire",
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
                TL_COMMUNITY,
                (Vector2){4, 4},
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
                TL_CREATURES,
                (Vector2){8,15},
                magenta,
                "A small, mysterious creature, wearing adventuring gear.",
                {EN_NONE, EN_GORLAK, EN_NONE},
                false,
            };
        default:
            return (Encounter)
            {
                id,
                "No encounter",
                TL_GENERAL,
                (Vector2){1, 0},
                darkgray,
                "This path is safe to traverse, neither traps nor monsters lurk there.",
                {EN_NONE, EN_NONE, EN_NONE},
                false,
            };
    }
}