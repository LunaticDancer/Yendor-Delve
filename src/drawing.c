#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "rlgl.h"
#include "constants.h"
#include "state.h"
#include "drawing.h"
#include "item.h"
#include "colors.h"

const int MENU_BUTTON_SPACING = 56;
const int LAYOUT_SPACING = 4;

extern const char* GAME_TITLE;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Font titleFont;
extern Font basicFont;
extern Font basicFontLarger;
extern Camera2D camera;
extern Texture creatureAtlas;
extern Texture bigCreatureAtlas;
extern Texture itemAtlas;
extern Texture generalAtlas;
extern Texture communityAtlas;
extern Texture bigCommunityAtlas;
extern Texture boneFrame;
extern Texture spikeFrame;
extern Texture ornateFrame;
extern struct AppState appState;

NPatchInfo frameInfo = {(Rectangle){0,0,96,96}, 32, 32, 32, 32, NPATCH_NINE_PATCH};

void DrawBattleAbilitySelection();
void DrawBattleAbilityDescription(CreatureStats*, ABILITY);
void DrawPrognoses();
void DrawBattleScreenPartyMember(char);
void DrawBattleScreenEnemy(char);
void DrawBattleScreenEnemyHp(char index);
void DrawDuelistOpportunitySelection();
void DrawStatusEffects(Vector2 position, CreatureStats* creature);
void DrawBattleScreenMessageFeed();
void DrawPauseMenu();
void DrawCharacterStats();
void DrawItemSelection();
void DrawItemListElement(Item it, char index);
void DrawEquipmentPanel();
void DrawEquipmentSlot(char index);
void DrawCharacterSelectOptionFrame(char index);
void DrawCharacterSelectCharacterSprite(char index);
void DrawDungeonScreenSelector();
void DrawDungeonScreenPartyMember(char index);
void DrawDungeonScreenPartyMemberFrame(char index);
void DrawDungeonScreenPartyMemberBars(char index);
void DrawDungeonEncounterText(char index);
void DrawDungeonScreenRewardSprite(char index);
void DrawDungeonScreenEncounterSprite(char index);
void DrawPopupMessage();

void DrawMainMenu()
{
    BeginMode2D(camera);

    Vector2 textSize = MeasureTextEx(titleFont, GAME_TITLE, 120, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 80};
    DrawTextEx(titleFont, GAME_TITLE, textPosition, 120, 0, *maroon);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){SCREEN_WIDTH/4, 254 + (MENU_BUTTON_SPACING * (int)appState.stateData.mainMenuState.currentSelection), 
        SCREEN_WIDTH/2, 64}, (Vector2){0,0}, 0, *gray);
    textSize = MeasureTextEx(basicFontLarger, "Play", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 272;
    DrawTextEx(basicFontLarger, "Play", textPosition, 32, 0, (appState.stateData.mainMenuState.currentSelection == MS_PLAY) ? *white : *gray);
    textSize = MeasureTextEx(basicFontLarger, "Settings", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 272 + MENU_BUTTON_SPACING;
    DrawTextEx(basicFontLarger, "Settings", textPosition, 32, 0, (appState.stateData.mainMenuState.currentSelection == MS_SETTINGS) ? *white : *gray);
    textSize = MeasureTextEx(basicFontLarger, "Quit", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 272 + MENU_BUTTON_SPACING * 2;
    DrawTextEx(basicFontLarger, "Quit", textPosition, 32, 0, (appState.stateData.mainMenuState.currentSelection == MS_QUIT) ? *white : *gray);

    textSize = MeasureTextEx(basicFontLarger, "LunaticDancer, 2026", 16, 0);
    textPosition.x = 0;
    textPosition.y = SCREEN_HEIGHT - textSize.y;
    DrawTextEx(basicFontLarger, "LunaticDancer, 2026", textPosition, 16, 0, *gray);
    textSize = MeasureTextEx(basicFontLarger, GAME_VERSION_FOOTER, 16, 0);
    textPosition.x = SCREEN_WIDTH - textSize.x;
    textPosition.y = SCREEN_HEIGHT - textSize.y;
    DrawTextEx(basicFontLarger, GAME_VERSION_FOOTER, textPosition, 16, 0, *gray);

    EndMode2D();
    return;
}

void DrawSettings()
{
    BeginMode2D(camera);
	struct SettingsState* state = (appState.appState == AS_SETTINGS) ? &appState.stateData.settingsState : &appState.stateData.gameState.settings;

    Vector2 textSize = MeasureTextEx(titleFont, "Settings", 80, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 16};
    DrawTextEx(titleFont, "Settings", textPosition, 80, 0, *lightgray);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){16, 110 + (MENU_BUTTON_SPACING * (int)(*state).currentSelection), 
        624, 64}, (Vector2){0,0}, 0, *gray);
    textPosition.x = 48;
    textPosition.y = 128;
    DrawTextEx(basicFontLarger, "Action Speed:", textPosition, 32, 0, ((*state).currentSelection == SET_ACTION_SPEED) ? *white : *gray);
    textPosition.x = 48;
    textPosition.y = 128 + MENU_BUTTON_SPACING;
    DrawTextEx(basicFontLarger, "Font:", textPosition, 32, 0, ((*state).currentSelection == SET_FONT) ? *white : *gray);
    textPosition.x = 48;
    textPosition.y = 128 + MENU_BUTTON_SPACING * 2;
    DrawTextEx(basicFontLarger, "Palette:", textPosition, 32, 0, ((*state).currentSelection == SET_PALETTE) ? *white : *gray);
    textPosition.x = 48;
    textPosition.y = 128 + MENU_BUTTON_SPACING * 3;
    DrawTextEx(basicFontLarger, "Windowed:", textPosition, 32, 0, ((*state).currentSelection == SET_WINDOWED) ? *white : *gray);
    textPosition.x = 48;
    textPosition.y = 128 + MENU_BUTTON_SPACING * 4;
    DrawTextEx(basicFontLarger, "Effects Volume:", textPosition, 32, 0, ((*state).currentSelection == SET_SFX_VOLUME) ? *white : *gray);
    textPosition.x = 48;
    textPosition.y = 128 + MENU_BUTTON_SPACING * 5;
    DrawTextEx(basicFontLarger, "Music Volume:", textPosition, 32, 0, ((*state).currentSelection == SET_MUSIC_VOLUME) ? *white : *gray);

    char* text;
    char strnum[3];
    switch(appState.settings.actionSpeed)
    {
        case 0:
        text = "Slow";
        break;
        case 1:
        text = "Regular";
        break;
        case 2:
        text = "Fast";
        break;
        case 3:
        text = "Instant";
        break;
    }
    textSize = MeasureTextEx(basicFontLarger, text, 32, 0);
    textPosition.x = 608 - textSize.x;
    textPosition.y = 128;
    DrawTextEx(basicFontLarger, text, textPosition, 32, 0, ((*state).currentSelection == SET_ACTION_SPEED) ? *white : *gray);
    switch(appState.settings.font)
    {
        case FS_FANCY:
        text = "Fancy";
        break;
        case FS_SIMPLE:
        text = "Simple";
        break;
    }
    textSize = MeasureTextEx(basicFontLarger, text, 32, 0);
    textPosition.x = 608 - textSize.x;
    textPosition.y = 128 + MENU_BUTTON_SPACING;
    DrawTextEx(basicFontLarger, text, textPosition, 32, 0, ((*state).currentSelection == SET_FONT) ? *white : *gray);
    text = (appState.settings.windowed) ? "Yes" : "No";
    textSize = MeasureTextEx(basicFontLarger, text, 32, 0);
    textPosition.x = 608 - textSize.x;
    textPosition.y = 128 + MENU_BUTTON_SPACING * 3;
    DrawTextEx(basicFontLarger, text, textPosition, 32, 0, ((*state).currentSelection == SET_WINDOWED) ? *white : *gray);
    switch(appState.settings.palette)
    {
        case CS_CLASSIC:text = "Classic";break;
        case CS_FADING:text = "Fading";break;
        case CS_GREENORANGE:text = "Green Orange";break;
        //case CS_EPHEMERA:text = "Ephemeral";break;
        case CS_NEON:text = "NEON";break;
        //case CS_DELICIOUS_BLOOD:text = "Delicious Blood";break;
        case CS_NIGHT_SKY:text = "Night Sky";break;
        //case CS_OVERGROWN_TEMPLE:text = "Overgrown Temple";break;
    }
    textSize = MeasureTextEx(basicFontLarger, text, 32, 0);
    textPosition.x = 608 - textSize.x;
    textPosition.y = 128 + MENU_BUTTON_SPACING*2;
    DrawTextEx(basicFontLarger, text, textPosition, 32, 0, ((*state).currentSelection == SET_PALETTE) ? *white : *gray);
    sprintf(strnum, "%d", appState.settings.sfxVolume);
    textSize = MeasureTextEx(basicFontLarger, strnum, 32, 0);
    textPosition.x = 608 - textSize.x;
    textPosition.y = 128 + MENU_BUTTON_SPACING * 4;
    DrawTextEx(basicFontLarger, strnum, textPosition, 32, 0, ((*state).currentSelection == SET_SFX_VOLUME) ? *white : *gray);
    sprintf(strnum, "%d", appState.settings.musicVolume);
    textSize = MeasureTextEx(basicFontLarger, strnum, 32, 0);
    textPosition.x = 608 - textSize.x;
    textPosition.y = 128 + MENU_BUTTON_SPACING * 5;
    DrawTextEx(basicFontLarger, strnum, textPosition, 32, 0, ((*state).currentSelection == SET_MUSIC_VOLUME) ? *white : *gray);

    EndMode2D();
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
        case GS_BATTLE:
            DrawBattle();
            break;
        case GS_GAME_OVER:
            DrawGameOver();
            break;
    }
    DrawPopupMessage();
}

