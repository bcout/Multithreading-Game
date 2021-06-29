/********************************************************************
    Module: player.c
    Author: Brennan Couturier

    Functions to manage player data
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "../include/player.h"
#include "../include/game_globals.h"

void create_player(int start_row_in, int start_col_in, int num_lives_in){
    player* to_return = malloc(sizeof(player));
    if(to_return == NULL){
        //TODO: Create error handlers
        fprintf(stderr, "Error in create_player() : Could not allocate space for player\n");
        exit(EXIT_FAILURE);
    }

    to_return->start_col = start_col_in;
    to_return->start_row = start_row_in;
    to_return->state = ALIVE;
    to_return->running = true;
    to_return->lives = num_lives_in;
    to_return->col = start_col_in;
    to_return->row = start_row_in;
    to_return->anim_frame = 0;
    to_return->score = 0;

    //assign this object to the global player variable
    player_ref = to_return;

    return;
}

/*
void reset_player(player* p){
    p->col = p->start_col;
    p->row = p->start_row;
    p->anim_frame = 0;
    p->state = ALIVE;
}
*/
/*
void redraw_player(player* p){
    //not thread safe
    pthread_mutex_lock(&screen_mutex);
    consoleClearImage(p->row, p->col, PLAYER_HEIGHT, PLAYER_WIDTH);
    consoleDrawImage(p->row, p->col, PLAYER_GRAPHIC[p->anim_frame], PLAYER_HEIGHT);
    pthread_mutex_unlock(&screen_mutex);
}
*/