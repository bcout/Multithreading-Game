/********************************************************************
    Module: lists.h
    Author: Brennan Couturier

    Functions to manage bullet/caterpillar/thread linked lists
********************************************************************/

#ifndef LISTS_H
#define LISTS_H

#include <pthread.h>

#include "caterpillar.h"
#include "bullet.h"

/********************************************************************
Keep track of how many threads that need to be joined at the end
********************************************************************/
typedef struct thread_node_struct{
    pthread_t _thread;
    int custom_id;
    struct thread_node_struct* next;
} thread_node;

/********************************************************************
Keep track of how many caterpillars are currently in play
********************************************************************/
typedef struct caterpillar_node_struct{
    caterpillar* _caterpillar;
    struct caterpillar_node_struct* next;
} caterpillar_node;

/********************************************************************
Keep track of how many bullets are currently in play
********************************************************************/
typedef struct bullet_node_struct{
    bullet* _bullet;
    struct bullet_node_struct* next;
} bullet_node;


/********************************************************************
Adds a caterpillar to the end of the linked list
    returns 0 upon successful addition
********************************************************************/
int add_caterpillar_to_list(caterpillar* to_add);

/********************************************************************
Remove a given caterpillar from its list
    Compares the address of to_remove with the caterpillars in the
    list. If one matches, remove it and return.
    
    Returns 0 upon successful deletion
    Returns -1 if to_remove wasn't in the list
********************************************************************/
int remove_caterpillar_from_list(caterpillar* to_remove);

/********************************************************************
Print the caterpillar list using its global head variable
********************************************************************/
void print_caterpillar_list();

/********************************************************************
Frees every caterpillar in the list
********************************************************************/
void free_caterpillar_list();

/********************************************************************
Adds a bullet node to the end of the list
********************************************************************/
int add_bullet_to_list(bullet* to_add);

/********************************************************************
Remove a given bullet from its list
********************************************************************/
int remove_bullet_from_list(bullet* to_remove);

/********************************************************************
Print the bullet list using its global head variable
********************************************************************/
void print_bullet_list();

/********************************************************************
Frees every bullet in the list
********************************************************************/
void free_bullet_list();

/********************************************************************
Adds a thread node to the end of the list
********************************************************************/
int add_thread_to_list(pthread_t thread, int custom_thread_id);

/********************************************************************
Remove a given thread from its list. Assumes the thread has been 
    joined already when it frees the node.
********************************************************************/
int remove_thread_from_list(pthread_t thread);

/********************************************************************
Print the addresses of the thread in the thread list using its global 
    head variable
********************************************************************/
void print_thread_list();

/********************************************************************
Joins each thread in the list and frees the nodes
********************************************************************/
void join_free_thread_list();

#endif