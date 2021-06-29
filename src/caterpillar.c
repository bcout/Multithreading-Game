/********************************************************************
    Module: caterpillar.c
    Author: Brennan Couturier

    Functions to manage caterpillar data
********************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "../include/caterpillar.h"

caterpillar* create_caterpillar(int start_col, int start_row, int size){
    caterpillar* to_return = malloc(sizeof(caterpillar));
    if(to_return == NULL){
        //TODO: Create error handlers
        fprintf(stderr, "Error in create_caterpillar() : Could not allocate space for caterpillar\n");
        exit(EXIT_FAILURE);
    }

    to_return->col = start_col;
    to_return->row = start_row;
    to_return->size = size;
    return to_return;
}