void DrawGameOver()
{
    BeginMode2D(camera);

    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){16,16,SCREEN_WIDTH-32,SCREEN_HEIGHT-32}, (Vector2){0,0}, 0, *darkgray);
    Vector2 textSize = MeasureTextEx(titleFont, "Perished", 96, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 60};
    DrawTextEx(titleFont, "Perished", textPosition, 96, 0, *maroon);

    DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[0].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[0].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[0].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                320 - 128, 200, TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0}, 0, *gray
        );
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[1].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[1].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[1].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                320 - 16, 200, TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0}, 0, *gray
        );
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[2].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[2].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[2].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                320 + 96, 200, TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0}, 0, *gray
        );

    char* text;
    if(appState.stateData.gameState.floor == 24)
    {
        text = "The brave adventurers were slain by the Dungeon Master's hand!";
    }
    else
    {
        text = "The brave adventurers have met their untimely end somewhere ";
        if(appState.stateData.gameState.floor > 16)
        {
            text = CombineStrings(text, "in a long forgotten city of a mysterious civilization, ");
        }
        else if(appState.stateData.gameState.floor > 8)
        {
            text = CombineStrings(text, "deep within a hidden cavern system under the dungeon, ");
        }
        else 
        {
            text = CombineStrings(text, "in a decrepit, abandoned dungeon, ");
        }
        text = CombineStrings(text, "having made it ");
        switch(appState.stateData.gameState.floor)
        {
            case 1: text = CombineStrings(text, "one"); break;
            case 2: text = CombineStrings(text, "two"); break;
            case 3: text = CombineStrings(text, "three"); break;
            case 4: text = CombineStrings(text, "four"); break;
            case 5: text = CombineStrings(text, "five"); break;
            case 6: text = CombineStrings(text, "six"); break;
            case 7: text = CombineStrings(text, "seven"); break;
            case 8: text = CombineStrings(text, "eight"); break;
            case 9: text = CombineStrings(text, "nine"); break;
            case 10: text = CombineStrings(text, "ten"); break;
            case 11: text = CombineStrings(text, "eleven"); break;
            case 12: text = CombineStrings(text, "twelve"); break;
            case 13: text = CombineStrings(text, "thirteen"); break;
            case 14: text = CombineStrings(text, "fourteen"); break;
            case 15: text = CombineStrings(text, "fifteen"); break;
            case 16: text = CombineStrings(text, "sixteen"); break;
            case 17: text = CombineStrings(text, "seventeen"); break;
            case 18: text = CombineStrings(text, "eighteen"); break;
            case 19: text = CombineStrings(text, "nineteen"); break;
            case 20: text = CombineStrings(text, "twenty"); break;
            case 21: text = CombineStrings(text, "twenty one"); break;
            case 22: text = CombineStrings(text, "twenty two"); break;
            case 23: text = CombineStrings(text, "twenty three"); break;
        }
        text = CombineStrings(text, (appState.stateData.gameState.floor == 1) ? " layer deep.\nMay their souls rest." :  " layers deep.\nMay their souls rest.");
    }
    DrawTextBoxed(basicFontLarger,text,(Rectangle){64, 280, SCREEN_WIDTH-128, 240},16,0,true, *lightgray);

    EndMode2D();
    return;
}

void DrawBattle()
{
    BeginMode2D(camera);

    if(appState.stateData.gameState.isInSettings)
    {
        DrawSettings();
        EndMode2D();
        return;
    }
    if(appState.stateData.gameState.isPaused)
    {
        DrawPauseMenu();
        EndMode2D();
        return;
    }

    DrawBattleScreenMessageFeed();
    DrawPrognoses();

    DrawBattleScreenPartyMember(0);
    DrawBattleScreenPartyMember(1);
    DrawBattleScreenPartyMember(2);

    DrawBattleScreenEnemy(0);
    DrawBattleScreenEnemy(1);
    DrawBattleScreenEnemy(2);

    if(appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_ABILITY_SELECT)
    {
        DrawBattleAbilitySelection();
    }
    if(appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_TARGET_SELECT)
    {
        GetAbilityDescription(appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.battleState.currentActingEntity].stats.abilities[
            appState.stateData.gameState.stateData.battleState.verticalSelection].abilityId, &appState.stateData.gameState.playerTeam[
            appState.stateData.gameState.stateData.battleState.currentActingEntity].stats);
    }
    if(appState.stateData.gameState.stateData.battleState.battleState == BS_OPPORTUNITY_CHOICE)
    {
        DrawDuelistOpportunitySelection();
    }
    

    DrawBattleScreenEnemyHp(0);
    DrawBattleScreenEnemyHp(1);
    DrawBattleScreenEnemyHp(2);

    EndMode2D();
    return;
}

void DrawBattleAbilitySelection()
{
    CreatureStats* caster = &appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.battleState.currentActingEntity].stats;
    DrawBattleAbilityDescription(caster, caster->abilities[appState.stateData.gameState.stateData.battleState.verticalSelection].abilityId);

    short height = (*caster).abilityCount * 18;
    DrawRectangle(390, 446 - height, 245, height+29, *black);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){ 390, 446 - height, 245, height+30}, (Vector2){0,0}, 0, *white);

    for (int i = 0; i < (*caster).abilityCount; i++)
    {
        DrawTextEx(basicFont, caster->abilities[i].name, (Vector2){406, 463 - height + (i * 18)}, 16, 0, 
            (appState.stateData.gameState.stateData.battleState.verticalSelection == i) ? *white : *gray);
    }
}

void DrawBattleAbilityDescription(CreatureStats* c, ABILITY a)
{
    char* abilityDesc = GetAbilityDescription(a, c);

    DrawRectangle(152, 128, 488 - LAYOUT_SPACING, 224, *black);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){ 152, 128, 488 - LAYOUT_SPACING, 224}, (Vector2){0,0}, 0, *darkgray);

    Vector2 textSize = MeasureTextEx(basicFontLarger, InitAbility(a).name, 32, 0);
    Vector2 textPosition = {396 - textSize.x / 2, 144};
    DrawTextEx(basicFontLarger, InitAbility(a).name, textPosition, 32, 0, *lightgray);

    DrawTextBoxed(basicFont, abilityDesc, (Rectangle){184, 188, 424-LAYOUT_SPACING, 224},16,0,true, *gray);
}

