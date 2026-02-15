#include "raylib.h"

#ifndef resource_dir
#define resource_dir
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#endif

#include "drawing.h"
#include "constants.h"

Camera2D worldSpaceCamera = {0};
Camera2D screenSpaceCamera = {0};
RenderTexture2D renderTexture;
Font titleFont;
Font basicFont;
Font basicFontLarger;
Texture creatureAtlas;
Texture itemAtlas;
Texture generalAtlas;
Texture dungeonAtlas;
Texture boneFrame;

bool markedToClose;
struct AppState appState;
char inputThisFrame;		// bitmask of possible inputs, see enum CONTROLS in constants.h

void InitAppState(enum APP_STATE);
void ReadInput();
bool IsPressed(enum CONTROLS);
void HandleInput();
void HandleMainMenuInput();

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(GetScreenWidth(), GetScreenHeight(), GAME_TITLE);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	worldSpaceCamera.zoom = 1.0f;
	screenSpaceCamera.zoom = 1.0f;

	// Load render texture to draw all our objects
	renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	Rectangle sourceRec = {0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height};
	int displayWidth = SCREEN_WIDTH * (GetScreenHeight() / SCREEN_HEIGHT);
	int wMargin = (GetScreenWidth() - displayWidth) / 2;
	Rectangle destRec = {wMargin, 0, displayWidth, GetScreenHeight()};
	Vector2 origin = {0.0f, 0.0f};

	titleFont = LoadFontEx("fonts/KAISG.TTF", 120, 0, 0);
	SetTextureFilter(titleFont.texture, 0);
	basicFont = LoadFontEx("fonts/alagard.ttf", 16, 0, 0);
	SetTextureFilter(basicFont.texture, 0);
	basicFontLarger = LoadFontEx("fonts/alagard.ttf", 32, 0, 0);
	SetTextureFilter(basicFontLarger.texture, 0);

	creatureAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/creatures_transparent.png");
	itemAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/items_transparent.png");
	generalAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/general_transparent.png");
	dungeonAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/autotile_transparent.png");
	boneFrame = LoadTexture("hexanys_1bit_ui/Panels/Transparent/bone_breakers.png");

	InitAppState(AS_MAIN_MENU);

	// game loop
	while (!WindowShouldClose() && !markedToClose) // run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		ReadInput();
		HandleInput();

		switch (appState.appState)
		{
		case AS_MAIN_MENU:
			DrawMainMenu();
			break;
		}

		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode2D(screenSpaceCamera);
		DrawTexturePro(renderTexture.texture, sourceRec, destRec, origin, 0.0f, WHITE);
		EndMode2D();
		EndDrawing();
	}

	// cleanup
	UnloadRenderTexture(renderTexture);
	UnloadTexture(itemAtlas);
	UnloadTexture(creatureAtlas);
	UnloadTexture(generalAtlas);
	UnloadTexture(dungeonAtlas);
	UnloadTexture(boneFrame);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void InitAppState(enum APP_STATE state)
{
	switch(state)
	{
		case AS_MAIN_MENU:
			appState.stateData.mainMenuState.currentSelection = MS_PLAY;
			break;
		case AS_GAMEPLAY:
			appState.stateData.gameState.gameState = GS_CHARACTER_SELECT;
			break;
	}
	appState.appState = state;
}

void ReadInput()
{
	inputThisFrame = 0;

	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_COMMA) 
		|| IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
	{
		inputThisFrame = inputThisFrame | VK_UP;
	}
	if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_O) 
		|| IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
	{
		inputThisFrame = inputThisFrame | VK_DOWN;
	}
	if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_Q) 
		|| IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
	{
		inputThisFrame = inputThisFrame | VK_LEFT;
	}
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_E) || IsKeyPressed(KEY_D) 
		|| IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
	{
		inputThisFrame = inputThisFrame | VK_RIGHT;
	}
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) 
		|| IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
	{
		inputThisFrame = inputThisFrame | VK_CONFIRM;
	}
	if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_BACKSPACE) 
		|| IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
	{
		inputThisFrame = inputThisFrame | VK_BACK;
	}
}

bool IsPressed(enum CONTROLS input)
{
	return (inputThisFrame & (char)input) != 0;
}

void HandleInput()
{
	switch(appState.appState)
	{
		case AS_MAIN_MENU:
			HandleMainMenuInput();
			break;
	}
}

void HandleMainMenuInput()
{
	if(IsPressed(VK_UP))
	{
		if(appState.stateData.mainMenuState.currentSelection == 0)
		{
			appState.stateData.mainMenuState.currentSelection = MS_LENGTH - 1;
		}
		else
		{
			appState.stateData.mainMenuState.currentSelection--;
		}
	}
	if(IsPressed(VK_DOWN))
	{
		if(appState.stateData.mainMenuState.currentSelection + 1 == MS_LENGTH)
		{
			appState.stateData.mainMenuState.currentSelection = 0;
		}
		else
		{
			appState.stateData.mainMenuState.currentSelection++;
		}
	}
	if(IsPressed(VK_BACK))
	{
		appState.stateData.mainMenuState.currentSelection = MS_QUIT;
	}
	if(IsPressed(VK_CONFIRM))
	{
		if (appState.stateData.mainMenuState.currentSelection == MS_QUIT)
		{
			markedToClose = true;
		}
		if (appState.stateData.mainMenuState.currentSelection == MS_PLAY)
		{
			InitAppState(AS_GAMEPLAY);
		}
	}

}