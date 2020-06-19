/*
 * main.c
 * Author: Michael Bossner
 *
 * This file contains the main function and handles the set up of the program.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "game.h"
#include "error.h"
#include "tilefile.h"
#include "saveGame.h"
#include "humanPlayer.h"
#include "autoPlayer.h"

#define DISPLAY_TILEFILE 2
#define ARGV_TILEFILE 1
#define ARGV_P1_TYPE 2
#define ARGV_P2_TYPE 3
#define ARGV_HEIGHT 4
#define ARGV_WIDTH 5
#define ARGV_SAVE_FILE 4
#define MAX_BOARD_SIZE 999
#define PLAYER_TYPE_LEN 1
#define PLAYER_TYPE_INDEX 0

///////////////////////// Private Function Prototypes /////////////////////////

/*
 * Assigns the height and width of the board to the games state.
 *
 * height: The amount of columns the board contains
 *
 * width: Amount of rows the board contains
 *
 * state: The current state of the game
 *
 * error_5: The dimensions of the board must not be (<= 0 || > 999)
 */
static void assign_dimensions(char* height, char* width, GameStateInfo* state);

/*
 * Assigns the player types to the games state.
 * 
 * p1: Player 1s type
 *
 * p2: Players 2s type
 *
 * state: The current state of the game
 *
 * error_4: The player types must equal either ('h' || '1' || '2')
 */
static void assign_player_type(char* p1, char* p2, GameStateInfo* state);

//////////////////////////////// Functions ////////////////////////////////////

int main(int argc, char** argv) {
    GameStateInfo state;
    LoadedTilefile loadedFile;
    switch (argc) {
        case NEW_GAME:            
            loadedFile.tilefileName = argv[ARGV_TILEFILE];
            load_tilefile(&loadedFile);
            assign_player_type(argv[ARGV_P1_TYPE], argv[ARGV_P2_TYPE],
                    &state);
            assign_dimensions(argv[ARGV_HEIGHT], argv[ARGV_WIDTH], &state);
            init_game(&state, &loadedFile, NEW_GAME);
            break;

        case LOAD_GAME:
            loadedFile.tilefileName = argv[ARGV_TILEFILE];
            load_tilefile(&loadedFile);
            assign_player_type(argv[ARGV_P1_TYPE], argv[ARGV_P2_TYPE], 
                    &state);
            load_game(argv[ARGV_SAVE_FILE], &state, &loadedFile);
            loadedFile.index = state.tileIndex;
            // assigns players name
            if (state.turn == P1) {
                state.player = PLAYER_1;
            } else {
                state.player = PLAYER_2;
            }
            init_game(&state, &loadedFile, LOAD_GAME);
            break;

        case DISPLAY_TILEFILE:
            loadedFile.tilefileName = argv[ARGV_TILEFILE];
            load_tilefile(&loadedFile);
            display_tilefile(&loadedFile);
            break;

        default:
            // incorrect number of arguments
            error_1();
    }
    free_loaded_tiles(&loadedFile);
    return EXIT;
}

////////////////////////////// Private Functions //////////////////////////////
//
static void assign_dimensions(char* height, char* width, 
        GameStateInfo* state) {    
    state->height = atoi(height);
    state->width = atoi(width);
    if ((state->height <= 0 || state->height > MAX_BOARD_SIZE) || 
            (state->width <= 0 || state->width > MAX_BOARD_SIZE)) {
        // Board dimension are invalid
        error_5();
    }
}

//
static void assign_player_type(char* p1, char* p2, GameStateInfo* state) {
    state->p1Type = p1[PLAYER_TYPE_INDEX];
    state->p2Type = p2[PLAYER_TYPE_INDEX];
    if ((state->p1Type != HUMAN && state->p1Type != APT1 &&
            state->p1Type != APT2) || (state->p2Type != HUMAN && 
            state->p2Type != APT1 && state->p2Type != APT2) || 
            (strlen(p1) != PLAYER_TYPE_LEN || strlen(p2) != PLAYER_TYPE_LEN)) {
        // player type is invalid
        error_4();
    }
}