void DrawPrognoses()
{
    Vector2 boxSize = (Vector2){144, 34};
    for (int i = 0; i < TURN_PROGNOSES; i++)
    {
        if(appState.stateData.gameState.stateData.battleState.horizontalSelection == i && appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_OVERVIEW)
        {
            char strnum[8];
            sprintf(strnum, "%d", appState.stateData.gameState.stateData.battleState.turnIndicators[i].ticksUntil);
            char* text = CombineStrings("Happens in\n", strnum);
            text = CombineStrings(text, " moments.");
            if(i==0) text = "Happening\nright now!";
            Vector2 textSize = MeasureTextEx(basicFont, text, 16,0);
            DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){ 428, 384, 160, 64}, 
                (Vector2){0,0}, 0, *gray);
            DrawTextEx(basicFont, text,(Vector2){508-textSize.x/2, 416 - textSize.y/2},16,0,*lightgray);
        }

        Vector2 position = (Vector2){LAYOUT_SPACING, 355 - ((i+1)*(LAYOUT_SPACING+boxSize.y))};
        DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){ position.x, position.y, boxSize.x, boxSize.y}, 
        (Vector2){0,0}, 0, 
        (appState.stateData.gameState.stateData.battleState.horizontalSelection == i && appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_OVERVIEW) 
        ? *gold : ((i == 0) ? *lightgray : *darkgray));
        if(appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId < 3)
        {
            DrawTexturePro(
                GetTileset(appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId].stats.baseStats.tileset),
                (Rectangle){
                    appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                    appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){
                    position.x + boxSize.x/2 - TILE_SIZE/2,
                    position.y + boxSize.y/2 - TILE_SIZE/2,
                    TILE_SIZE, TILE_SIZE,
                },
                (Vector2){0,0},
                0,
                *appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId].stats.baseStats.color
            );
        }
        else
        {
            if(appState.stateData.gameState.stateData.battleState.horizontalSelection == i && appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_OVERVIEW)
            {
                DrawBattleAbilityDescription(&appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats,
                    appState.stateData.gameState.stateData.battleState.turnIndicators[i].abilityId);
            }
            char numberOfTargets = 0;
            char offset = -12;
            for(int j = 0; j < 6; j++)
            {
                if((appState.stateData.gameState.stateData.battleState.turnIndicators[i].receiverMask & (1 << j))) numberOfTargets++;
            }
            offset += -8 * numberOfTargets;
            DrawTexturePro(         // draw caster
                GetTileset(appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.tileset),
                (Rectangle){
                    appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.tileLookupPosition.x * TILE_SIZE 
                    * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.tileset) ? 2 : 1),
                    appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.tileLookupPosition.y * TILE_SIZE 
                    * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.tileset) ? 2 : 1),
                    TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.tileset) ? 2 : 1), 
                    TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.tileset) ? 2 : 1),
                },
                (Rectangle){
                    position.x + boxSize.x/2 + offset - TILE_SIZE/2,
                    position.y + boxSize.y/2 - TILE_SIZE/2,
                    TILE_SIZE, TILE_SIZE,
                },
                (Vector2){0,0},
                0,
                *appState.stateData.gameState.stateData.battleState.enemies[appState.stateData.gameState.stateData.battleState.turnIndicators[i].senderId-3].stats.baseStats.color
            );
            offset += 20;
            DrawTexturePro(         // draw action icon
                GetTileset(TL_ITEMS),
                (Rectangle){
                    appState.stateData.gameState.stateData.battleState.turnIndicators[i].isAttack ? 0 : 2 * TILE_SIZE,
                    appState.stateData.gameState.stateData.battleState.turnIndicators[i].isAttack ? 1 : 3 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){
                    position.x + boxSize.x/2 + offset - TILE_SIZE/2,
                    position.y + boxSize.y/2 - TILE_SIZE/2,
                    TILE_SIZE, TILE_SIZE,
                },
                (Vector2){0,0},
                0,
                appState.stateData.gameState.stateData.battleState.turnIndicators[i].isAttack ? *red : *green
            );
            offset += 20;

            for(int j = 0; j < 6; j++)
            {
                if(!(appState.stateData.gameState.stateData.battleState.turnIndicators[i].receiverMask & (1 << j))) continue;
                if(j<3)
                {
                    DrawTexturePro(
                        GetTileset(appState.stateData.gameState.playerTeam[j].stats.baseStats.tileset),
                        (Rectangle){
                            appState.stateData.gameState.playerTeam[j].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                            appState.stateData.gameState.playerTeam[j].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                            TILE_SIZE, TILE_SIZE,
                        },
                        (Rectangle){
                            position.x + boxSize.x/2 + offset - TILE_SIZE/2,
                            position.y + boxSize.y/2 - TILE_SIZE/2,
                            TILE_SIZE, TILE_SIZE,
                        },
                        (Vector2){0,0},
                        0,
                        *appState.stateData.gameState.playerTeam[j].stats.baseStats.color
                    );
                }
                else
                {
                    DrawTexturePro(
                        GetTileset(appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.tileset),
                        (Rectangle){
                            appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.tileLookupPosition.x * TILE_SIZE 
                    * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.tileset) ? 2 : 1),
                            appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.tileLookupPosition.y * TILE_SIZE 
                    * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.tileset) ? 2 : 1),
                            TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.tileset) ? 2 : 1), 
                            TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.tileset) ? 2 : 1),
                        },
                        (Rectangle){
                            position.x + boxSize.x/2 + offset - TILE_SIZE/2,
                            position.y + boxSize.y/2 - TILE_SIZE/2,
                            TILE_SIZE, TILE_SIZE,
                        },
                        (Vector2){0,0},
                        0,
                        *appState.stateData.gameState.stateData.battleState.enemies[j-3].stats.baseStats.color
                    );
                }
                offset += 16;
            }
        }
    }
}

void DrawBattleScreenPartyMember(char index)
{
    short creatureBoxSize = 128;
    DrawRectangle(LAYOUT_SPACING + creatureBoxSize * index, SCREEN_HEIGHT - creatureBoxSize + LAYOUT_SPACING,
        creatureBoxSize - 2 * LAYOUT_SPACING, creatureBoxSize - 2 * LAYOUT_SPACING, *black);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + creatureBoxSize * index, SCREEN_HEIGHT - creatureBoxSize + LAYOUT_SPACING,
        creatureBoxSize - 2 * LAYOUT_SPACING, creatureBoxSize - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
            ((appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_TARGET_SELECT 
                && appState.stateData.gameState.stateData.battleState.horizontalSelection == index)) ? *red : 
                (appState.stateData.gameState.stateData.battleState.currentActingEntity == index) ? *white : *darkgray);

    if(!(appState.stateData.gameState.stateData.battleState.battleState == BS_SHOW_ABILITY_VFX 
        && (appState.stateData.gameState.stateData.battleState.flickeringMask & (1<<index)) 
        && ((fmodf(appState.stateData.gameState.stateData.battleState.statePauseTimer, CHARACTER_BLINK_INTERVAL)) < (CHARACTER_BLINK_INTERVAL/2))))
    {
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[index].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[index].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[index].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                (creatureBoxSize / 2) + (creatureBoxSize * index) - TILE_SIZE,
                SCREEN_HEIGHT - (creatureBoxSize / 2) - TILE_SIZE,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            (appState.stateData.gameState.playerTeam[index].stats.baseStats.currentHealth > 0) ? *appState.stateData.gameState.playerTeam[index].stats.baseStats.color : *darkgray
        );
    }
    
        //                                                              DEBUG INFO SETUP
        // char str_num[6];
        // sprintf(str_num, "%d", appState.stateData.gameState.playerTeam[index].stats.baseStats.ticksUntilNextTurn);
        // DrawText(str_num, LAYOUT_SPACING + creatureBoxSize * index + 16, SCREEN_HEIGHT - creatureBoxSize + LAYOUT_SPACING + 16, 16, PINK);
    

    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 441, creatureBoxSize-40, 2, *darkgray);
    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 440, 
        (int)((creatureBoxSize-40) * ClampFloat((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.currentHealth / 
        ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.maxHealth + (float)appState.stateData.gameState.playerTeam[index].stats.itemStats.health), 0, 1)), 
        4, *red);
        DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 440, 
        (int)((creatureBoxSize-40) * ClampFloat((float)appState.stateData.gameState.playerTeam[index].stats.encounterStats.shield / 
        ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.maxHealth + (float)appState.stateData.gameState.playerTeam[index].stats.itemStats.health), 0, 1)), 
        4,*skyblue);

    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 449, creatureBoxSize-40, 2, *darkgray);
    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 448, 
        (int)((creatureBoxSize-40) * ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.currentStamina / 
        ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.maxStamina + (float)appState.stateData.gameState.playerTeam[index].stats.itemStats.stamina + (float)appState.stateData.gameState.playerTeam[index].stats.encounterStats.stamina))), 
        4, *darkgreen);

    if((appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_ABILITY_SELECT || appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_TARGET_SELECT) && appState.stateData.gameState.stateData.battleState.currentActingEntity == index)
    {
        short cost = appState.stateData.gameState.playerTeam[index].stats.abilities[appState.stateData.gameState.stateData.battleState.verticalSelection].staminaCost
            + appState.stateData.gameState.playerTeam[index].stats.statusEffects[SE_EXHAUSTION];

        if(appState.stateData.gameState.playerTeam[index].stats.baseStats.currentStamina < cost)
        {
            DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 448, 
                (int)((creatureBoxSize-40) * ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.currentStamina / 
                ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.maxStamina + (float)appState.stateData.gameState.playerTeam[index].stats.itemStats.stamina + (float)appState.stateData.gameState.playerTeam[index].stats.encounterStats.stamina))), 
                4, *maroon);
        }
        else
        {
            float width = (float)cost / ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.maxStamina + (float)appState.stateData.gameState.playerTeam[index].stats.itemStats.stamina + (float)appState.stateData.gameState.playerTeam[index].stats.encounterStats.stamina);
            short offset = (short)(((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.currentStamina / 
                ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.maxStamina + 
                (float)appState.stateData.gameState.playerTeam[index].stats.itemStats.stamina + 
                (float)appState.stateData.gameState.playerTeam[index].stats.encounterStats.stamina) - width) * (float)(creatureBoxSize-40));

                DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize + offset, 448, (short)(width * (float)(creatureBoxSize-40))+1, 4, *lightgray);
                DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize + offset, 448, 1, 4, *black);
        }
    }

    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 457, creatureBoxSize-40, 2, *darkgray);
    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 456, 
        (int)((creatureBoxSize-40) * ClampFloat((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.critCounter / 
        (float)CRIT_PROGRESS_MAX, 0, 1)), 
        4, *yellow);
    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 456, 
        (int)((creatureBoxSize-40) * ClampFloat((float)(appState.stateData.gameState.playerTeam[index].stats.baseStats.critCounter-CRIT_PROGRESS_MAX) / 
        (float)CRIT_PROGRESS_MAX, 0, 1)), 
        4, *orange);
    DrawRectangle(LAYOUT_SPACING + 16 + index * creatureBoxSize, 456, 
        (int)((creatureBoxSize-40) * ClampFloat((float)(appState.stateData.gameState.playerTeam[index].stats.baseStats.critCounter - CRIT_PROGRESS_MAX - CRIT_PROGRESS_MAX) / 
        (float)CRIT_PROGRESS_MAX, 0, 1)), 
        4, *red);
    
    DrawStatusEffects((Vector2){LAYOUT_SPACING + creatureBoxSize * index + 16, SCREEN_HEIGHT - creatureBoxSize + LAYOUT_SPACING + 16}, &appState.stateData.gameState.playerTeam[index].stats);
}

