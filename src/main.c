#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main ()
{
	const int SCREEN_WIDTH = 320;
	const int SCREEN_HEIGHT = 240;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(GetScreenWidth(), GetScreenHeight(), "Yendor Delve");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Camera2D worldSpaceCamera = { 0 };  // Game world camera
    worldSpaceCamera.zoom = 1.0f;

    Camera2D screenSpaceCamera = { 0 }; // Smoothing camera
    screenSpaceCamera.zoom = 1.0f;

    // Load render texture to draw all our objects
    RenderTexture2D renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	Rectangle sourceRec = { 0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height };
	int displayWidth = SCREEN_WIDTH * (GetScreenHeight() / SCREEN_HEIGHT);
	int wMargin = (GetScreenWidth() - displayWidth) / 2;
    Rectangle destRec = { wMargin, 0, displayWidth , GetScreenHeight() };
	Vector2 origin = { 0.0f, 0.0f };

	Texture creatureAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/creatures_transparent.png");
	Texture itemAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/items_transparent.png");
	Texture generalAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/general_transparent.png");
	Texture dungeonAtlas = LoadTexture("hexanys_roguelike_tiles/Tilesheets/Transparent/autotile_transparent.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		BeginTextureMode(renderTexture);
            ClearBackground(BLACK);
            BeginMode2D(worldSpaceCamera);
				DrawText("Yendor\n Delve", 64,64,32,MAROON);
				DrawTexture(creatureAtlas, 300, 64, GREEN);
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
