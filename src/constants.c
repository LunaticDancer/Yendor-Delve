#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 16;
const char *GAME_TITLE = "Yendor Delve";
const int PAUSE_MENU_OPTION_COUNT = 2;
const char CRIT_PROGRESS_MAX = 100;
const float TURN_ACTION_DURATION = 0.4;
const float GAME_MESSAGE_DISPLAY_TIME = 1.0;
const float GAME_MESSAGE_DISPLAY_TIME_PER_CHARACTER = 0.05;


char* CombineStrings(char* str1, char* str2)
{
    char* new_str;
    if((new_str = malloc(strlen(str1)+strlen(str2)+1)) != NULL){
        new_str[0] = '\0';   // ensures the memory is an empty string
        strcat(new_str,str1);
        strcat(new_str,str2);
    } else {
        fprintf(stderr,"malloc failed!\n");
        // exit?
    }
}