void DrawBattleScreenEnemyHp(char index)
{    
    if(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth <= 0) return;

    short creatureBoxSize = 128;
    char strnum[8];
    sprintf(strnum, "%d", appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth
        +appState.stateData.gameState.stateData.battleState.enemies[index].stats.encounterStats.shield);
    Vector2 str_size = MeasureTextEx(basicFontLarger, strnum, 16,0);
    DrawTextEx(basicFontLarger, strnum, (Vector2){SCREEN_WIDTH - (index+1) * creatureBoxSize + creatureBoxSize/2 - str_size.x/2 + 1, 103-str_size.y/2 + 1}, 16,0,*black);
    DrawTextEx(basicFontLarger, strnum, (Vector2){SCREEN_WIDTH - (index+1) * creatureBoxSize + creatureBoxSize/2 - str_size.x/2 - 1, 103-str_size.y/2 - 1}, 16,0,*black);
    DrawTextEx(basicFontLarger, strnum, (Vector2){SCREEN_WIDTH - (index+1) * creatureBoxSize + creatureBoxSize/2 - str_size.x/2, 103-str_size.y/2}, 16,0,*white);
    
}

void DrawBattleScreenEnemy(char index)
{
    short creatureBoxSize = 128;
    DrawRectangle(SCREEN_WIDTH + LAYOUT_SPACING - creatureBoxSize * (index+1), LAYOUT_SPACING,
        creatureBoxSize - 2 * LAYOUT_SPACING, creatureBoxSize - 2 * LAYOUT_SPACING, *black);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){ SCREEN_WIDTH + LAYOUT_SPACING - creatureBoxSize * (index+1), LAYOUT_SPACING,
        creatureBoxSize - 2 * LAYOUT_SPACING, creatureBoxSize - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.battleState.currentActingEntity == index+3) ? *white :
            ((appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_TARGET_SELECT 
                && appState.stateData.gameState.stateData.battleState.horizontalSelection == index+3)) ? *red : *darkgray);

        if(!(appState.stateData.gameState.stateData.battleState.battleState == BS_SHOW_ABILITY_VFX 
        && (appState.stateData.gameState.stateData.battleState.flickeringMask & (1<<(index+3))) 
        && ((fmodf(appState.stateData.gameState.stateData.battleState.statePauseTimer, CHARACTER_BLINK_INTERVAL)) < (CHARACTER_BLINK_INTERVAL/2))))
        {
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileLookupPosition.x * TILE_SIZE * ((IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset)) ? 2 : 1),
                appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileLookupPosition.y * TILE_SIZE * ((IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset)) ? 2 : 1),
                TILE_SIZE * ((IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset)) ? 2 : 1), 
                TILE_SIZE * ((IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset)) ? 2 : 1),
            },
            (Rectangle){
                SCREEN_WIDTH - (creatureBoxSize / 2) - (creatureBoxSize * index) - TILE_SIZE - ((IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset)) ? TILE_SIZE : 0),
                (creatureBoxSize / 2) - TILE_SIZE - ((IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset)) ? TILE_SIZE + 8 : 0),
                TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset) ? 4 : 2), 
                TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.tileset) ? 4 : 2),
            },
            (Vector2){0,0},
            0,
            (appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth > 0) ? 
                *appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.color : *darkgray
        );
    }

        //                                                              DEBUG INFO SETUP
        // char str_num[6];
        // sprintf(str_num, "%d", appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.ticksUntilNextTurn);
        // DrawText(str_num, SCREEN_WIDTH + LAYOUT_SPACING - creatureBoxSize * (index+1) + 16, LAYOUT_SPACING + 16, 16, PINK);
    
    if(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth > 0)
    {
    DrawRectangle(SCREEN_WIDTH + LAYOUT_SPACING + 16 - (index+1) * creatureBoxSize, 102, creatureBoxSize-40, 2, *darkgray);
    DrawRectangle(SCREEN_WIDTH + LAYOUT_SPACING + 16 - (index+1) * creatureBoxSize, 101, 
        (int)((creatureBoxSize-40) * ((float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth / 
        (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.maxHealth)), 
        4, *red);
    DrawRectangle(SCREEN_WIDTH + LAYOUT_SPACING + 16 - (index+1) * creatureBoxSize, 101, 
        (int)((creatureBoxSize-40) * ClampFloat((float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.encounterStats.shield / 
        (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.maxHealth, 0, 1)), 
        4, *skyblue);
    }
    DrawStatusEffects((Vector2){SCREEN_WIDTH + LAYOUT_SPACING - creatureBoxSize * (index+1) + 16, LAYOUT_SPACING + 16}, &appState.stateData.gameState.stateData.battleState.enemies[index].stats);
}

void DrawEnemyDamageOverlay(char index, short value)
{
    if(!(appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_ABILITY_SELECT || appState.stateData.gameState.stateData.battleState.battleState == BS_PLAYER_TARGET_SELECT)) return;
    if(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth < 0) return;
    short creatureBoxSize = 128;
    if(appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth < value)
        {
            DrawRectangle(SCREEN_WIDTH + LAYOUT_SPACING + 16 - (index+1) * creatureBoxSize, 101, 
                (int)((creatureBoxSize-40) * ((float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth / 
                ((float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.maxHealth + (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.itemStats.health + (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.encounterStats.health))), 
                4, *darkgray);
        }
        else
        {
            float width = (float)(value - appState.stateData.gameState.stateData.battleState.enemies[index].stats.encounterStats.shield) / ((float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.maxHealth + 
                (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.itemStats.health + (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.encounterStats.health);
            short offset = (short)(((float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.currentHealth / 
                ((float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.baseStats.maxHealth + 
                (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.itemStats.health + 
                (float)appState.stateData.gameState.stateData.battleState.enemies[index].stats.encounterStats.health) - width) * (float)(creatureBoxSize-40));

                DrawRectangle(SCREEN_WIDTH + LAYOUT_SPACING + 16 - (index+1) * creatureBoxSize + offset, 101, (short)(width * (float)(creatureBoxSize-40))+1, 4, *gray);
                DrawRectangle(SCREEN_WIDTH + LAYOUT_SPACING + 16 - (index+1) * creatureBoxSize + offset, 101, 1, 4, *black);
        }
}

void ShowEnemyDamageOverlays(short damage, bool trueDamage)
{
    DrawEnemyDamageOverlay(0, trueDamage ? damage : CalculateDamage(damage, &appState.stateData.gameState.stateData.battleState.enemies[0].stats));
    DrawEnemyDamageOverlay(1, trueDamage ? damage : CalculateDamage(damage, &appState.stateData.gameState.stateData.battleState.enemies[1].stats));
    DrawEnemyDamageOverlay(2, trueDamage ? damage : CalculateDamage(damage, &appState.stateData.gameState.stateData.battleState.enemies[2].stats));
}

void DrawDuelistOpportunitySelection()
{
    char strnum[8];
    short height = OPPORTUNITY_MAX_TURNS * 18 + 48;
    DrawRectangle(284, 240 - height/2, 72, height, *black);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){284, 240 - height/2, 72, height}, (Vector2){0,0}, 0, *white);

    for (int i = 0; i < OPPORTUNITY_MAX_TURNS; i++)
    {
        sprintf(strnum, "%d", i+1);
        DrawTextEx(basicFont, strnum, (Vector2){316, 240 - height/2 + 24 + (i * 18)}, 16, 0, 
            (appState.stateData.gameState.stateData.battleState.horizontalSelection == i) ? *white : *gray);
    }
}

void DrawStatusEffects(Vector2 position, CreatureStats* _creature)
{
    if(_creature->baseStats.currentHealth <= 0) return;

    char strnum[8];
    int hOffset = 0;
    for(int i = 0; i < SE_LENGTH; i++)
    {
        if (_creature->statusEffects[i] <= 0) continue;

        sprintf(strnum, "%d", _creature->statusEffects[i]);
        DrawTextEx(basicFontLarger, strnum, (Vector2){position.x+21, position.y+hOffset+1}, 16,0, *black);
        DrawTextEx(basicFontLarger, strnum, (Vector2){position.x+19, position.y+hOffset-1}, 16,0, *black);
        DrawTextEx(basicFontLarger, strnum, (Vector2){position.x+20, position.y+hOffset}, 16,0, *white);
        switch(i)
        {
            case SE_PROTECTION:
            DrawTexturePro(
                GetTileset(TL_ITEMS),
                (Rectangle){
                    8 * TILE_SIZE,
                    2 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){position.x, position.y+hOffset, TILE_SIZE, TILE_SIZE, },
                (Vector2){0,0}, 0, *skyblue
            );
            break;
            case SE_BERSERK:
            DrawTexturePro(
                GetTileset(TL_CREATURES),
                (Rectangle){
                    13 * TILE_SIZE,
                    6 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){position.x, position.y+hOffset, TILE_SIZE, TILE_SIZE, },
                (Vector2){0,0}, 0, *maroon
            );
            break;
            case SE_BLEED:
            DrawTexturePro(
                GetTileset(TL_ITEMS),
                (Rectangle){
                    3 * TILE_SIZE,
                    3 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){position.x, position.y+hOffset, TILE_SIZE, TILE_SIZE, },
                (Vector2){0,0}, 0, *red
            );
            break;
            case SE_EXHAUSTION:
            DrawTexturePro(
                GetTileset(TL_ITEMS),
                (Rectangle){
                    20 * TILE_SIZE,
                    3 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){position.x, position.y+hOffset, TILE_SIZE, TILE_SIZE, },
                (Vector2){0,0}, 0, *gray
            );
            break;
            case SE_UNTARGETABLE:
            DrawTexturePro(
                GetTileset(TL_CREATURES),
                (Rectangle){
                    4 * TILE_SIZE,
                    6 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){position.x, position.y+hOffset, TILE_SIZE, TILE_SIZE, },
                (Vector2){0,0}, 0, *darkgray
            );
            break;
            case SE_PAIN:
            DrawTexturePro(
                GetTileset(TL_GENERAL),
                (Rectangle){
                    31 * TILE_SIZE,
                    1 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){position.x, position.y+hOffset, TILE_SIZE, TILE_SIZE, },
                (Vector2){0,0}, 0, *brown
            );
            break;
            case SE_CONFUSION:
            DrawTexturePro(
                GetTileset(TL_ITEMS),
                (Rectangle){
                    4 * TILE_SIZE,
                    3 * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE,
                },
                (Rectangle){position.x, position.y+hOffset, TILE_SIZE, TILE_SIZE, },
                (Vector2){0,0}, 0, *purple
            );
            break;
        }
        hOffset += 18;
    }
}

void DrawBattleScreenMessageFeed()
{
    char* result = malloc(1);
     result[0] =  '\0'; 

     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[11]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[10]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[9]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[8]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[7]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[6]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[5]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[4]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[3]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[2]);
    result = CombineStrings(result, "\n");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[1]);
    result = CombineStrings(result, "\n");
    result = CombineStrings(result, "[cffffffff]");
     result = CombineStrings(result, appState.stateData.gameState.stateData.battleState.messages[0]);
    result = CombineStrings(result, "[r]");

    result = WrapText(basicFontLarger, result, 16, 0, 488-LAYOUT_SPACING);
    Vector2 textSize = MeasureTextEx(basicFontLarger, result, 16,0);

    DrawTextStyled(basicFontLarger, result, (Vector2){160,368 - textSize.y},16,0, *gray);
    free(result);
}

void DrawDungeonScreen()
{
    BeginMode2D(camera);

    if(appState.stateData.gameState.isInSettings)
    {
        DrawSettings();
        EndMode2D();
        return;
    }
    if(appState.stateData.gameState.isPaused)
    {
        DrawPauseMenu();
        EndMode2D();
        return;
    }

    Vector2 textSize = MeasureTextEx(basicFontLarger, "Prepare for an encounter:", 32, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 10};
    DrawTextEx(basicFontLarger, "Prepare for an encounter:", textPosition, 32, 0, *darkgray);

    // encounter selection frames
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING + 5, 48 + LAYOUT_SPACING,
        (210) - 2 * LAYOUT_SPACING, (312) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        ((appState.stateData.gameState.stateData.dungeonState.selectionX == 0 || appState.stateData.gameState.stateData.dungeonState.selectionX == 1)
             && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) ? *gold : *gray);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING +  + 215, 48 + LAYOUT_SPACING,
        (210) - 2 * LAYOUT_SPACING, (312) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        ((appState.stateData.gameState.stateData.dungeonState.selectionX == 2 || appState.stateData.gameState.stateData.dungeonState.selectionX == 3)
            && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) ? *gold : *gray);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING+ 425, 48 + LAYOUT_SPACING,
        (210) - 2 * LAYOUT_SPACING, (312) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        ((appState.stateData.gameState.stateData.dungeonState.selectionX == 4 || appState.stateData.gameState.stateData.dungeonState.selectionX == 5)
            && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) ? *gold : *gray);

    DrawDungeonScreenSelector();

    DrawDungeonScreenEncounterSprite(0);
    DrawDungeonScreenEncounterSprite(1);
    DrawDungeonScreenEncounterSprite(2);

    DrawDungeonScreenRewardSprite(0);
    DrawDungeonScreenRewardSprite(1);
    DrawDungeonScreenRewardSprite(2);

    DrawDungeonEncounterText(0);
    DrawDungeonEncounterText(1);
    DrawDungeonEncounterText(2);

    DrawDungeonScreenPartyMemberFrame(0);
    DrawDungeonScreenPartyMemberFrame(1);
    DrawDungeonScreenPartyMemberFrame(2);

    DrawDungeonScreenPartyMember(0);
    DrawDungeonScreenPartyMember(1);
    DrawDungeonScreenPartyMember(2);

    DrawDungeonScreenPartyMemberBars(0);
    DrawDungeonScreenPartyMemberBars(1);
    DrawDungeonScreenPartyMemberBars(2);

    if(appState.stateData.gameState.stateData.dungeonState.isBrowsingEquipment)
    {
        DrawEquipmentPanel();
        DrawCharacterStats();
        if(appState.stateData.gameState.stateData.dungeonState.isSelectingItem)
        {
            DrawItemSelection();
        }
    }

    EndMode2D();
    return;
}

void DrawPauseMenu()
{
    Vector2 textSize = MeasureTextEx(titleFont, "PAUSED", 80, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 96};
    DrawTextEx(titleFont, "PAUSED", textPosition, 80, 0, *gray);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){SCREEN_WIDTH/4, 222 + (MENU_BUTTON_SPACING * appState.stateData.gameState.pauseMenuSelection), 
        SCREEN_WIDTH/2, 64}, (Vector2){0,0}, 0, *gray);
    textSize = MeasureTextEx(basicFontLarger, "Resume", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 240;
    DrawTextEx(basicFontLarger, "Resume", textPosition, 32, 0, (appState.stateData.gameState.pauseMenuSelection == 0) ? *white : *gray);
    textSize = MeasureTextEx(basicFontLarger, "Settings", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 240 + MENU_BUTTON_SPACING;
    DrawTextEx(basicFontLarger, "Settings", textPosition, 32, 0, (appState.stateData.gameState.pauseMenuSelection == 1) ? *white : *gray);
    textSize = MeasureTextEx(basicFontLarger, "Abandon", 32, 0);
    textPosition.x = SCREEN_WIDTH / 2 - textSize.x / 2;
    textPosition.y = 240 + MENU_BUTTON_SPACING * 2;
    DrawTextEx(basicFontLarger, "Abandon", textPosition, 32, 0, (appState.stateData.gameState.pauseMenuSelection == 2) ? *white : *gray);
}

void DrawCharacterStats()
{
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){320, 48 + LAYOUT_SPACING, 320-LAYOUT_SPACING, 304}, (Vector2){0,0}, 0, *gray);

    char* name = appState.stateData.gameState.playerTeam[
        appState.stateData.gameState.stateData.dungeonState.selectionX
        ].stats.baseStats.name;
    Vector2 textSize = MeasureTextEx(basicFontLarger, name, 16, 0);
    Vector2 textPosition = (Vector2){480 - textSize.x / 2, 64 + LAYOUT_SPACING};
    DrawTextEx(basicFontLarger, name, textPosition, 16, 0, *white);

    char* text = GetCharacterStatsRundown(appState.stateData.gameState.playerTeam[
        appState.stateData.gameState.stateData.dungeonState.selectionX
    ]);
    DrawTextBoxed(basicFontLarger, text, (Rectangle){336, 88+LAYOUT_SPACING, 304-LAYOUT_SPACING, 360}, 16, 0, true, *gray);
}

void DrawItemSelection()
{
    Item none = InitItem(ITEM_NONE);
    short listH = 50 + appState.stateData.gameState.stateData.dungeonState.itemIndexListLength * 18;

    DrawRectangle(LAYOUT_SPACING + 434, 48 + LAYOUT_SPACING, 200, 304, *black);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){434 + LAYOUT_SPACING, 48 + LAYOUT_SPACING, 200, 304}, (Vector2){0,0}, 0, *gray);

    char* statDiff = GetItemStatDiff(
        appState.stateData.gameState.playerTeam[
        appState.stateData.gameState.stateData.dungeonState.selectionX
    ].items[appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot],
    (appState.stateData.gameState.stateData.dungeonState.highlightedItem == 0) ? none : appState.stateData.gameState.inventory[
        appState.stateData.gameState.stateData.dungeonState.slotItemIndexes[appState.stateData.gameState.stateData.dungeonState.highlightedItem - 1]]
    );
    DrawTextBoxed(basicFont, statDiff, (Rectangle){LAYOUT_SPACING + 450, 64+LAYOUT_SPACING, 182, 360}, 16, 0, true, *lightgray);

    DrawRectangle(LAYOUT_SPACING + 230, 48 + LAYOUT_SPACING, 208, listH, *black);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){230 + LAYOUT_SPACING, 48 + LAYOUT_SPACING, 208, listH}, (Vector2){0,0}, 0, *white);

    DrawItemListElement(none, 0);
    for(int i = 0; i < appState.stateData.gameState.stateData.dungeonState.itemIndexListLength; i++)
    {
        DrawItemListElement(appState.stateData.gameState.inventory[appState.stateData.gameState.stateData.dungeonState.slotItemIndexes[i]], i+1);
    }
}

