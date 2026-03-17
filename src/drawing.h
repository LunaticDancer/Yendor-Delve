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
void DrawDungeonScreen();
void DrawCharacterSelect();
Texture2D GetTileset(enum TILESET);
void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);
void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

#endif