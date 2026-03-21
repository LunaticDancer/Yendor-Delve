#include "raylib.h"
#include "constants.h"
#include "state.h"
#include "drawing.h"
#include "item.h"

const int MENU_BUTTON_SPACING = 56;
const int LAYOUT_SPACING = 4;

extern const char* GAME_TITLE;
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
        case GS_DUNGEON:
            DrawDungeonScreen();
            break;
    }
}

void DrawDungeonScreen()
{
    BeginTextureMode(renderTexture);
    ClearBackground(BLACK);
    BeginMode2D(worldSpaceCamera);

    Vector2 textSize = MeasureTextEx(basicFontLarger, "Prepare for an encounter:", 32, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 10};
    DrawTextEx(basicFontLarger, "Prepare for an encounter:", textPosition, 32, 0, DARKGRAY);

    // encounter selection frames
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING + 5, 48 + LAYOUT_SPACING,
        (210) - 2 * LAYOUT_SPACING, (312) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        ((appState.stateData.gameState.stateData.dungeonState.selectionX == 0 || appState.stateData.gameState.stateData.dungeonState.selectionX == 1)
             && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) ? GOLD : GRAY);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING +  + 215, 48 + LAYOUT_SPACING,
        (210) - 2 * LAYOUT_SPACING, (312) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        ((appState.stateData.gameState.stateData.dungeonState.selectionX == 2 || appState.stateData.gameState.stateData.dungeonState.selectionX == 3)
            && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) ? GOLD : GRAY);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING+ 425, 48 + LAYOUT_SPACING,
        (210) - 2 * LAYOUT_SPACING, (312) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        ((appState.stateData.gameState.stateData.dungeonState.selectionX == 4 || appState.stateData.gameState.stateData.dungeonState.selectionX == 5)
            && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) ? GOLD : GRAY);

    // info selection
    if(appState.stateData.gameState.stateData.dungeonState.selectionY == 0)
    {
        switch(appState.stateData.gameState.stateData.dungeonState.selectionX)
        {
            case 0:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){54, 87 , 49, 45}, (Vector2){0,0}, 0, RED);
            break;
            case 1:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){115, 87 , 49, 45}, (Vector2){0,0}, 0, RED);
            break;
            case 2:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){54 + 210, 87 , 49, 45}, (Vector2){0,0}, 0, RED);
            break;
            case 3:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){115 + 210, 87 , 49, 45}, (Vector2){0,0}, 0, RED);
            break;
            case 4:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){54 + 420, 87 , 49, 45}, (Vector2){0,0}, 0, RED);
            break;
            case 5:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){115 + 420, 87 , 49, 45}, (Vector2){0,0}, 0, RED);
            break;
        }
    }

    // encounter icons
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.encounters[0].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.encounters[0].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.stateData.dungeonState.encounters[0].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 67,
                 LAYOUT_SPACING + 96,
                TILE_SIZE, TILE_SIZE,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.encounters[0].color
        );
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.encounters[1].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.encounters[1].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.stateData.dungeonState.encounters[1].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 67 + 210,
                 LAYOUT_SPACING + 96,
                TILE_SIZE, TILE_SIZE,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.encounters[1].color
        );
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.encounters[2].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.encounters[2].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.stateData.dungeonState.encounters[2].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 67 + 420,
                 LAYOUT_SPACING + 96,
                TILE_SIZE, TILE_SIZE,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.encounters[2].color
        );

    // reward icons
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.rewards[0].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.rewards[0].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.stateData.dungeonState.rewards[0].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                -LAYOUT_SPACING + 137,
                 LAYOUT_SPACING + 96,
                TILE_SIZE, TILE_SIZE,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.rewards[0].color
        );
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.rewards[1].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.rewards[1].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.stateData.dungeonState.rewards[1].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                -LAYOUT_SPACING + 137 + 210,
                 LAYOUT_SPACING + 96,
                TILE_SIZE, TILE_SIZE,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.rewards[1].color
        );
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.rewards[2].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.rewards[2].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.stateData.dungeonState.rewards[2].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                -LAYOUT_SPACING + 137 + 420,
                 LAYOUT_SPACING + 96,
                TILE_SIZE, TILE_SIZE,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.rewards[2].color
        );

    // descriptions
    char* text;

    text =(appState.stateData.gameState.stateData.dungeonState.selectionX == 1 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? appState.stateData.gameState.stateData.dungeonState.rewards[0].name : appState.stateData.gameState.stateData.dungeonState.encounters[0].name;
    textSize = MeasureTextEx(basicFont, text, 16, 0);
    textPosition = (Vector2){110 - textSize.x / 2, 140 + LAYOUT_SPACING};
    DrawTextEx(basicFont, text, textPosition, 16, 0, WHITE);
    text = (appState.stateData.gameState.stateData.dungeonState.selectionX == 1 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? GetItemStatSpread(appState.stateData.gameState.stateData.dungeonState.rewards[0]) : appState.stateData.gameState.stateData.dungeonState.encounters[0].description;
    DrawTextBoxed(basicFontLarger, text, (Rectangle){21+LAYOUT_SPACING, 168+LAYOUT_SPACING, 174, 360}, 16, 0, true, GRAY);

    text =(appState.stateData.gameState.stateData.dungeonState.selectionX == 3 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? appState.stateData.gameState.stateData.dungeonState.rewards[1].name : appState.stateData.gameState.stateData.dungeonState.encounters[1].name;
    textSize = MeasureTextEx(basicFont, text, 16, 0);
    textPosition = (Vector2){210 + 110 - textSize.x / 2, 140 + LAYOUT_SPACING};
    DrawTextEx(basicFont, text, textPosition, 16, 0, WHITE);
    text = (appState.stateData.gameState.stateData.dungeonState.selectionX == 3 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? GetItemStatSpread(appState.stateData.gameState.stateData.dungeonState.rewards[1]) : appState.stateData.gameState.stateData.dungeonState.encounters[1].description;
    DrawTextBoxed(basicFontLarger, text, (Rectangle){210+21+LAYOUT_SPACING, 168+LAYOUT_SPACING, 174, 360}, 16, 0, true, GRAY);

    text =(appState.stateData.gameState.stateData.dungeonState.selectionX == 5 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? appState.stateData.gameState.stateData.dungeonState.rewards[2].name : appState.stateData.gameState.stateData.dungeonState.encounters[2].name;
    textSize = MeasureTextEx(basicFont, text, 16, 0);
    textPosition = (Vector2){420 + 110 - textSize.x / 2, 140 + LAYOUT_SPACING};
    DrawTextEx(basicFont, text, textPosition, 16, 0, WHITE);
    text = (appState.stateData.gameState.stateData.dungeonState.selectionX == 5 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? GetItemStatSpread(appState.stateData.gameState.stateData.dungeonState.rewards[2]) : appState.stateData.gameState.stateData.dungeonState.encounters[2].description;
    DrawTextBoxed(basicFontLarger, text, (Rectangle){420+21+LAYOUT_SPACING, 168+LAYOUT_SPACING, 174, 360}, 16, 0, true, GRAY);

    // party member frames
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + (SCREEN_WIDTH / 8), SCREEN_HEIGHT - (SCREEN_HEIGHT  / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_HEIGHT  / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.dungeonState.selectionX == 0 && appState.stateData.gameState.stateData.dungeonState.selectionY == 1) ? GOLD : GRAY);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + (SCREEN_WIDTH / 8) + 160, SCREEN_HEIGHT - (SCREEN_HEIGHT  / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_HEIGHT / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.dungeonState.selectionX == 1 && appState.stateData.gameState.stateData.dungeonState.selectionY == 1) ? GOLD : GRAY);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + (SCREEN_WIDTH / 8) + 320, SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_HEIGHT / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.dungeonState.selectionX == 2 && appState.stateData.gameState.stateData.dungeonState.selectionY == 1) ? GOLD : GRAY);

    // party members
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[0].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[0].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[0].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 138,
                SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + LAYOUT_SPACING + 44,
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
                LAYOUT_SPACING + 138 + 160,
                SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + LAYOUT_SPACING + 44,
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
                LAYOUT_SPACING + 138 + 320,
                SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + LAYOUT_SPACING + 44,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.playerTeam[2].stats.baseStats.color
        );

    EndMode2D();
    EndTextureMode();
    return;
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
        "Descent" : appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected].stats.baseStats.name;
    textSize = MeasureTextEx(basicFontLarger, name, 32, 0);
    textPosition = (Vector2){SCREEN_WIDTH / 2 - textSize.x / 2, 72 + LAYOUT_SPACING};
    DrawTextEx(basicFontLarger, name, textPosition, 32, 0, WHITE);
    
    char* description = (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 3) ?
        "Press Confirm to delve into the dungeons of Yendor..." : 
        appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected].description;
    DrawTextBoxed(basicFontLarger, description, (Rectangle){24+LAYOUT_SPACING, 128+LAYOUT_SPACING, 592, 360}, 16, 0, true, LIGHTGRAY);

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

void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

// Draw text using font inside rectangle limits with support for text selection
void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
{
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0;          // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int _state = wordWrap? MEASURE_STATE : DRAW_STATE;

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width*scaleFactor : font.glyphs[index].advanceX*scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container)
        // When wordWrap is OFF we don't need the measure _state so we go to the drawing _state immediately
        // and begin drawing on the next line before we can get outside the container
        if (_state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1)? i : endLine;
                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                _state = !_state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                _state = !_state;
            }
            else if (codepoint == '\n') _state = !_state;

            if (_state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += (font.baseSize + (float)font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + (float)font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize*scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec((Rectangle){ rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth, (float)font.baseSize*scaleFactor }, selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font, codepoint, (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY }, fontSize, isGlyphSelected? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += (font.baseSize + (float)font.baseSize/2)*scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                _state = !_state;
            }
        }

        if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
    }
}