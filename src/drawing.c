#include "raylib.h"
#include "constants.h"
#include "state.h"
#include "drawing.h"

const int MENU_BUTTON_SPACING = 56;
const int LAYOUT_SPACING = 4;

extern const char *GAME_TITLE;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Font titleFont;
extern Font basicFont;
extern Font basicFontLarger;
extern Camera2D worldSpaceCamera;
extern RenderTexture2D renderTexture;
extern Texture creatureAtlas;
extern Texture itemAtlas;
extern Texture generalAtlas;
extern Texture boneFrame;
extern Texture spikeFrame;
extern Texture ornateFrame;
extern struct AppState appState;

NPatchInfo frameInfo = {(Rectangle){0,0,96,96}, 32, 32, 32, 32, NPATCH_NINE_PATCH};

void DrawMainMenu()
{
    BeginTextureMode(renderTexture);
    ClearBackground(BLACK);
    BeginMode2D(worldSpaceCamera);

    Vector2 textSize = MeasureTextEx(titleFont, GAME_TITLE, 120, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 80};
    DrawTextEx(titleFont, GAME_TITLE, textPosition, 120, 0, MAROON);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){SCREEN_WIDTH/4, 254 + (MENU_BUTTON_SPACING * (int)appState.stateData.mainMenuState.currentSelection), 
        SCREEN_WIDTH/2, 64}, (Vector2){0,0}, 0, GRAY);
    textSize = MeasureTextEx(basicFontLarger, "Play", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 272;
    DrawTextEx(basicFontLarger, "Play", textPosition, 32, 0, (appState.stateData.mainMenuState.currentSelection == MS_PLAY) ? WHITE : GRAY);
    textSize = MeasureTextEx(basicFontLarger, "Quit", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 272 + MENU_BUTTON_SPACING;
    DrawTextEx(basicFontLarger, "Quit", textPosition, 32, 0, (appState.stateData.mainMenuState.currentSelection == MS_QUIT) ? WHITE : GRAY);

    textSize = MeasureTextEx(basicFontLarger, "LunaticDancer, 2026", 16, 0);
    textPosition.x = 0;
    textPosition.y = SCREEN_HEIGHT - textSize.y;
    DrawTextEx(basicFontLarger, "LunaticDancer, 2026", textPosition, 16, 0, GRAY);
    textSize = MeasureTextEx(basicFontLarger, "v0.0.1, made with C and Raylib", 16, 0);
    textPosition.x = SCREEN_WIDTH - textSize.x;
    textPosition.y = SCREEN_HEIGHT - textSize.y;
    DrawTextEx(basicFontLarger, "v0.0.1, made with C and Raylib", textPosition, 16, 0, GRAY);

    EndMode2D();
    EndTextureMode();
    return;
}

void DrawGameplay()
{
    switch(appState.stateData.gameState.gameState)
    {
        case GS_CHARACTER_SELECT:
            DrawCharacterSelect();
            break;
    }
}

void DrawCharacterSelect()
{
    BeginTextureMode(renderTexture);
    ClearBackground(BLACK);
    BeginMode2D(worldSpaceCamera);

    Vector2 textSize = MeasureTextEx(basicFontLarger, "Create your party:", 32, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 10};
    DrawTextEx(basicFontLarger, "Create your party:", textPosition, 32, 0, DARKGRAY);

    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING, 48 + LAYOUT_SPACING, 
        SCREEN_WIDTH - 2 * LAYOUT_SPACING, SCREEN_HEIGHT - (SCREEN_WIDTH / 4) - (48 + 2 * LAYOUT_SPACING)}, (Vector2){0,0}, 0, GRAY);

    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING, SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 0) ? GOLD : GRAY);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + 160, SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 1) ? GOLD : GRAY);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + 320, SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 2) ? GOLD : GRAY);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + 480, SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 3) ? GOLD : GRAY);

    char* name = (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 3) ?
        "Descent" : appState.stateData.gameState.playerTeam[0].stats.baseStats.name;
    textSize = MeasureTextEx(basicFontLarger, name, 32, 0);
    textPosition = (Vector2){SCREEN_WIDTH / 2 - textSize.x / 2, 72 + LAYOUT_SPACING};
    DrawTextEx(basicFontLarger, name, textPosition, 32, 0, WHITE);
    
    char* description = (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 3) ?
        "Press Confirm to delve into the dungeons of Yendor..." : appState.stateData.gameState.playerTeam[0].description;
    DrawTextEx(basicFontLarger, description, (Vector2){24+LAYOUT_SPACING, 128+LAYOUT_SPACING}, 16, 0, LIGHTGRAY);

        DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[0].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[0].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[0].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 60,
                SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING + 64,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.playerTeam[0].stats.baseStats.color
        );
        DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[1].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[1].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[1].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 60 + 160,
                SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING + 64,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.playerTeam[1].stats.baseStats.color
        );
        DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[2].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[2].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[2].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 60 + 320,
                SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING + 64,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.playerTeam[2].stats.baseStats.color
        );
        DrawTexturePro(
            GetTileset(TL_GENERAL),
            (Rectangle){
                2 * TILE_SIZE,
                0 * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 60 + 480,
                SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING + 64,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            BROWN
        );

    EndMode2D();
    EndTextureMode();
    return;
}

Texture GetTileset(enum TILESET ts)
{
    switch(ts)
    {
        case TL_CREATURES:
            return creatureAtlas;
            break;
        case TL_ITEMS:
            return itemAtlas;
            break;
        case TL_GENERAL:
            return generalAtlas;
            break;
    }
}