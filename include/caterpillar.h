/********************************************************************
    Module: enemy.h
    Author: Brennan Couturier

    Functions to manage caterpillar data
********************************************************************/

#ifndef CATERPILLAR_H
#define CATERPILLAR_H

/********************************************************************
Keep track of important caterpillar data
    col, row: Where the caterpillar currently is, measured at the head
********************************************************************/
typedef struct caterpillar_struct{
    int col;
    int row;
    int size;
} caterpillar;

/********************************************************************
Creates a new caterpillar of a given size
********************************************************************/
caterpillar* create_caterpillar(int start_col, int start_row, int size);

/********************************************************************
Draws an empty space where the caterpillar currently is then draws
    a caterpillar at the new location
********************************************************************/
void move_caterpillar(caterpillar* c, int dest_col, int dest_row);

#endif