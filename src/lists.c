/********************************************************************
    Module: lists.c
    Author: Brennan Couturier

    Functions to manage bullet/caterpillar/thread linked lists
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/lists.h"
#include "../include/threads_mutexes.h"
#include "../include/game_globals.h"

/********************************************************************
Caterpillar list functions
********************************************************************/

caterpillar_node* create_caterpillar_node(caterpillar* caterpillar_in){

    caterpillar_node* to_return = malloc(sizeof(caterpillar_node));
    if(to_return == NULL){
        //TODO: Create error handlers
        fprintf(stderr, "Error in create_caterpillar_node() : Could not allocate space for caterpillar node\n");
        exit(EXIT_FAILURE);
    }
    to_return->_caterpillar = caterpillar_in;
    to_return->next = NULL;

    return to_return;
}

int add_caterpillar_to_list(caterpillar* in){
    caterpillar_node* to_add = create_caterpillar_node(in);

    pthread_mutex_lock(&caterpillar_list_mutex);

    //if the list is empty, just make head the new node
    if(caterpillar_list_head == NULL){
        caterpillar_list_head = to_add;
        pthread_mutex_unlock(&caterpillar_list_mutex);
        return 0;
    }

    //otherwise, add c to the end of the list
    caterpillar_node* curr = caterpillar_list_head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = to_add;

    pthread_mutex_unlock(&caterpillar_list_mutex);

    return 0;
}

int remove_caterpillar_from_list(caterpillar* to_remove){
    pthread_mutex_lock(&caterpillar_list_mutex);
    caterpillar_node* curr = caterpillar_list_head;
    caterpillar_node* prev = curr;
    while(curr->next != NULL){
        curr = curr->next;
        if(prev->_caterpillar == to_remove){
            //The addresses match, they are the same caterpillar
            if(prev == caterpillar_list_head){
                //removing the first item
                caterpillar_list_head = prev->next;
                free(prev->_caterpillar);
                free(prev);
                prev = NULL;
                pthread_mutex_unlock(&caterpillar_list_mutex);
                return 0;
            }
        }else if(curr->_caterpillar == to_remove){
            //Remove curr, make prev->next skip it
            prev->next = curr->next;
            free(curr->_caterpillar);
            free(curr);
            curr = NULL;
            pthread_mutex_unlock(&caterpillar_list_mutex);
            return 0;
        }
        prev = curr;
    }
    pthread_mutex_unlock(&caterpillar_list_mutex);
    return -1;
}

void print_caterpillar_list(){

    pthread_mutex_lock(&caterpillar_list_mutex);
    caterpillar_node* curr = caterpillar_list_head;
    while(curr != NULL){
        fprintf(stdout, "<%p>\tcol %d\trow %d\tsize %d\n",
                (curr->_caterpillar),
                curr->_caterpillar->col,
                curr->_caterpillar->row,
                curr->_caterpillar->size);
        curr = curr->next;
    }
    pthread_mutex_unlock(&caterpillar_list_mutex);
}

void free_caterpillar_list(){
    pthread_mutex_lock(&caterpillar_list_mutex);
    caterpillar_node* curr = caterpillar_list_head;
    caterpillar_node* prev = curr;
    while(curr != NULL){
        curr = curr->next;
        free(prev->_caterpillar);
        free(prev);
        prev = curr;
    }
    pthread_mutex_unlock(&caterpillar_list_mutex);
}

/********************************************************************
Bullet list functions
********************************************************************/

bullet_node* create_bullet_node(bullet* bullet_in){

    bullet_node* to_return = malloc(sizeof(bullet_node));
    if(to_return == NULL){
        //TODO: Create error handlers
        fprintf(stderr, "Error in create_bullet_node() : Could not allocate space for bullet node\n");
        exit(EXIT_FAILURE);
    }
    to_return->_bullet = bullet_in;
    to_return->next = NULL;

    return to_return;
}

int add_bullet_to_list(bullet* in){
    bullet_node* to_add = create_bullet_node(in);

    pthread_mutex_lock(&bullet_list_mutex);

    //if the list is empty, just make head the new node
    if(bullet_list_head == NULL){
        bullet_list_head = to_add;
        pthread_mutex_unlock(&bullet_list_mutex);
        return 0;
    }

    //otherwise, add c to the end of the list
    bullet_node* curr = bullet_list_head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = to_add;

    pthread_mutex_unlock(&bullet_list_mutex);

    return 0;
}

