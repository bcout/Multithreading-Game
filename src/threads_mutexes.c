/********************************************************************
    Module: threads_mutexes.c
    Author: Brennan Couturier

    All the threads and mutexes
********************************************************************/

#define SCREEN_SLEEP_TICKS 1

#define LIVES_DIGITS 9
#define SCORE_DIGITS 13
#define LIVES_COL 40
#define SCORE_COL 20

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <curses.h>
#include <sys/select.h>

#include "../include/threads_mutexes.h"
#include "../include/game_globals.h"
#include "../include/console.h"
#include "../include/lists.h"
#include "../include/player.h"
#include "../include/bullet.h"

pthread_mutex_t player_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t screen_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t caterpillar_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bullet_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t game_cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t game_running_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t collision_map_mutex = PTHREAD_MUTEX_INITIALIZER;

//Local input buffer mutex
pthread_mutex_t input_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

//Input buffer, local to this entire file
char input_buffer;

/********************************************************************
Update the screen every tick
********************************************************************/
void* run_screen_thread(void* data){

    while(1){
        //Check if the game is still going
        pthread_mutex_lock(&game_running_mutex);
        if(!game_running){
            pthread_mutex_unlock(&game_running_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&game_running_mutex);

        //refresh the screen
        pthread_mutex_lock(&screen_mutex);
        //
        consoleDrawImage(0, 0, GAME_BOARD, GAME_ROWS);
        consoleRefresh();
        //
        pthread_mutex_unlock(&screen_mutex);

        sleepTicks(SCREEN_SLEEP_TICKS);
    }
    
    return NULL;
}

/********************************************************************
Check statuses to see if the game needs to end
    Also, print score and lives
********************************************************************/
void* run_upkeep_thread(void* data){

    //space for up to 99 lives
    char lives_buffer[LIVES_DIGITS];

    //space for a 5 digit score
    char score_buffer[SCORE_DIGITS];

    while(1){
        //check if the game is still going
        pthread_mutex_lock(&game_running_mutex);
        if(!game_running){
            pthread_mutex_unlock(&game_running_mutex);
            //This thread handles signalling the main function to wake up
            pthread_cond_signal(&game_cond);
            return NULL;
        }
        pthread_mutex_unlock(&game_running_mutex);

        /*
        pthread_mutex_lock(&player_mutex);
        pthread_mutex_lock(&screen_mutex);
        pthread_mutex_lock(&collision_map_mutex);
        //
        char buffer[65];
        if(collision_map[player_ref->row][player_ref->col] == NULL){
            snprintf(buffer, 65, "collision_map[%d, %d] : NULL\nplayer[%d, %d]",
                            player_ref->row, player_ref->col,
                            player_ref->row, player_ref->col);
        }else{
            snprintf(buffer, 65, "collision_map[%d, %d] : %p\nplayer[%d, %d]",
                            player_ref->row, player_ref->col, 
                            collision_map[player_ref->row][player_ref->col]->_player,
                            player_ref->row, player_ref->col);
        }
        putString(buffer, 20, 0, 65);
        //
        pthread_mutex_unlock(&collision_map_mutex);
        pthread_mutex_unlock(&screen_mutex);
        pthread_mutex_unlock(&player_mutex);
        */

        //Print score and lives
        pthread_mutex_lock(&player_mutex);
        pthread_mutex_lock(&screen_mutex);
        //
        snprintf(lives_buffer, LIVES_DIGITS, "Lives: %d", player_ref->lives);
        snprintf(score_buffer, SCORE_DIGITS, "Score: %05d", player_ref->score); //print score in 5 digits always

        putString(lives_buffer, 0, LIVES_COL, LIVES_DIGITS);
        putString(score_buffer, 0, SCORE_COL, SCORE_DIGITS);
        //
        pthread_mutex_unlock(&screen_mutex);
        pthread_mutex_unlock(&player_mutex);

    }

    return NULL;
}

/********************************************************************
Move the bullet up the screen until it hits a caterpillar or the
    top of the screen
********************************************************************/
void* run_player_bullet_thread(void* data){

    int new_row;

    bullet* _bullet = (bullet*)data;
    
    pthread_mutex_lock(&player_mutex);
    _bullet->col = (player_ref->col) + (int)(PLAYER_WIDTH / 2);
    _bullet->row = player_ref->row - 1;
    pthread_mutex_unlock(&player_mutex);

    while(1){
        //check if the game is still going
        pthread_mutex_lock(&game_running_mutex);
        if(!game_running){
            pthread_mutex_unlock(&game_running_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&game_running_mutex);

        //check collision_map and move player up
        new_row = _bullet->row - 1;
        if(new_row == 1){
            //bullet hit the top of the screen
            pthread_mutex_lock(&screen_mutex);
            consoleClearImage(_bullet->row, _bullet->col, BULLET_HEIGHT, BULLET_WIDTH);
            pthread_mutex_unlock(&screen_mutex);
            free(_bullet);
            return NULL;
        }

        //clear the bullet's current location
        pthread_mutex_lock(&screen_mutex);
        consoleClearImage(_bullet->row, _bullet->col, BULLET_HEIGHT, BULLET_WIDTH);
        pthread_mutex_unlock(&screen_mutex);

        //move the bullet up
        _bullet->row = new_row;

        pthread_mutex_lock(&screen_mutex);
        consoleDrawImage(_bullet->row, _bullet->col, PLAYER_BULLET_GRAPHIC[0], BULLET_HEIGHT);
        pthread_mutex_unlock(&screen_mutex);

        sleepTicks(SCREEN_SLEEP_TICKS * 7);
    }

    return NULL;
}

/********************************************************************
Clear the current player, calculate its next position, draw it
********************************************************************/
void* run_player_movement_thread(void* data){

    int x_offset = 0;
    int y_offset = 0;
    int new_x = 0;
    int new_y = 0;

    bullet* _b;

    while(1){

        //check if the game is still going
        pthread_mutex_lock(&game_running_mutex);
        if(!game_running){
            pthread_mutex_unlock(&game_running_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&game_running_mutex);

        //Clear the player's current position
        pthread_mutex_lock(&player_mutex);
        pthread_mutex_lock(&screen_mutex);
        //
        consoleClearImage(player_ref->row, player_ref->col, PLAYER_HEIGHT, PLAYER_WIDTH);
        //
        pthread_mutex_unlock(&screen_mutex);
        pthread_mutex_unlock(&player_mutex);

        //check for cached input
        pthread_mutex_lock(&input_buffer_mutex);
        //
        switch(input_buffer){
            case 'a':
                x_offset = -1;
                break;
            case 's':
                y_offset = 1;
                break;
            case 'd':
                x_offset = 1;
                break;
            case 'w':
                y_offset = -1;
                break;
            case 32:
                //space
                //spawn bullet
                _b = create_bullet();
                add_bullet_to_list(_b);
                pthread_create(&(_b->thread), NULL, run_player_bullet_thread, (void*)_b);
                add_thread_to_list(_b->thread, 100);
                break;
            case 'q':
                pthread_mutex_lock(&game_running_mutex);
                game_running = false;
                pthread_mutex_unlock(&game_running_mutex);
            default:
                break;
        }
        input_buffer = 0;
        //
        pthread_mutex_unlock(&input_buffer_mutex);

        
        //Do collision checking and update player position
        pthread_mutex_lock(&player_mutex);
        pthread_mutex_lock(&screen_mutex);
        pthread_mutex_lock(&collision_map_mutex);
        //
        new_x = player_ref->col + x_offset;
        new_y = player_ref->row + y_offset;
        
        if(collision_map[new_y][new_x] != NULL){
            //Remove the player from the current game_space
            collision_map[player_ref->row][player_ref->col]->_player = NULL;

            player_ref->col += x_offset;
            player_ref->row += y_offset;

            //Add the player to the current game space
            collision_map[player_ref->row][player_ref->col]->_player = player_ref;
        }
        //
        pthread_mutex_unlock(&collision_map_mutex);
        pthread_mutex_unlock(&screen_mutex);
        pthread_mutex_unlock(&player_mutex);
        

        //Reset offset values
        x_offset = 0;
        y_offset = 0;
        
        //Draw the player at their new location
        pthread_mutex_lock(&player_mutex);
        pthread_mutex_lock(&screen_mutex);
        //
        consoleDrawImage(player_ref->row, player_ref->col, PLAYER_GRAPHIC[player_ref->anim_frame], PLAYER_HEIGHT);
        //
        pthread_mutex_unlock(&screen_mutex);
        pthread_mutex_unlock(&player_mutex);

        sleepTicks(SCREEN_SLEEP_TICKS * 7);
    }
    return NULL;
}

/********************************************************************
Update the player's anim_frame
********************************************************************/
void* run_player_animation_thread(void* data){
    

    while(1){
        //check if the game is still going
        pthread_mutex_lock(&game_running_mutex);
        if(!game_running){
            pthread_mutex_unlock(&game_running_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&game_running_mutex);

        //Update animation
        pthread_mutex_lock(&player_mutex);
        //
        player_ref->anim_frame++;
        player_ref->anim_frame %= PLAYER_ANIM_FRAMES;
        //
        pthread_mutex_unlock(&player_mutex);

        sleepTicks(PLAYER_ANIM_TICKS);        
    }

    return NULL;
}

/********************************************************************
Move the bullet down the screen until it hits the or the
    bottom of the screen
********************************************************************/
void* run_caterpillar_bullet_thread(void* data){
    return NULL;
}

void* run_caterpillar_thread(void* data){

    return NULL;
}

/********************************************************************
Check for input, this thread blocks until a key is hit
********************************************************************/
void* run_input_thread(void* data){

    char key_pressed;

    while(1){
        //check if the game is still going
        pthread_mutex_lock(&game_running_mutex);
        if(!game_running){
            pthread_mutex_unlock(&game_running_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&game_running_mutex);

        //block and wait until input
        fd_set rfds;
        struct timeval tv;
        int retval;
        //watch stdin (fd 0) for input
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        retval = select(1, &rfds, NULL, NULL, &tv);

        if(retval == -1){
            //TODO: Create error handlers
            exit(EXIT_FAILURE);
        }else if(retval){
            key_pressed = getch();
        }

        //put the key into the input buffer
        pthread_mutex_lock(&input_buffer_mutex);
        input_buffer = key_pressed;
        key_pressed = 0;
        pthread_mutex_unlock(&input_buffer_mutex);

        //This thread doesn't sleep, instead it just blocks at getch()
    }
    return NULL;
}

void run_game(){

    //Try to initialize the console
    pthread_mutex_lock(&game_running_mutex);
    game_running = consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);
    if(!game_running){
        pthread_mutex_unlock(&game_running_mutex);
        consoleFinish();
        return;
    }
    pthread_mutex_unlock(&game_running_mutex);

    //initalize collision map
    fill_collision_map();

    //make cursor invisible
    curs_set(0);

    //Initialize the input buffer
    input_buffer = 0;

    //Create the player using the constants in game_globals.h
    create_player(PLAYER_START_ROW, PLAYER_START_COL, PLAYER_START_LIVES);

    //Start the threads
    pthread_create(&screen_thread, NULL, run_screen_thread, (void*)NULL);
    pthread_create(&upkeep_thread, NULL, run_upkeep_thread, (void*)NULL);
    pthread_create(&player_animation_thread, NULL, run_player_animation_thread, (void*)NULL);
    pthread_create(&player_movement_thread, NULL, run_player_movement_thread, (void*)NULL);
    pthread_create(&input_thread, NULL, run_input_thread, (void*)NULL);

    //Add the threads to the list of threads to join
    add_thread_to_list(screen_thread, 0);
    add_thread_to_list(upkeep_thread, 1);
    add_thread_to_list(player_animation_thread, 2);
    add_thread_to_list(player_movement_thread, 3);
    add_thread_to_list(input_thread, 4);

    //let the threads do their thing, wait until we're woken up
    pthread_mutex_lock(&game_cond_mutex);
    pthread_cond_wait(&game_cond, &game_cond_mutex);

    //create_player allocates the player so free it here
    free(player_ref);

    //Adding a thread to the list allocates it, so free each thread
    // here. Also join each thread at the same time
    join_free_thread_list();

    consoleFinish();
    clear_collision_map();
    fprintf(stdout, "\nGame Over\n");

    return;
}