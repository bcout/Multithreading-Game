/********************************************************************
    Module: game_space.c
    Author: Brennan Couturier

    Functions to manipulate individual coordinates
********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "../include/game_space.h"
#include "../include/game_globals.h"
#include "../include/player.h"
#include "../include/caterpillar.h"
#include "../include/bullet.h"

game_space* create_game_space(collision_status status_in, int row_in, int col_in){
    game_space* to_return = malloc(sizeof(game_space));
    if(to_return == NULL){
        //TODO: create error handlers
        fprintf(stderr, "Error in create_game_space() : Could not allocate space for game space\n");
        exit(EXIT_FAILURE);
    }

    to_return->row = row_in;
    to_return->col = col_in;
    to_return->status = status_in;
    to_return->_bullet = NULL;
    to_return->_caterpillar = NULL;
    to_return->_player = NULL;

    return to_return;
}

void delete_game_space(game_space* to_delete){
    to_delete->_player = NULL;
    to_delete->_bullet = NULL;
    to_delete->_caterpillar = NULL;

    free(to_delete);
}

void fill_collision_map(){
    int i;
    int j;
    int top_player_line_y_coord = 16;
    collision_status status;

    //initialize them all as NULL
    for(i = 0; i < GAME_ROWS; i++){
        for(j = 0; j < GAME_COLS; j++){
            collision_map[i][j] = NULL;
        }
    }

    for(i = 0; i < GAME_ROWS; i++){
        for(j = 0; j < GAME_COLS; j++){
            //-1 is an invalid value, just to make it so the boundaries
            //  do not get assigned a game space
            if(j == 0 || j == (GAME_COLS - PLAYER_WIDTH)){
                status = -1;
            }else if(i == top_player_line_y_coord){
                status = -1;
            }else if(i == GAME_ROWS - 1){
                status = -1;
            }else if(i == 1){
                status = -1;
            }else{
                status = empty;
            }

            if(status == empty){
                //Create game space with that status
                game_space* to_add = create_game_space(status, i, j);

                collision_map[i][j] = to_add;
            }
            
        }
    }
}

void clear_collision_map(){
    int i;
    int j;

    for(i = 0; i < GAME_ROWS; i++){
        for(j = 0; j < GAME_COLS; j++){
            if(collision_map[i][j] != NULL){
                delete_game_space(collision_map[i][j]);
            }
        }
    }
}