void DrawItemListElement(Item it, char index)
{
    DrawTexturePro(
            GetTileset(it.tileset),
            (Rectangle){
                it.tileLookupPosition.x * TILE_SIZE,
                it.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 244,
                 64 + LAYOUT_SPACING + (18*index),
                16, 16
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.highlightedItem == index ? *it.color : *gray
        );
    DrawTextEx(basicFont, it.name, (Vector2){LAYOUT_SPACING + 262, 64 + LAYOUT_SPACING + (18*index)}, 16, 0,
        appState.stateData.gameState.stateData.dungeonState.highlightedItem == index ? *white : *gray);
}

void DrawEquipmentPanel()
{
    DrawRectangle(LAYOUT_SPACING + 5, 48 + LAYOUT_SPACING,
        (630) - 2 * LAYOUT_SPACING, (312) - 2 * LAYOUT_SPACING, *black);
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + 5, 48 + LAYOUT_SPACING, 256, 256}, (Vector2){0,0}, 0,
    appState.stateData.gameState.stateData.dungeonState.isSelectingItem ? *gray : *white);

    Vector2 textSize = MeasureTextEx(basicFont, "Equipment", 16, 0);
    Vector2 textPosition = (Vector2){122 - textSize.x / 2, 80};
    DrawTextEx(basicFont, "Equipment", textPosition, 16, 0, *white);

    DrawEquipmentSlot(0);
    DrawEquipmentSlot(1);
    DrawEquipmentSlot(2);
    DrawEquipmentSlot(3);
}

