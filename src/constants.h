#ifndef constants
#define constants

#define ITEM_SLOTS 4
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int TILE_SIZE;
extern const char *GAME_TITLE;
extern const int PAUSE_MENU_OPTION_COUNT;
extern const char CRIT_PROGRESS_MAX;
extern const float GAME_MESSAGE_DISPLAY_TIME;
extern const float GAME_MESSAGE_DISPLAY_TIME_PER_CHARACTER;

enum TILESET
{
	TL_CREATURES,
	TL_ITEMS,
	TL_GENERAL,
};

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

enum BATTLE_STATE
{
	BS_PLAYER_ABILITY_SELECT,
	BS_PLAYER_OVERVIEW,
	BS_ENEMY_TURN,
	BS_SHOW_ABILITY_VFX,
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

// UTILITY FUNCTIONS
char* CombineStrings(char* str1, char* str2);

#endif