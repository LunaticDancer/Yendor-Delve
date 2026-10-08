#ifndef drawing_h
#define drawing_h
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
void DrawBattle();
void DrawDungeonScreen();
void DrawEquipmentDialog();
void DrawItemSelection();
void DrawCharacterSelect();
Texture2D GetTileset(enum TILESET);
void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);
void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);
void DrawTextStyled(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color color);
char* WrapText(Font font, const char* text, float fontSize, float spacing, float maxWidth);

#endif