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

struct CharacterSelectState
{
	char currentSlotSelected;
};

struct DungeonState
{
	char selectionX;
	char selectionY;
	Encounter encounters[3];
	Item rewards[3];
	bool isBrowsingEquipment;
	char highlightedEquipmentSlot;
	bool isSelectingItem;
	char highlightedItem;
	char* slotItemIndexes;
	char itemIndexListLength;
};

struct TurnIndicator
{
	bool isPlayer;
	char senderId;
	bool isAttack;
	char receiverMask;
};

struct BattleState
{
	Enemy enemies[3];
	Item reward;
	char currentActingEntity;
	char abilitySelection;
	char targetSelection;
	struct TurnIndicator turnIndicators[6];
	char* messages[6];
	float statePauseTimer;
	enum BATTLE_STATE battleState;
};

union GameStateData
{
	struct CharacterSelectState characterSelectState;
	struct DungeonState dungeonState;
	struct BattleState battleState;
};

struct GameState
{
	enum GAME_STATE gameState;
	char teamCompMask;
	Character playerTeam[3];
	char floor;
	bool isPaused;
	char pauseMenuSelection;
	union GameStateData stateData;
	char* message;
	float messageTimer;
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

void InitAppState(enum APP_STATE);
void InitGameState(enum GAME_STATE);
void TransitionToBattle();
void AddMessageToFeed(char*);
short DetermineCurrentActingEntity();
void ProgressTime(short ticks);
bool CheckIfHeroInParty(CHARACTER_ID);
void HandleRealTimePopups();
void ShowPopupMessage(char*);

#endif