#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const char* GAME_TITLE = "Yendor Delve";

enum APP_STATE{
	MainMenu,
	Gameplay,
};

enum GAME_STATE{
	Dungeon,
	Battle,
};

Camera2D worldSpaceCamera = { 0 };
Camera2D screenSpaceCamera = { 0 };
RenderTexture2D renderTexture;
Font titleFont;
Font basicFont;
Texture creatureAtlas;
Texture itemAtlas;
Texture generalAtlas;
Texture dungeonAtlas;

int main ()
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
	Rectangle sourceRec = { 0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height };
	int displayWidth = SCREEN_WIDTH * (GetScreenHeight() / SCREEN_HEIGHT);
	int wMargin = (GetScreenWidth() - displayWidth) / 2;
    Rectangle destRec = { wMargin, 0, displayWidth , GetScreenHeight() };
	Vector2 origin = { 0.0f, 0.0f };

    titleFont = LoadFontEx("fonts/KAISG.TTF", 64, 0, 0);
	SetTextureFilter(titleFont.texture, 0);
    basicFont = LoadFontEx("fonts/alagard.ttf", 16, 0, 0);
	SetTextureFilter(basicFont.texture, 0);

	creatureAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/creatures_transparent.png");
	itemAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/items_transparent.png");
	generalAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/general_transparent.png");
	dungeonAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/autotile_transparent.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		BeginTextureMode(renderTexture);
            ClearBackground(BLACK);
            BeginMode2D(worldSpaceCamera);
				Vector2 textSize = MeasureTextEx(titleFont, GAME_TITLE, 64, 0);
				Vector2 textPosition = {SCREEN_WIDTH/2 - textSize.x/2, 64};
				DrawTextEx(titleFont, GAME_TITLE, textPosition, 64, 0, MAROON);
            EndMode2D();
        EndTextureMode();

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

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
