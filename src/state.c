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
			appState.stateData.gameState.teamCompMask = 7;
			appState.stateData.gameState.playerTeam[0] = InitCharacterData(CHAR_BERSERKER);
			appState.stateData.gameState.playerTeam[1] = InitCharacterData(CHAR_ASSASSIN);
			appState.stateData.gameState.playerTeam[2] = InitCharacterData(CHAR_DUELIST);
			appState.stateData.gameState.floor = 0;
			appState.stateData.gameState.isPaused = 0;
            InitGameState(GS_CHARACTER_SELECT);
			break;
	}
	appState.appState = _state;
}

void InitGameState(enum GAME_STATE _state)
{
	appState.stateData.gameState.playerTeam[0].items[3] = InitItem(ITEM_TEST);
	appState.stateData.gameState.playerTeam[0].stats.baseStats.currentHealth = 700;
	appState.stateData.gameState.playerTeam[0].stats.baseStats.critCounter = 40;
    switch(_state)
    {
        case GS_CHARACTER_SELECT:
        appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected = 0;
        break;

		case GS_DUNGEON:
		appState.stateData.gameState.stateData.dungeonState.selectionX = 0;
		appState.stateData.gameState.stateData.dungeonState.selectionY = 0;
		ENCOUNTER_ID* encounterSelection = SelectRandomEncounters(appState.stateData.gameState.floor);
		appState.stateData.gameState.stateData.dungeonState.encounters[0] = GetEncounterData(encounterSelection[0]);
		appState.stateData.gameState.stateData.dungeonState.encounters[1] = GetEncounterData(encounterSelection[1]);
		appState.stateData.gameState.stateData.dungeonState.encounters[2] = GetEncounterData(encounterSelection[2]);
		appState.stateData.gameState.stateData.dungeonState.rewards[0] = InitItem(ITEM_TEST);
		appState.stateData.gameState.stateData.dungeonState.rewards[1] = InitItem(ITEM_NONE);
		appState.stateData.gameState.stateData.dungeonState.rewards[2] = InitItem(ITEM_TEST);
		break;

		case GS_BATTLE:
		appState.stateData.gameState.stateData.battleState.abilitySelection = 0;
		appState.stateData.gameState.stateData.battleState.targetSelection = 0;
		appState.stateData.gameState.stateData.battleState.messages[0] = "The battle has begun!";
		appState.stateData.gameState.stateData.battleState.messages[1] = "oppan";
		appState.stateData.gameState.stateData.battleState.messages[2] = "gangnam";
		appState.stateData.gameState.stateData.battleState.messages[3] = "style";
		appState.stateData.gameState.stateData.battleState.messages[4] = "now this here is a test of a longer message to see if text wrapping works";
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

bool CheckIfHeroInParty(CHARACTER_ID id)
{
	return appState.stateData.gameState.teamCompMask & (1 << id);
}