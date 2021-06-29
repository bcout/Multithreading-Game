/********************************************************************
    Module: bullet.h
    Author: Brennan Couturier

    Functions to manage bullet data
********************************************************************/

#ifndef BULLET_H
#define BULLET_H

#define BULLET_ANIM_FRAMES 1
#define BULLET_ANIM_TICKS 50
#define BULLET_WIDTH 1
#define BULLET_HEIGHT 1

#include <stdbool.h>
#include <pthread.h>

extern char* PLAYER_BULLET_GRAPHIC[BULLET_ANIM_FRAMES][BULLET_HEIGHT];
extern char* ENEMY_BULLET_GRAPHIC[BULLET_ANIM_FRAMES][BULLET_HEIGHT];
/********************************************************************
Keep track of important bullet data
    col, row: The current location of the bullet
********************************************************************/
typedef struct bullet_struct{
    int col;
    int row;
    pthread_t thread;
    pthread_mutex_t mutex;
} bullet;

/********************************************************************
Allocate a bullet on the heap
********************************************************************/
bullet* create_bullet();

#endif