int remove_bullet_from_list(bullet* to_remove){
    pthread_mutex_lock(&bullet_list_mutex);
    bullet_node* curr = bullet_list_head;
    bullet_node* prev = curr;
    while(curr->next != NULL){
        curr = curr->next;
        if(prev->_bullet == to_remove){
            //The addresses match, they are the same bullet
            if(prev == bullet_list_head){
                //removing the first item
                bullet_list_head = prev->next;
                free(prev->_bullet);
                free(prev);
                prev = NULL;
                pthread_mutex_unlock(&bullet_list_mutex);
                return 0;
            }
        }else if(curr->_bullet == to_remove){
            //Remove curr, make prev->next skip it
            prev->next = curr->next;
            free(curr->_bullet);
            free(curr);
            curr = NULL;
            pthread_mutex_unlock(&bullet_list_mutex);
            return 0;
        }
        prev = curr;
    }
    pthread_mutex_unlock(&bullet_list_mutex);
    return -1;
}

void print_bullet_list(){

    pthread_mutex_lock(&bullet_list_mutex);
    bullet_node* curr = bullet_list_head;
    while(curr != NULL){
        fprintf(stdout, "<%p>\tcol %d\trow %d\n",
                (curr->_bullet),
                curr->_bullet->col,
                curr->_bullet->row);
        curr = curr->next;
    }
    pthread_mutex_unlock(&bullet_list_mutex);
}

void free_bullet_list(){
    pthread_mutex_lock(&bullet_list_mutex);
    bullet_node* curr = bullet_list_head;
    bullet_node* prev = curr;
    while(curr != NULL){
        curr = curr->next;
        free(prev->_bullet);
        free(prev);
        prev = curr;
    }
    pthread_mutex_unlock(&bullet_list_mutex);
}

/********************************************************************
Thread list functions
********************************************************************/

thread_node* create_thread_node(pthread_t thread_in, int id){
    thread_node* to_return = malloc(sizeof(thread_node));
    if(to_return == NULL){
        //TODO: Create error handlers
        fprintf(stderr, "Error in create_thread_node() : Could not allocate space for thread_node\n");
        exit(EXIT_FAILURE);
    }
    to_return->_thread = thread_in;
    to_return->custom_id = id;
    to_return->next = NULL;
    return to_return;
}

int add_thread_to_list(pthread_t in, int id){
    thread_node* to_add = create_thread_node(in, id);

    pthread_mutex_lock(&thread_list_mutex);

    //if the list is empty, just make head the new node
    if(thread_list_head == NULL){
        thread_list_head = to_add;
        pthread_mutex_unlock(&thread_list_mutex);
        return 0;
    }

    //otherwise, add c to the end of the list
    thread_node* curr = thread_list_head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = to_add;

    pthread_mutex_unlock(&thread_list_mutex);

    return 0;
}

int remove_thread_from_list(pthread_t to_remove){
    pthread_mutex_lock(&thread_list_mutex);
    thread_node* curr = thread_list_head;
    if(curr == NULL){
        fprintf(stderr, "List is empty\n");
        pthread_mutex_unlock(&thread_list_mutex);
        return -1;
    }
    thread_node* prev = curr;
    while(curr->next != NULL){
        curr = curr->next;
        if(pthread_equal(prev->_thread, to_remove) != 0){
            //The addresses match, they are the same thread
            if(prev == thread_list_head){
                //join the thread and remove the first item
                thread_list_head = prev->next;
                pthread_join(prev->_thread, NULL);
                //fprintf(stdout, "Joined <%d>\n", prev->custom_id);
                free(prev);
                prev = NULL;
                pthread_mutex_unlock(&thread_list_mutex);
                return 0;
            }
        }else if(pthread_equal(curr->_thread, to_remove) != 0){
            //Join curr->_thread, remove curr, make prev->next skip it
            pthread_join(curr->_thread, NULL);
            //fprintf(stdout, "Joined <%d>\n", curr->custom_id);
            prev->next = curr->next;
            free(curr);
            curr = NULL;
            pthread_mutex_unlock(&thread_list_mutex);
            return 0;
        }
        prev = curr;
    }
    pthread_mutex_unlock(&thread_list_mutex);
    return -1;
}

void print_thread_list(){

    pthread_mutex_lock(&thread_list_mutex);
    thread_node* curr = thread_list_head;
    while(curr != NULL){
        fprintf(stdout, "<%d>\n", curr->custom_id);
        curr = curr->next;
    }
    pthread_mutex_unlock(&thread_list_mutex);
}

#include <string.h>
void join_free_thread_list(){
    pthread_mutex_lock(&thread_list_mutex);
    thread_node* curr = thread_list_head;
    thread_node* prev = curr;
    while(curr != NULL){
        curr = curr->next;
        //fprintf(stdout, "Joining <%d>\n", prev->custom_id);
        int err = pthread_join(prev->_thread, NULL);
        if(err != 0){
            fprintf(stdout, "Error %s\n", strerror(err));
        }
        //fprintf(stdout, "Joined <%d>\n", prev->custom_id);
        free(prev);
        prev = curr;
    }
    pthread_mutex_unlock(&thread_list_mutex);
}