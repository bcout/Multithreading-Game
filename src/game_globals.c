/********************************************************************
    Module: game_globals.c
    Author: Brennan Couturier

    Variables used throughout the program
********************************************************************/

#include <stdlib.h>

#include "../include/game_globals.h"
#include "../include/lists.h"
#include "../include/player.h"
#include "../include/game_space.h"

caterpillar_node* caterpillar_list_head = NULL;
bullet_node* bullet_list_head = NULL;
thread_node* thread_list_head = NULL;
player* player_ref = NULL;

bool game_running = false;

char* PLAYER_GRAPHIC[PLAYER_ANIM_FRAMES][PLAYER_HEIGHT] = {
    {"<^>"},
    {"< >"}
};

/**** DIMENSIONS MUST MATCH the ROWS/COLS */
char* GAME_BOARD[] = {
"",
"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-centipiede!=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
"",
"",
"",
"",
"",
"", 
"" };