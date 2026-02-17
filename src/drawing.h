#ifndef drawing
#define drawing
#include "raylib.h"
#include "constants.h"

extern const char* GAME_TITLE;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Font titleFont;
extern Font basicFont;
extern Font basicFontLarger;
extern Camera2D worldSpaceCamera;
extern RenderTexture2D renderTexture;
extern Texture boneFrame;

void DrawMainMenu();
void DrawGameplay();
void DrawCharacterSelect();
Texture2D GetTileset(enum TILESET);

#endif