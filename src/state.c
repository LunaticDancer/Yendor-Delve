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
			appState.stateData.gameState.gameState = GS_CHARACTER_SELECT;
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
    switch(_state)
    {
        case GS_CHARACTER_SELECT:
        appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected = 0;
        break;
		case GS_DUNGEON:
		appState.stateData.gameState.stateData.dungeonState.selectionX = 0;
		appState.stateData.gameState.stateData.dungeonState.selectionY = 0;
		appState.stateData.gameState.stateData.dungeonState.encounters[0] = GetEncounterData(ENC_BONFIRE);
		appState.stateData.gameState.stateData.dungeonState.encounters[1] = GetEncounterData(ENC_NONE);
		appState.stateData.gameState.stateData.dungeonState.encounters[2] = GetEncounterData(ENC_CHEST);
		appState.stateData.gameState.stateData.dungeonState.rewards[0] = InitItem(ITEM_TEST);
		appState.stateData.gameState.stateData.dungeonState.rewards[1] = InitItem(ITEM_TEST);
		appState.stateData.gameState.stateData.dungeonState.rewards[2] = InitItem(ITEM_TEST);
		break;
    }
}

bool CheckIfHeroInParty(CHARACTER_ID id)
{
	return appState.stateData.gameState.teamCompMask & (1 << id);
}