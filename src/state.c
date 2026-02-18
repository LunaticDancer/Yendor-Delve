#include "state.h"

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
			appState.stateData.gameState.playerTeam[0] = InitCharacterData(CHAR_FLESH_GOLEM);
			appState.stateData.gameState.playerTeam[1] = InitCharacterData(CHAR_DUELIST);
			appState.stateData.gameState.playerTeam[2] = InitCharacterData(CHAR_BERSERKER);
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
        appState.stateData.gameState.stateData.characterSelectState.selectedCharactersMask = 0;
        break;
    }
}