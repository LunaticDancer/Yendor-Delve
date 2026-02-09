#include "raylib.h"
#include "constants.h"

const int MENU_BUTTON_SPACING = 56;

extern const char *GAME_TITLE;
extern const int SCREEN_WIDTH;
extern Font titleFont;
extern Font basicFont;
extern Font basicFontLarger;
extern Camera2D worldSpaceCamera;
extern RenderTexture2D renderTexture;
extern Texture boneFrame;
extern enum MENU_SELECTION menuSelection;

NPatchInfo boneFrameInfo = {(Rectangle){0,0,96,96}, 32, 32, 32, 32, NPATCH_NINE_PATCH};

void DrawMainMenu()
{
    BeginTextureMode(renderTexture);
    ClearBackground(BLACK);
    BeginMode2D(worldSpaceCamera);

    Vector2 textSize = MeasureTextEx(titleFont, GAME_TITLE, 120, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 80};
    DrawTextEx(titleFont, GAME_TITLE, textPosition, 120, 0, MAROON);
    DrawTextureNPatch(boneFrame, boneFrameInfo, (Rectangle){SCREEN_WIDTH/4, 256, SCREEN_WIDTH/2, 64}, (Vector2){0,0}, 0, GRAY);
    textSize = MeasureTextEx(basicFontLarger, "Play", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 272;
    DrawTextEx(basicFontLarger, "Play", textPosition, 32, 0, (menuSelection == Play) ? WHITE : GRAY);
    textSize = MeasureTextEx(basicFontLarger, "Quit", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 272 + MENU_BUTTON_SPACING;
    DrawTextEx(basicFontLarger, "Quit", textPosition, 32, 0, (menuSelection == Quit) ? WHITE : GRAY);

    textSize = MeasureTextEx(basicFontLarger, "LunaticDancer, 2026", 16, 0);
    textPosition.x = 0;
    textPosition.y = SCREEN_HEIGHT - textSize.y;
    DrawTextEx(basicFontLarger, "LunaticDancer, 2026", textPosition, 16, 0, GRAY);
    textSize = MeasureTextEx(basicFontLarger, "made with C and Raylib", 16, 0);
    textPosition.x = SCREEN_WIDTH - textSize.x;
    textPosition.y = SCREEN_HEIGHT - textSize.y;
    DrawTextEx(basicFontLarger, "made with C and Raylib", textPosition, 16, 0, GRAY);

    EndMode2D();
    EndTextureMode();
    return;
}