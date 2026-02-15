#ifndef constants
#define constants

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int TILE_SIZE;
extern const char *GAME_TITLE;

enum APP_STATE
{
	AS_MAIN_MENU,
	AS_GAMEPLAY,
};

enum GAME_STATE
{
	GS_DUNGEON,
	GS_BATTLE,
	GS_CHARACTER_SELECT,
};

enum MENU_SELECTION
{
	MS_PLAY,
	MS_QUIT,
	MS_LENGTH,
};

enum CONTROLS
{
	VK_UP = 1,
	VK_DOWN = 2,
	VK_LEFT = 4,
	VK_RIGHT = 8,
	VK_CONFIRM = 16,
	VK_BACK = 32,
};

struct MainMenuState
{
	enum MENU_SELECTION currentSelection;
};

struct GameState
{
	enum GAME_STATE gameState;
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