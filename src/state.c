#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "dungeon.h"
#include "item.h"

extern struct AppState appState;

void InitAppState(enum APP_STATE _state)
{
	switch(_state)
	{
		case AS_MAIN_MENU:
			appState.stateData.mainMenuState.currentSelection = MS_PLAY;
			break;
		case AS_GAMEPLAY:
			appState.stateData.gameState.teamCompMask = 14;
			appState.stateData.gameState.playerTeam[0] = InitCharacterData(CHAR_MONK);
			appState.stateData.gameState.playerTeam[1] = InitCharacterData(CHAR_ASSASSIN);
			appState.stateData.gameState.playerTeam[2] = InitCharacterData(CHAR_DUELIST);
			appState.stateData.gameState.floor = 1;
			appState.stateData.gameState.isPaused = 0;
            InitGameState(GS_CHARACTER_SELECT);
			break;
	}
	appState.appState = _state;
}

void InitGameState(enum GAME_STATE _state)
{
    switch(_state)
    {
        case GS_CHARACTER_SELECT:
        appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected = 0;
        break;

		case GS_DUNGEON:
		appState.stateData.gameState.stateData.dungeonState.isBrowsingEquipment = false;
		appState.stateData.gameState.stateData.dungeonState.selectionX = 0;
		appState.stateData.gameState.stateData.dungeonState.selectionY = 0;
		ENCOUNTER_ID* encounterSelection = SelectRandomEncounters(appState.stateData.gameState.floor);
		appState.stateData.gameState.stateData.dungeonState.encounters[0] = GetEncounterData(encounterSelection[0]);
		appState.stateData.gameState.stateData.dungeonState.encounters[1] = GetEncounterData(encounterSelection[1]);
		appState.stateData.gameState.stateData.dungeonState.encounters[2] = GetEncounterData(encounterSelection[2]);
		appState.stateData.gameState.stateData.dungeonState.rewards[0] = InitItem(MatchRewardToEncounter(encounterSelection[0], appState.stateData.gameState.floor));
		appState.stateData.gameState.stateData.dungeonState.rewards[1] = InitItem(MatchRewardToEncounter(encounterSelection[1], appState.stateData.gameState.floor));
		appState.stateData.gameState.stateData.dungeonState.rewards[2] = InitItem(MatchRewardToEncounter(encounterSelection[2], appState.stateData.gameState.floor));
		break;

		case GS_BATTLE:
		appState.stateData.gameState.stateData.battleState.verticalSelection = 0;
		appState.stateData.gameState.stateData.battleState.horizontalSelection = 0;
		appState.stateData.gameState.stateData.battleState.battleState = BS_ENEMY_TURN;
		appState.stateData.gameState.stateData.battleState.messages[0] = "The battle has begun!";
		appState.stateData.gameState.stateData.battleState.messages[1] = "";
		appState.stateData.gameState.stateData.battleState.messages[2] = "";
		appState.stateData.gameState.stateData.battleState.messages[3] = "";
		appState.stateData.gameState.stateData.battleState.messages[4] = "";
		appState.stateData.gameState.stateData.battleState.messages[5] = "";
    }
	appState.stateData.gameState.gameState = _state;
}

