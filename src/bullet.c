/********************************************************************
    Module: bullet.c
    Author: Brennan Couturier

    Functions to manage bullet data
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/bullet.h"

char* PLAYER_BULLET_GRAPHIC[BULLET_ANIM_FRAMES][BULLET_HEIGHT] = {
    {"'"}
};

bullet* create_bullet(){
    bullet* to_return = malloc(sizeof(bullet));
    if(to_return == NULL){
        //TODO: Create error handlers
        fprintf(stderr, "Error in create_bullet() : Could not allocate space for bullet\n");
        exit(EXIT_FAILURE);
    }
    return to_return;
}