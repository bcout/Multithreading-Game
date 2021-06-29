/********************************************************************
    Module: game_globals.h
    Author: Brennan Couturier

    Variables used throughout the program
********************************************************************/

#ifndef GAME_GLOBALS_H
#define GAME_GLOBALS_H

#include "lists.h"
#include "player.h"
#include "game_space.h"

#define GAME_COLS 80
#define GAME_ROWS 24
#define PLAYER_START_ROW 18
#define PLAYER_START_COL 20
#define PLAYER_START_LIVES 5
/********************************************************************
Keep track of the beginning of the caterpillar and bullet lists
********************************************************************/
extern caterpillar_node* caterpillar_list_head;
extern bullet_node* bullet_list_head;
extern thread_node* thread_list_head;


/********************************************************************
The visual representation of the player
********************************************************************/
extern char* PLAYER_GRAPHIC[PLAYER_ANIM_FRAMES][PLAYER_HEIGHT];

/********************************************************************
Give everyone access to the player object, use mutexes to control access
********************************************************************/
extern player* player_ref;

/********************************************************************
The condition variable the main thread waits on after creating
    and starting up the game's many threads
********************************************************************/
pthread_cond_t game_cond;

/********************************************************************
A variable checked by every thread to see if they should still run
********************************************************************/
extern bool game_running;

/********************************************************************
A matrix populated with values signifying whehter a space is occupied,
    empty, or a solid wall (border of playable area)
********************************************************************/
game_space* collision_map[GAME_ROWS][GAME_COLS];

/********************************************************************
The visual version of the game board, made out of strings
********************************************************************/
extern char* GAME_BOARD[];

#endif