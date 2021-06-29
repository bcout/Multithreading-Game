/********************************************************************
    Module: threads_mutexes.h
    Author: Brennan Couturier

    All the threads and mutexes available globally
********************************************************************/

#include <pthread.h>

/********************************************************************
All the main threads that are needed. The caterpillar and bullet 
    threads are made on the fly and are assigned to each individual
    bullet/caterpillar. They are not global variables like these.
********************************************************************/
pthread_t player_animation_thread;
pthread_t player_movement_thread;
pthread_t keyboard_thread;
pthread_t screen_thread;
pthread_t upkeep_thread;
pthread_t caterpillar_spawn_thread;
pthread_t input_thread;

/********************************************************************
All the mutexes that are needed. Each caterpillar/bullet has its own
    mutex, assigned when the bullet/caterpillar is made
********************************************************************/
extern pthread_mutex_t player_mutex;
extern pthread_mutex_t screen_mutex;
extern pthread_mutex_t caterpillar_list_mutex;
extern pthread_mutex_t bullet_list_mutex;
extern pthread_mutex_t thread_list_mutex;
extern pthread_mutex_t game_cond_mutex;
extern pthread_mutex_t game_running_mutex;
extern pthread_mutex_t collision_map_mutex;

/********************************************************************
Function that starts the game up then sleeps on a condition variable
********************************************************************/
void run_game();
