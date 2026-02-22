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
			appState.stateData.gameState.teamCompMask = 7;
			appState.stateData.gameState.playerTeam[0] = InitCharacterData(CHAR_BERSERKER);
			appState.stateData.gameState.playerTeam[1] = InitCharacterData(CHAR_ASSASSIN);
			appState.stateData.gameState.playerTeam[2] = InitCharacterData(CHAR_DUELIST);
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
    }
}

bool CheckIfHeroInParty(CHARACTER_ID id)
{
	return appState.stateData.gameState.teamCompMask & (1 << id);
}