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

#endif