void DrawEquipmentSlot(char index)
{
    char* text;
    switch(index + 1)
    {
        case ES_HEAD:
        text = "Head";
        break;
        case ES_TORSO:
        text = "Torso";
        break;
        case ES_LEGS:
        text = "Legs";
        break;
        case ES_WEAPON:
        text = "Weapon";
        break;
    }

    DrawTextEx(basicFont, text, (Vector2){80, 128 + index*40}, 16, 0,
    appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot == index ? *gray : *darkgray);
    DrawTextEx(basicFont, appState.stateData.gameState.playerTeam[
        appState.stateData.gameState.stateData.dungeonState.selectionX
    ].items[index].name, (Vector2){80, 144 + index*40}, 16, 0,
    appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot == index ? *white : *gray);

    DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[
                appState.stateData.gameState.stateData.dungeonState.selectionX
            ].items[index].tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[
                    appState.stateData.gameState.stateData.dungeonState.selectionX
                ].items[index].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[
                    appState.stateData.gameState.stateData.dungeonState.selectionX
                ].items[index].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                32,
                 128 + (40*index),
                32, 32
            },
            (Vector2){0,0},
            0,
            appState.stateData.gameState.stateData.dungeonState.highlightedEquipmentSlot == index ? *appState.stateData.gameState.playerTeam[
                appState.stateData.gameState.stateData.dungeonState.selectionX].items[index].color : *gray
        );
}

void DrawDungeonScreenEncounterSprite(char index)
{
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.encounters[index].tileLookupPosition.x * TILE_SIZE 
                * (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 2 : 1),
                appState.stateData.gameState.stateData.dungeonState.encounters[index].tileLookupPosition.y * TILE_SIZE 
                * (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 2 : 1),
                TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 2 : 1), 
                TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 2 : 1),
            },
            (Rectangle){
                LAYOUT_SPACING + 67 + 210 * index - (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 8 : 0),
                 LAYOUT_SPACING + 96 - (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 8 : 0),
                TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 2 : 1), 
                TILE_SIZE * (IsBigSpritesheet(appState.stateData.gameState.stateData.dungeonState.encounters[index].tileset) ? 2 : 1),
            },
            (Vector2){0,0},
            0,
            *appState.stateData.gameState.stateData.dungeonState.encounters[index].color
        );
}

