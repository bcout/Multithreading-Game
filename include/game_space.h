/********************************************************************
    Module: game_space.h
    Author: Brennan Couturier

    Functions to manipulate individual coordinates
********************************************************************/

#ifndef GAME_SPACE_H
#define GAME_SPACE_H

#include "../include/player.h"
#include "../include/caterpillar.h"
#include "../include/bullet.h"

/********************************************************************
A coordinate can be empty (nothing on it), solid (wall that player 
    can't go through), or is populated by the player, a bullet or 
    a caterpillar.
********************************************************************/
typedef enum collision_status_enum{
    empty,
    occupied
} collision_status;

/********************************************************************
Each coordinate has a status and 1 object on it.
********************************************************************/
typedef struct game_space_struct{
    int row;
    int col;
    collision_status status;
    player* _player;
    caterpillar* _caterpillar;
    bullet* _bullet;
} game_space;

/********************************************************************
Create a square to populate the collision matrix with
********************************************************************/
game_space* create_game_space(collision_status status, int row, int col);

/********************************************************************
Free a given game space. Assumes the player/bullet/caterpillar
    has been freed already, so it just sets them to NULL
********************************************************************/
void delete_game_space(game_space* to_delete);

/********************************************************************
Populate the collision matrix
********************************************************************/
void fill_collision_map();

/********************************************************************
Frees every game space
********************************************************************/
void clear_collision_map();

#endif