void TransitionToBattle()
{
	Item reward = appState.stateData.gameState.stateData.dungeonState.rewards[
		appState.stateData.gameState.stateData.dungeonState.selectionX / 2];
	Encounter encounter = appState.stateData.gameState.stateData.dungeonState.encounters[
		appState.stateData.gameState.stateData.dungeonState.selectionX / 2];

	InitGameState(GS_BATTLE);

	appState.stateData.gameState.stateData.battleState.reward = reward;
	appState.stateData.gameState.stateData.battleState.enemies[0] = InitEnemyData(encounter.enemies[0]);
	appState.stateData.gameState.stateData.battleState.enemies[1] = InitEnemyData(encounter.enemies[1]);
	appState.stateData.gameState.stateData.battleState.enemies[2] = InitEnemyData(encounter.enemies[2]);

	ResetTurnClock(&appState.stateData.gameState.playerTeam[0].stats, appState.stateData.gameState.playerTeam[0].itemStats.speed);
	ResetTurnClock(&appState.stateData.gameState.playerTeam[1].stats, appState.stateData.gameState.playerTeam[1].itemStats.speed);
	ResetTurnClock(&appState.stateData.gameState.playerTeam[2].stats, appState.stateData.gameState.playerTeam[2].itemStats.speed);
	ResetTurnClock(&appState.stateData.gameState.stateData.battleState.enemies[0].stats, 0);
	ResetTurnClock(&appState.stateData.gameState.stateData.battleState.enemies[1].stats, 0);
	ResetTurnClock(&appState.stateData.gameState.stateData.battleState.enemies[2].stats, 0);

	PassTurn();
}

void PassTurn()
{
	short timeToProgress = DetermineCurrentActingEntity();
	ProgressTime(timeToProgress);
}

void AddMessageToFeed(char* msg)
{
		appState.stateData.gameState.stateData.battleState.messages[5] = appState.stateData.gameState.stateData.battleState.messages[4];
		appState.stateData.gameState.stateData.battleState.messages[4] = appState.stateData.gameState.stateData.battleState.messages[3];
		appState.stateData.gameState.stateData.battleState.messages[3] = appState.stateData.gameState.stateData.battleState.messages[2];
		appState.stateData.gameState.stateData.battleState.messages[2] = appState.stateData.gameState.stateData.battleState.messages[1];
		appState.stateData.gameState.stateData.battleState.messages[1] = appState.stateData.gameState.stateData.battleState.messages[0];
		appState.stateData.gameState.stateData.battleState.messages[0] = msg;
}

void AddItemToInventory(Item it)
{
	for(int i = 0; i < INVENTORY_SIZE; i++)
	{
		if(appState.stateData.gameState.inventory[i].itemId != ITEM_NONE) continue;

		appState.stateData.gameState.inventory[i] = it;
		return;
	}
}

void HandleItemEquip()
{
	if (appState.stateData.gameState.playerTeam[
		appState.stateData.gameState.stateData.dungeonState.selectionX
	].items[appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot].itemId != ITEM_NONE)
	{
		AddItemToInventory(appState.stateData.gameState.playerTeam[
		appState.stateData.gameState.stateData.dungeonState.selectionX
	].items[appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot]);
	}

	UnequipItem(&appState.stateData.gameState.playerTeam[
		appState.stateData.gameState.stateData.dungeonState.selectionX], 
		appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot);

	EquipItem(&appState.stateData.gameState.playerTeam[
		appState.stateData.gameState.stateData.dungeonState.selectionX], 
		(appState.stateData.gameState.stateData.dungeonState.highlightedItem == 0) ? ITEM_NONE : 
		appState.stateData.gameState.inventory[appState.stateData.gameState.stateData.dungeonState.slotItemIndexes[
			appState.stateData.gameState.stateData.dungeonState.highlightedItem - 1]].itemId,
		appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot);

	if (appState.stateData.gameState.stateData.dungeonState.highlightedItem != 0)
	{
	appState.stateData.gameState.inventory[appState.stateData.gameState.stateData.dungeonState.slotItemIndexes[
		appState.stateData.gameState.stateData.dungeonState.highlightedItem - 1]] = InitItem(ITEM_NONE);
	}

	appState.stateData.gameState.stateData.dungeonState.isSelectingItem = false;
}

