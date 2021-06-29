/********************************************************************
    Module: player.h
    Author: Brennan Couturier

    Functions to manage player data
********************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_ANIM_FRAMES 2
#define PLAYER_ANIM_TICKS 50
#define PLAYER_WIDTH 3
#define PLAYER_HEIGHT 1

#include <stdbool.h>
#include <pthread.h>

/********************************************************************
Keep track of the player's current status, whether they're alive and
    moving, just got hit and are respawing, or are out of lives.
********************************************************************/
typedef enum player_state_enum{
    ALIVE,
    DEAD,
    GAMEOVER
} player_state;

/********************************************************************
Keep track of useful player info
    start_col, start_row: Where the player spawned
    state: The player's current status
    running: Whether the player can move/shoot/get hit
    lives: How many lives the player has left
    col, row: The current location of the player
    anim_frame: What animation frame is currently being drawn
    score: The player's current score
********************************************************************/
typedef struct player_struct{
    int start_col;
    int start_row;

    player_state state;

    bool running;
    int lives;
    int col;
    int row;
    int anim_frame;
    int score;
} player;

/********************************************************************
Allocate a player struct on the heap, assigns that pointer to
    the global variable player_ref instead of returning it
********************************************************************/
void create_player(int start_row, int start_col, int num_lives);

/********************************************************************
Start the player thread, initialize its mutex
********************************************************************/
void spawn_player(player* p);

/********************************************************************
Draw a blank where the player currently is, then draw the player at
    the new location
********************************************************************/
void move_player(player* p, int dest_col, int dest_row);

/********************************************************************
Called when the player gets hit by an enemy bullet, remove one life,
    end game if all the lives are gone
********************************************************************/
void damage_player(player* p);

#endif