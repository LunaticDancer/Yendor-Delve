#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

// variables
Color color_lightgray = LIGHTGRAY;
Color color_gray = GRAY;
Color color_darkgray = DARKGRAY;
Color color_yellow = YELLOW;
Color color_gold = GOLD;
Color color_orange = ORANGE;
Color color_red = RED;
Color color_maroon = MAROON;
Color color_green = GREEN;
Color color_lime = LIME;
Color color_darkgreen = DARKGREEN;
Color color_skyblue = SKYBLUE;
Color color_blue = BLUE;
Color color_darkblue = DARKBLUE;
Color color_purple = PURPLE;
Color color_violet = VIOLET;
Color color_darkpurple = DARKPURPLE;
Color color_beige = BEIGE;
Color color_brown = BROWN;
Color color_darkbrown = DARKBROWN;
Color color_white = WHITE;
Color color_black = BLACK;
Color color_magenta = MAGENTA;
Color color_raywhite = RAYWHITE;

// pointers
Color *lightgray = &color_lightgray;
Color *gray = &color_gray;
Color *darkgray = &color_darkgray;
Color *yellow = &color_yellow;
Color *gold = &color_gold;
Color *orange = &color_orange;
Color *red = &color_red;
Color *maroon = &color_maroon;
Color *green = &color_green;
Color *lime = &color_lime;
Color *darkgreen = &color_darkgreen;
Color *skyblue = &color_skyblue;
Color *blue = &color_blue;
Color *darkblue = &color_darkblue;
Color *purple = &color_purple;
Color *violet = &color_violet;
Color *darkpurple = &color_darkpurple;
Color *beige = &color_beige;
Color *brown = &color_brown;
Color *darkbrown = &color_darkbrown;
Color *white = &color_white;
Color *black = &color_black;
Color *magenta = &color_magenta;
Color *raywhite = &color_raywhite;


void set_palette(int palette_index)
{
    Palette_Values use_palette;
    use_palette = get_palette(palette_index);


    color_lightgray =   use_palette.lightgray;
    color_gray =        use_palette.gray;
    color_darkgray =    use_palette.darkgray;
    color_yellow =      use_palette.yellow;
    color_gold =        use_palette.gold;
    color_orange =      use_palette.orange;
    color_red =         use_palette.red;
    color_maroon =      use_palette.maroon;
    color_green =       use_palette.green;
    color_lime =        use_palette.lime;
    color_darkgreen =   use_palette.darkgreen;
    color_skyblue =     use_palette.skyblue;
    color_blue =        use_palette.blue;
    color_darkblue =    use_palette.darkblue;
    color_purple =      use_palette.purple;
    color_violet =      use_palette.violet;
    color_darkpurple =  use_palette.darkpurple;
    color_beige =       use_palette.beige;
    color_brown =       use_palette.brown;
    color_darkbrown =   use_palette.darkbrown;
    color_white =       use_palette.white;
    color_black =       use_palette.black;
    color_magenta =     use_palette.magenta;
    color_raywhite =    use_palette.raywhite;

}