short DetermineCurrentActingEntity()
{
	char result = 0;
	short minTicks = appState.stateData.gameState.playerTeam[0].stats.baseStats.ticksUntilNextTurn;
	appState.stateData.gameState.stateData.battleState.battleState = BS_PLAYER_ABILITY_SELECT;

	// no point in turning something this trivial into a loop
	if (appState.stateData.gameState.playerTeam[1].stats.baseStats.ticksUntilNextTurn < minTicks)
	{
		minTicks = appState.stateData.gameState.playerTeam[1].stats.baseStats.ticksUntilNextTurn;
		result = 1;
	}
	if (appState.stateData.gameState.playerTeam[2].stats.baseStats.ticksUntilNextTurn < minTicks)
	{
		minTicks = appState.stateData.gameState.playerTeam[2].stats.baseStats.ticksUntilNextTurn;
		result = 2;
	}
	if (appState.stateData.gameState.stateData.battleState.enemies[0].stats.baseStats.ticksUntilNextTurn < minTicks)
	{
		appState.stateData.gameState.stateData.battleState.battleState = BS_ENEMY_TURN;
		minTicks = appState.stateData.gameState.stateData.battleState.enemies[0].stats.baseStats.ticksUntilNextTurn;
		result = 3;
	}
	if (appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.ticksUntilNextTurn < minTicks)
	{
		appState.stateData.gameState.stateData.battleState.battleState = BS_ENEMY_TURN;
		minTicks = appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.ticksUntilNextTurn;
		result = 4;
	}
	if (appState.stateData.gameState.stateData.battleState.enemies[2].stats.baseStats.ticksUntilNextTurn < minTicks)
	{
		appState.stateData.gameState.stateData.battleState.battleState = BS_ENEMY_TURN;
		minTicks = appState.stateData.gameState.stateData.battleState.enemies[2].stats.baseStats.ticksUntilNextTurn;
		result = 5;
	}

	appState.stateData.gameState.stateData.battleState.currentActingEntity = result;
	return minTicks;
}

void ProgressTime(short ticks)
{
	appState.stateData.gameState.playerTeam[0].stats.baseStats.ticksUntilNextTurn -= ticks;
	appState.stateData.gameState.playerTeam[1].stats.baseStats.ticksUntilNextTurn -= ticks;
	appState.stateData.gameState.playerTeam[2].stats.baseStats.ticksUntilNextTurn -= ticks;
	appState.stateData.gameState.stateData.battleState.enemies[0].stats.baseStats.ticksUntilNextTurn -= ticks;
	appState.stateData.gameState.stateData.battleState.enemies[1].stats.baseStats.ticksUntilNextTurn -= ticks;
	appState.stateData.gameState.stateData.battleState.enemies[2].stats.baseStats.ticksUntilNextTurn -= ticks;
}

void PrepareListOfSlotAppropriateItems(EQUIPMENT_SLOT slot)
{
	char* result = malloc(sizeof(char));
	char numberOfPositions = 0;

	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		if((appState.stateData.gameState.inventory[i].slot == slot ||
			appState.stateData.gameState.inventory[i].slot == ES_EVERY)
		&& appState.stateData.gameState.inventory[i].itemId != ITEM_NONE)
		{
			numberOfPositions++;
			result = realloc(result, numberOfPositions * sizeof(char));
			result[numberOfPositions-1] = i;
		}
	}

	appState.stateData.gameState.stateData.dungeonState.itemIndexListLength = numberOfPositions;
	appState.stateData.gameState.stateData.dungeonState.slotItemIndexes = result;
}

bool CheckIfHeroInParty(CHARACTER_ID id)
{
	return appState.stateData.gameState.teamCompMask & (1 << id);
}

void HandleRealTimePopups()
{
	if(appState.appState == AS_GAMEPLAY)
	{
		appState.stateData.gameState.messageTimer -= GetFrameTime();
	}
}

void ShowPopupMessage(char* msg)
{
	appState.stateData.gameState.messageTimer = GAME_MESSAGE_DISPLAY_TIME + strlen(msg) * GAME_MESSAGE_DISPLAY_TIME_PER_CHARACTER;
	appState.stateData.gameState.message = msg;
}