void DrawDungeonScreenRewardSprite(char index)
{
    if(appState.stateData.gameState.stateData.dungeonState.encounters[index].hidesLoot)
    {
        DrawTextEx(basicFont, "?", (Vector2){LAYOUT_SPACING + (210 * index) + 132,  LAYOUT_SPACING + 100}, 16, 0, *gray);
    }
    else
    {
        DrawTexturePro(
            GetTileset(appState.stateData.gameState.stateData.dungeonState.rewards[index].tileset),
            (Rectangle){
                appState.stateData.gameState.stateData.dungeonState.rewards[index].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.stateData.dungeonState.rewards[index].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                -LAYOUT_SPACING + 137 + (210*index),
                 LAYOUT_SPACING + 96,
                TILE_SIZE, TILE_SIZE,
            },
            (Vector2){0,0},
            0,
            *appState.stateData.gameState.stateData.dungeonState.rewards[index].color
        );
    }
}

void DrawDungeonEncounterText(char index)
{
    char* text =(appState.stateData.gameState.stateData.dungeonState.selectionX == (index * 2) + 1 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? (appState.stateData.gameState.stateData.dungeonState.encounters[index].hidesLoot ? "Unknown" :
         appState.stateData.gameState.stateData.dungeonState.rewards[index].name) : appState.stateData.gameState.stateData.dungeonState.encounters[index].name;
    Vector2 textSize = MeasureTextEx(basicFont, text, 16, 0);
    Vector2 textPosition = (Vector2){(210 * index) + 110 - textSize.x / 2, 140 + LAYOUT_SPACING};
    DrawTextEx(basicFont, text, textPosition, 16, 0, *white);
    text = (appState.stateData.gameState.stateData.dungeonState.selectionX == (index * 2) + 1 && appState.stateData.gameState.stateData.dungeonState.selectionY == 0) 
        ? (appState.stateData.gameState.stateData.dungeonState.encounters[index].hidesLoot ? "You will only learn what this reward is after you obtain it." :
        GetItemStatSpread(appState.stateData.gameState.stateData.dungeonState.rewards[index])) : appState.stateData.gameState.stateData.dungeonState.encounters[index].description;
    DrawTextBoxed(basicFont, text, (Rectangle){(210*index)+21+LAYOUT_SPACING, 168+LAYOUT_SPACING, 174, 360}, 16, 0, true, *gray);
}

void DrawDungeonScreenPartyMemberFrame(char index)
{
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + (SCREEN_WIDTH / 8) + 160 * index, SCREEN_HEIGHT - (SCREEN_HEIGHT  / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_HEIGHT / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.dungeonState.selectionX == index && appState.stateData.gameState.stateData.dungeonState.selectionY == 1) ? *gold : *gray);
}

void DrawDungeonScreenPartyMember(char index)
{
        DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[index].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[index].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[index].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 138 + (160 * index),
                SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 48,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            *appState.stateData.gameState.playerTeam[index].stats.baseStats.color
        );
}

void DrawDungeonScreenPartyMemberBars(char index)
{
    for(int i = 0; i < 4; i++)
    {
    DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[index].items[i].tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[index].items[i].tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[index].items[i].tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                108 + index * 160 + i * 28,
                 380,
                16, 16,
            },
            (Vector2){0,0},
            0,
            *appState.stateData.gameState.playerTeam[index].items[i].color
        );
    }

    DrawRectangle(104 + index * 160, 449, 112, 2, *darkgray);
    DrawRectangle(104 + index * 160, 448, 
        (int)(112 * ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.currentHealth / 
        ((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.maxHealth + (float)appState.stateData.gameState.playerTeam[index].stats.itemStats.health))), 
        4, *red);

    DrawRectangle(104 + index * 160, 457, 112, 2, *darkgray);
    DrawRectangle(104 + index * 160, 456, 
        (int)(112 * ClampFloat((float)appState.stateData.gameState.playerTeam[index].stats.baseStats.critCounter / 
        (float)CRIT_PROGRESS_MAX, 0, 1)), 
        4, *yellow);
}

void DrawDungeonScreenSelector()
{
    if(appState.stateData.gameState.stateData.dungeonState.selectionY == 0)
    {
        switch(appState.stateData.gameState.stateData.dungeonState.selectionX)
        {
            case 0:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){54, 87 , 49, 45}, (Vector2){0,0}, 0, *red);
            break;
            case 1:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){115, 87 , 49, 45}, (Vector2){0,0}, 0, *red);
            break;
            case 2:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){54 + 210, 87 , 49, 45}, (Vector2){0,0}, 0, *red);
            break;
            case 3:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){115 + 210, 87 , 49, 45}, (Vector2){0,0}, 0, *red);
            break;
            case 4:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){54 + 420, 87 , 49, 45}, (Vector2){0,0}, 0, *red);
            break;
            case 5:
            DrawTextureNPatch(spikeFrame, frameInfo, (Rectangle){115 + 420, 87 , 49, 45}, (Vector2){0,0}, 0, *red);
            break;
        }
    }
}

void DrawCharacterSelect()
{
    BeginMode2D(camera);

    Vector2 textSize = MeasureTextEx(basicFontLarger, "Create your party:", 32, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, 10};
    DrawTextEx(basicFontLarger, "Create your party:", textPosition, 32, 0, *darkgray);

    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){LAYOUT_SPACING, 48 + LAYOUT_SPACING, 
        SCREEN_WIDTH - 2 * LAYOUT_SPACING, SCREEN_HEIGHT - (SCREEN_WIDTH / 4) - (48 + 2 * LAYOUT_SPACING)}, (Vector2){0,0}, 0, *gray);

    DrawCharacterSelectOptionFrame(0);
    DrawCharacterSelectOptionFrame(1);
    DrawCharacterSelectOptionFrame(2);
    DrawCharacterSelectOptionFrame(3);

    char* name = (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 3) ?
        "Descent" : appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected].stats.baseStats.name;
    textSize = MeasureTextEx(basicFontLarger, name, 32, 0);
    textPosition = (Vector2){SCREEN_WIDTH / 2 - textSize.x / 2, 72 + LAYOUT_SPACING};
    DrawTextEx(basicFontLarger, name, textPosition, 32, 0, *white);
    
    char* description = (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == 3) ?
        "Press Confirm to delve into the dungeons of Yendor..." : 
        appState.stateData.gameState.playerTeam[appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected].description;
    DrawTextBoxed(basicFontLarger, description, (Rectangle){24+LAYOUT_SPACING, 128+LAYOUT_SPACING, 592, 360}, 16, 0, true, *lightgray);

    DrawCharacterSelectCharacterSprite(0);
    DrawCharacterSelectCharacterSprite(1);
    DrawCharacterSelectCharacterSprite(2);
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
            *brown
        );

    EndMode2D();
    return;
}

void DrawCharacterSelectCharacterSprite(char index)
{
        DrawTexturePro(
            GetTileset(appState.stateData.gameState.playerTeam[index].stats.baseStats.tileset),
            (Rectangle){
                appState.stateData.gameState.playerTeam[index].stats.baseStats.tileLookupPosition.x * TILE_SIZE,
                appState.stateData.gameState.playerTeam[index].stats.baseStats.tileLookupPosition.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE,
            },
            (Rectangle){
                LAYOUT_SPACING + 60 + (160 * index),
                SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING + 64,
                TILE_SIZE * 2, TILE_SIZE * 2,
            },
            (Vector2){0,0},
            0,
            *appState.stateData.gameState.playerTeam[index].stats.baseStats.color
        );
}

