#ifndef constants
#define constants

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int TILE_SIZE;
extern const char *GAME_TITLE;

enum APP_STATE
{
	MainMenu,
	Gameplay,
};

enum GAME_STATE
{
	Dungeon,
	Battle,
	CharacterSelect,
};

enum MENU_SELECTION
{
	Play,
	Quit,
	LENGTH,
};

enum CONTROLS
{
	Up = 1,
	Down = 2,
	Left = 4,
	Right = 8,
	Confirm = 16,
	Back = 32,
};

struct MainMenuState
{
	enum MENU_SELECTION CurrentSelection;
};

struct GameState
{
	enum GAME_STATE GameState;
};

union AppStateData
{
	struct MainMenuState mainMenuState;
	struct GameState gameState;
};

struct AppState
{
	enum APP_STATE AppState;
	union AppStateData StateData;
};

#endif