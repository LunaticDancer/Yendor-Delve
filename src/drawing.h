#ifndef drawing
#define drawing
#include "raylib.h"

extern const char* GAME_TITLE;
extern const int SCREEN_WIDTH;
extern Font titleFont;
extern Font basicFont;
extern Font basicFontLarger;
extern Camera2D worldSpaceCamera;
extern RenderTexture2D renderTexture;
extern Texture boneFrame;
extern enum MENU_SELECTION menuSelection;

void DrawMainMenu();

#endif