void DrawCharacterSelectOptionFrame(char index)
{
    DrawTextureNPatch(ornateFrame, frameInfo, (Rectangle){LAYOUT_SPACING + (160 * index), SCREEN_HEIGHT - (SCREEN_WIDTH / 4) + LAYOUT_SPACING,
        (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING, (SCREEN_WIDTH / 4) - 2 * LAYOUT_SPACING}, (Vector2){0,0}, 0,
        (appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == index) ? *gold : *gray);
    if(appState.stateData.gameState.stateData.characterSelectState.currentSlotSelected == index && index!=3)
    {
        DrawTexturePro(GetTileset(TL_COMMUNITY), (Rectangle){16,96,16,16},(Rectangle){160*index+72,360, 16,16},(Vector2){0,0},0,*darkgray);
        DrawTexturePro(GetTileset(TL_COMMUNITY), (Rectangle){0,96,16,16},(Rectangle){160*index+72,434, 16,16},(Vector2){0,0},0,*darkgray);
    }
}

void DrawPopupMessage()
{
    if(appState.stateData.gameState.messageTimer <= 0) return;

    BeginMode2D(camera);

    Vector2 textSize = MeasureTextEx(basicFontLarger, appState.stateData.gameState.message, 16, 0);
    Vector2 textPosition = {SCREEN_WIDTH / 2 - textSize.x / 2, SCREEN_HEIGHT / 2 - textSize.y / 2};
    DrawRectangle(textPosition.x-32, textPosition.y - 32, textSize.x+64, textSize.y+64, *black);
    DrawTextureNPatch(boneFrame, frameInfo, (Rectangle){textPosition.x-32, textPosition.y - 32, textSize.x+64, textSize.y+64}, (Vector2){0,0}, 0, *lightgray);
    DrawTextEx(basicFontLarger, appState.stateData.gameState.message, textPosition, 16, 0, *white);

    EndMode2D();
}

Texture GetTileset(enum TILESET ts)
{
    switch(ts)
    {
        case TL_CREATURES:
            return creatureAtlas;
            break;
        case TL_BIG_CREATURES:
            return bigCreatureAtlas;
            break;
        case TL_ITEMS:
            return itemAtlas;
            break;
        case TL_GENERAL:
            return generalAtlas;
            break;
        case TL_COMMUNITY:
            return communityAtlas;
            break;
        case TL_BIG_COMMUNITY:
            return bigCommunityAtlas;
            break;
    }
}

void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, *white, *white);
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

void DrawTextStyled(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color color)
{
    // Text inline styling strategy used: [ ] delimiters for format
    // - Define foreground color:      [cRRGGBBAA]
    // - Define background color:      [bRRGGBBAA]
    // - Reset formating:              [r]
    // Example: [bAA00AAFF][cFF0000FF]red text on gray background[r] normal text

    if (font.texture.id == 0) font = GetFontDefault();

    int textLen = TextLength(text);

    Color colFront = color;
    Color colBack = BLANK;
    int backRecPadding = 4; // Background rectangle padding

    float textOffsetY = 0.0f;
    float textOffsetX = 0.0f;
    float textLineSpacing = 0.0f;
    float scaleFactor = fontSize/font.baseSize;

    for (int i = 0; i < textLen;)
    {
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);

        if (codepoint == '\n')
        {
            textOffsetY += (fontSize + textLineSpacing);
            textOffsetX = 0.0f;
        }
        else
        {
            if (codepoint == '[') // Process pipe styling
            {
                if (((i + 2) < textLen) && (text[i + 1] == 'r') && (text[i + 2] == ']')) // Reset styling
                {
                    colFront = color;
                    colBack = BLANK;

                    i += 3;     // Skip "[r]"
                    continue;   // Do not draw characters
                }
                else if (((i + 1) < textLen) && ((text[i + 1] == 'c') || (text[i + 1] == 'b')))
                {
                    i += 2;     // Skip "[c" or "[b" to start parsing color

                    // Parse following color
                    char colHexText[9] = { 0 };
                    const char *textPtr = &text[i]; // Color should start here, let's see...

                    int colHexCount = 0;
                    while ((textPtr != NULL) && (textPtr[colHexCount] != '\0') && (textPtr[colHexCount] != ']'))
                    {
                        if (((textPtr[colHexCount] >= '0') && (textPtr[colHexCount] <= '9')) ||
                            ((textPtr[colHexCount] >= 'A') && (textPtr[colHexCount] <= 'F')) ||
                            ((textPtr[colHexCount] >= 'a') && (textPtr[colHexCount] <= 'f')))
                        {
                            colHexText[colHexCount] = textPtr[colHexCount];
                            colHexCount++;
                        }
                        else break; // Only affects while loop
                    }

                    // Convert hex color text into actual Color
                    unsigned int colHexValue = strtoul(colHexText, NULL, 16);
                    if (text[i - 1] == 'c')
                    {
						colFront = GetColor(colHexValue);
						//colFront.a *= (unsigned char)(colFront.a*(float)color.a/255.0f); // TODO: Review
					}
                    else if (text[i - 1] == 'b')
					{
						colBack = GetColor(colHexValue);
						//colBack.a *= (unsigned char)(colFront.a*(float)color.a/255.0f);
					}

                    i += (colHexCount + 1); // Skip color value retrieved and ']'
                    continue;   // Do not draw characters
                }
            }

            int index = GetGlyphIndex(font, codepoint);
            float increaseX = 0.0f;

            if (font.glyphs[index].advanceX == 0) increaseX = ((float)font.recs[index].width*scaleFactor + spacing);
            else increaseX += ((float)font.glyphs[index].advanceX*scaleFactor + spacing);

            // Draw background rectangle color (if required)
            if (colBack.a > 0) DrawRectangleRec((Rectangle) { position.x + textOffsetX, position.y + textOffsetY - backRecPadding, increaseX, fontSize + 2*backRecPadding }, colBack);

            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint(font, codepoint, (Vector2){ position.x + textOffsetX, position.y + textOffsetY }, fontSize, colFront);
            }

            textOffsetX += increaseX;
        }

        i += codepointByteCount;
    }
}

char* WrapText(Font font, const char* text, float fontSize, float spacing, float maxWidth)
{
    if (!text) return NULL;

    int length = TextLength(text);
    // Worst case: a '\n' after every character. Allocate generously.
    char* result = (char*)malloc(length * 2 + 1);
    if (!result) return NULL;

    int outLen = 0; 
    float lineWidth = 0.0f;
    float spaceWidth = MeasureTextEx(font, " ", fontSize, spacing).x;

    int   lastBreakOut = -1;
    float widthAtBreak = 0.0f;

    const char* p = text;
    while (*p)
    {
        int cpByteCount = 0;
        int codepoint = GetCodepoint(p, &cpByteCount);
        if (cpByteCount <= 0) cpByteCount = 1;

        char glyph[8] = {0};
        memcpy(glyph, p, cpByteCount < 7 ? cpByteCount : 7);
        float glyphWidth = MeasureTextEx(font, glyph, fontSize, spacing).x;

        bool isSpace = (codepoint == ' ' || codepoint == '\t');
        bool isNewline = (codepoint == '\n');

        if (isNewline)
        {
            result[outLen++] = '\n';
            lineWidth = 0.0f;
            lastBreakOut = -1;
            p += cpByteCount;
            continue;
        }

        float advance = isSpace ? spaceWidth : glyphWidth;
        if (lineWidth + advance > maxWidth && lineWidth > 0.0f)
        {
            if (isSpace)
            {
                result[outLen++] = '\n';
                lineWidth = 0.0f;
                lastBreakOut = -1;
                p += cpByteCount;
                continue;
            }
            else if (lastBreakOut >= 0)
            {
                result[lastBreakOut] = '\n';
                lineWidth = 0.0f;
                for (int i = lastBreakOut + 1; i < outLen; )
                {
                    int bc = 0;
                    GetCodepoint(&result[i], &bc);
                    if (bc <= 0) bc = 1;
                    char g[8] = {0};
                    memcpy(g, &result[i], bc < 7 ? bc : 7);
                    lineWidth += MeasureTextEx(font, g, fontSize, spacing).x;
                    i += bc;
                }
                lineWidth += glyphWidth; 
                lastBreakOut = -1;
            }
        }

        memcpy(&result[outLen], p, cpByteCount);
        outLen += cpByteCount;
        lineWidth += advance;

        if (isSpace)
        {
            lastBreakOut = outLen - cpByteCount;
            widthAtBreak = lineWidth;
        }

        p += cpByteCount;
    }

    result[outLen] = '\0';
    (void)widthAtBreak; 
    return result;
}

bool IsBigSpritesheet(enum TILESET tileset) {
    switch (tileset) {
        case TL_BIG_COMMUNITY:
            return true;
            break;
        case TL_BIG_CREATURES:
            return true;
            break;
        default:
            return false;
            break;
    }
}