Palette_Values get_palette(int palette_index)
{
    Palette_Values result;
    switch (palette_index)
    {
    case CS_FADING:
    return (Palette_Values){
        CLITERAL(Color){140,124,121,255},// LIGHTGRAY,
        CLITERAL(Color){125,123,98,255},// GRAY,
        CLITERAL(Color){100,83,85,255},// DARKGRAY,
        CLITERAL(Color){221,207,153,255},// YELLOW,
        CLITERAL(Color){204,168,123,255},// GOLD
        CLITERAL(Color){185,122,96,255},//ORANGE,
        CLITERAL(Color){156,82,78,255},// RED,
        CLITERAL(Color){119,66,81,255},// MAROON,
        CLITERAL(Color){142,159,125,255},//GREEN,
        CLITERAL(Color){170,162,93,255},//LIME,
        CLITERAL(Color){91,125,115,255},//DARKGREEN,
        CLITERAL(Color){125,123,98,255},//SKYBLUE,
        CLITERAL(Color){91,125,115,255},//BLUE,
        CLITERAL(Color){78,84,99,255},//DARKBLUE,
        CLITERAL(Color){119,66,81,255},//PURPLE,
        CLITERAL(Color){140,124,121,255},//VIOLET,
        CLITERAL(Color){100,83,85,255},//DARKPURPLE,
        CLITERAL(Color){140,124,121,255},//BEIGE,
        CLITERAL(Color){132,109,89,255},//BROWN,
        CLITERAL(Color){100,83,85,255},//DARKBROWN,
        CLITERAL(Color){169,156,141,255},// WHITE,
        CLITERAL(Color){75,61,68,255},// BLACK,
        CLITERAL(Color){119,66,81,255},//MAGENTA,
        CLITERAL(Color){169,156,141,255},//RAYWHITE
    };break;

    case CS_GREENORANGE:
    return (Palette_Values){
        CLITERAL(Color){0,148,121,255},// LIGHTGRAY,
        CLITERAL(Color){0,50,50,255},// GRAY,
        CLITERAL(Color){0,29,35,255},// DARKGRAY,
        CLITERAL(Color){255,192,115,255},// YELLOW,
        CLITERAL(Color){255,128,0,255},// GOLD
        CLITERAL(Color){221,89,40,255},//ORANGE,
        CLITERAL(Color){173,47,23,255},// RED,
        CLITERAL(Color){173,47,23,255},// MAROON,
        CLITERAL(Color){0,79,77,255},//GREEN,
        CLITERAL(Color){0,148,121,255},//LIME,
        CLITERAL(Color){0,50,50,255},//DARKGREEN,
        CLITERAL(Color){0,148,121,255},//SKYBLUE,
        CLITERAL(Color){0,79,77,255},//BLUE,
        CLITERAL(Color){0,50,50,255},//DARKBLUE,
        CLITERAL(Color){173,47,23,255},//PURPLE,
        CLITERAL(Color){221,89,40,255},//VIOLET,
        CLITERAL(Color){0,29,35,255},//DARKPURPLE,
        CLITERAL(Color){255,192,115,255},//BEIGE,
        CLITERAL(Color){221,89,40,255},//BROWN,
        CLITERAL(Color){173,47,23,255},//DARKBROWN,
        CLITERAL(Color){255,255,255,255},// WHITE,
        CLITERAL(Color){0,0,0,255},// BLACK,
        CLITERAL(Color){197,110,212,255},//MAGENTA,
        CLITERAL(Color){255,255,255,255},//RAYWHITE
    };break;
    
    /*case CS_EPHEMERA:
    return (Palette_Values){
        CLITERAL(Color){222,214,222,255},// LIGHTGRAY,
        CLITERAL(Color){164,157,164,255},// GRAY,
        CLITERAL(Color){164,157,164,255},// DARKGRAY,
        CLITERAL(Color){222,214,222,255},// YELLOW,
        CLITERAL(Color){230,234,238,255},// GOLD
        CLITERAL(Color){164,157,164,255},//ORANGE,
        CLITERAL(Color){213,190,205,255},// RED,
        CLITERAL(Color){197,178,189,255},// MAROON,
        CLITERAL(Color){115,129,123,255},//GREEN,
        CLITERAL(Color){131,145,139,255},//LIME,
        CLITERAL(Color){98,105,106,255},//DARKGREEN,
        CLITERAL(Color){131,137,139,255},//SKYBLUE,
        CLITERAL(Color){98,105,106,255},//BLUE,
        CLITERAL(Color){90,85,90,255},//DARKBLUE,
        CLITERAL(Color){164,157,164,255},//PURPLE,
        CLITERAL(Color){213,190,205,255},//VIOLET,
        CLITERAL(Color){74,60,74,255},//DARKPURPLE,
        CLITERAL(Color){222,214,222,255},//BEIGE,
        CLITERAL(Color){164,157,164,255},//BROWN,
        CLITERAL(Color){90,85,90,255},//DARKBROWN,
        CLITERAL(Color){230,234,238,255},// WHITE,
        CLITERAL(Color){57,44,49,255},// BLACK,
        CLITERAL(Color){197,178,189,255},//MAGENTA,
        CLITERAL(Color){230,234,238,255},//RAYWHITE
    };break;*/

    case CS_NEON:
    return (Palette_Values){
        CLITERAL(Color){116,255,252,255},// LIGHTGRAY,
        CLITERAL(Color){37,82,171,255},// GRAY,
        CLITERAL(Color){0,1,122,255},// DARKGRAY,
        CLITERAL(Color){240,255,0,255},// YELLOW,
        CLITERAL(Color){255,233,0,255},// GOLD
        CLITERAL(Color){255,185,0,255},//ORANGE,
        CLITERAL(Color){255,0,100,255},// RED,
        CLITERAL(Color){237,33,0,255},// MAROON,
        CLITERAL(Color){0,237,99,255},//GREEN,
        CLITERAL(Color){127,225,0,255},//LIME,
        CLITERAL(Color){0,240,10,255},//DARKGREEN,
        CLITERAL(Color){0,225,232,255},//SKYBLUE,
        CLITERAL(Color){0,224,255,255},//BLUE,
        CLITERAL(Color){0,87,255,255},//DARKBLUE,
        CLITERAL(Color){159,0,255,255},//PURPLE,
        CLITERAL(Color){207,0,255,255},//VIOLET,
        CLITERAL(Color){105,0,255,255},//DARKPURPLE,
        CLITERAL(Color){255,223,177,255},//BEIGE,
        CLITERAL(Color){221,89,40,255},//BROWN,
        CLITERAL(Color){240,115,55,255},//DARKBROWN,
        CLITERAL(Color){237,250,255,255},// WHITE,
        CLITERAL(Color){1,0,18,255},// BLACK,
        CLITERAL(Color){255,0,255,255},//MAGENTA,
        CLITERAL(Color){255,250,222,255},//RAYWHITE
    };break;

    /*case CS_DELICIOUS_BLOOD:
    return (Palette_Values){
        CLITERAL(Color){204,41,122,255},// LIGHTGRAY,
        CLITERAL(Color){102,41,71,255},// GRAY,
        CLITERAL(Color){38,26,38,255},// DARKGRAY,
        CLITERAL(Color){255,128,191,255},// YELLOW,
        CLITERAL(Color){255,128,191,255},// GOLD
        CLITERAL(Color){255,0,128,255},//ORANGE,
        CLITERAL(Color){255,0,128,255},// RED,
        CLITERAL(Color){179,45,112,255},// MAROON,
        CLITERAL(Color){245,184,212,255},//GREEN,
        CLITERAL(Color){255,217,235,255},//LIME,
        CLITERAL(Color){204,41,122,255},//DARKGREEN,
        CLITERAL(Color){204,41,122,255},//SKYBLUE,
        CLITERAL(Color){179,45,112,255},//BLUE,
        CLITERAL(Color){153,46,99,255},//DARKBLUE,
        CLITERAL(Color){179,45,112,255},//PURPLE,
        CLITERAL(Color){204,41,122,255},//VIOLET,
        CLITERAL(Color){153,46,99,255},//DARKPURPLE,
        CLITERAL(Color){230,161,193,255},//BEIGE,
        CLITERAL(Color){102,41,71,255},//BROWN,
        CLITERAL(Color){77,34,55,255},//DARKBROWN,
        CLITERAL(Color){255,242,249,255},// WHITE,
        CLITERAL(Color){26,14,20,255},// BLACK,
        CLITERAL(Color){255,0,128,255},//MAGENTA,
        CLITERAL(Color){255,242,249,255},//RAYWHITE
    };break;*/

    case CS_NIGHT_SKY: //NIGHT SKY
    return (Palette_Values){
        CLITERAL(Color){160,186,204,255},// LIGHTGRAY,
        CLITERAL(Color){132,140,163,255},// GRAY,
        CLITERAL(Color){73,77,112,255},// DARKGRAY,
        CLITERAL(Color){255,243,165,255},// YELLOW,
        CLITERAL(Color){255,223,95,255},// ORANGE
        CLITERAL(Color){255,192,82,255},//ORANGE,
        CLITERAL(Color){237,35,110,255},// RED,
        CLITERAL(Color){173,36,83,255},// MAROON,
        CLITERAL(Color){106,212,85,255},//GREEN,
        CLITERAL(Color){148,240,135,255},//LIME,
        CLITERAL(Color){80,163,81,255},//DARKGREEN,
        CLITERAL(Color){128,224,211,255},//SKYBLUE,
        CLITERAL(Color){57,197,204,255},//BLUE,
        CLITERAL(Color){24,157,189,255},//DARKBLUE,
        CLITERAL(Color){197,138,255,255},//PURPLE,
        CLITERAL(Color){222,177,255,255},//VIOLET,
        CLITERAL(Color){127,78,212,255},//DARKPURPLE,
        CLITERAL(Color){240,234,228,255},//BEIGE,
        CLITERAL(Color){217,170,141,255},//BROWN,
        CLITERAL(Color){135,103,94,255},//DARKBROWN,
        CLITERAL(Color){235,255,255,255},// WHITE,
        CLITERAL(Color){0,3,33,255},// BLACK,
        CLITERAL(Color){197,110,212,255},//MAGENTA,
        CLITERAL(Color){226,245,244,255},//RAYWHITE
    };break;
    
    /*case CS_OVERGROWN_TEMPLE: //Overgrown
    return (Palette_Values){
        CLITERAL(Color){224,158,114,255},//LIGHTGRAY,
        CLITERAL(Color){130,88,51,255},//GRAY,
        CLITERAL(Color){77,52,31,255},//DARKGRAY,
        YELLOW,
        GOLD,
        ORANGE,
        RED,
        MAROON,
        GREEN,
        LIME,
        DARKGREEN,
        SKYBLUE,
        BLUE,
        DARKBLUE,
        PURPLE,
        VIOLET,
        DARKPURPLE,
        BEIGE,
        BROWN,
        DARKBROWN,
        CLITERAL(Color){240,209,184,255},//WHITE,
        CLITERAL(Color){7,28,1,255},//BLACK,
        MAGENTA,
        CLITERAL(Color){240,235,211,255},//RAYWHITE
    };break;*/

    case CS_TOXIC_WASTE:
    return (Palette_Values){
        CLITERAL(Color){170,160,138,255},// LIGHTGRAY,
        CLITERAL(Color){121,108,98,255},// GRAY,
        CLITERAL(Color){75,67,65,255},// DARKGRAY,
        CLITERAL(Color){255,250,51,255},// YELLOW,
        CLITERAL(Color){255,168,36,255},// GOLD,
        CLITERAL(Color){223,95,0,255},// ORANGE,
        CLITERAL(Color){34,255,106,255},// RED,
        CLITERAL(Color){0,196,122,255},// MAROON,
        CLITERAL(Color){149,255,70,255},//GREEN,
        CLITERAL(Color){203,255,94,255},//LIME,
        CLITERAL(Color){22,221,0,255},//DARKGREEN,
        CLITERAL(Color){159,255,200,255},//SKYBLUE,
        CLITERAL(Color){74,255,183,255},//BLUE,
        CLITERAL(Color){34,255,227,255},//DARKBLUE,
        CLITERAL(Color){133,204,0,255},//PURPLE,
        CLITERAL(Color){183,255,13,255},//VIOLET,
        CLITERAL(Color){74,187,0,255},//DARKPURPLE,
        CLITERAL(Color){255,252,113,255},//BEIGE,
        CLITERAL(Color){213,181,0,255},//BROWN,
        CLITERAL(Color){164,115,0,255},//DARKBROWN,
        CLITERAL(Color){249,245,236,255},// WHITE,
        CLITERAL(Color){0,31,26,255},// BLACK,
        CLITERAL(Color){240,255,51,255},//MAGENTA,
        CLITERAL(Color){249,255,168,255},//RAYWHITE
    };break;

    default: // DEFAULT
    return (Palette_Values){
        LIGHTGRAY,
        GRAY,
        DARKGRAY,
        YELLOW,
        GOLD,
        ORANGE,
        RED,
        MAROON,
        GREEN,
        LIME,
        DARKGREEN,
        SKYBLUE,
        BLUE,
        DARKBLUE,
        PURPLE,
        VIOLET,
        DARKPURPLE,
        BEIGE,
        BROWN,
        DARKBROWN,
        WHITE,
        BLACK,
        MAGENTA,
        RAYWHITE
    };break;
    }
}
