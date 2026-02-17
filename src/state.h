#ifndef state
#define state

#include "constants.h"
#include "character.h"
#include "enemy.h"
#include "dungeon.h"

struct MainMenuState
{
	enum MENU_SELECTION currentSelection;
};

struct GameState
{
	enum GAME_STATE gameState;
	Character playerTeam[3];
};

union AppStateData
{
	struct MainMenuState mainMenuState;
	struct GameState gameState;
};

struct AppState
{
	enum APP_STATE appState;
	union AppStateData stateData;
};

#endif