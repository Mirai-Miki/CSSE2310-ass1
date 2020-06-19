/*
 * game.c
 * Author: Michael Bossner
 * 
 * This file contains the games main loop and handles the core functions
 * of the game.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "game.h"
#include "humanPlayer.h"
#include "autoPlayer.h"
#include "saveGame.h"

#define SAVED 1
#define TILE_ROW_MAX 4
#define TILE_COLM_MAX 4

///////////////////////// Private Function Prototypes /////////////////////////

/*
 * The main game loop. Once called does not end until the game is over either
 * by someone winning or an error occurring.
 *
 * state: The current state of the game
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * return: returns a 0 when someone has won the game
 *
 * error_10: EOF is received while waiting for input from stdin. Game ends.
 *
 * err_save_fail: The game could not be saved. This does not end the game.
 */
static int game_loop(GameStateInfo* state, LoadedTilefile* loadedFile);

/*
 * Creates the game board to be played on and stores it in the games state.
 * Size of the board is contained in the current game state.
 *
 * state: The current state of the game
 */
static void create_board(GameStateInfo* state);

/*
 * Prints a copy of the current state of the game board to the console.
 *
 * state: The current state of the game
 */
static void print_board(GameStateInfo* state);

/*
 * Moves to the next tile to be used for play and updates the current
 * state of the game. If there are no more tiles to be used the first tile
 * in the loadedFile will be selected.
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * state: The current state of the game
 */
static void increment_tiles(LoadedTilefile* loadedFile, GameStateInfo* state);

/*
 * Adds the last move made to the board and updates the game state.
 * This function does not care if the move is not a valid move.
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * state: The current state of the game
 *
 * return: Returns 0 when completed
 */
static int update_board(GameStateInfo* state);

/*
 * Checks if there is a valid move available for the next tile to be used.
 *
 * state: The current state of the game
 *
 * return: Returns true if the the next tile cannot be placed on the board.
 *         Returns false if the next tile can be placed on the board.
 */
static bool is_game_over(GameStateInfo* state);

//////////////////////////////// Functions ////////////////////////////////////

int init_game(GameStateInfo* state, LoadedTilefile* loadedFile, int gameType) {
    switch (gameType) {
        case NEW_GAME:
            create_board(state);
            loadedFile->index = 0;
            state->tileIndex = loadedFile->index;
            state->player = PLAYER_1;
            state->turn = 0;         

        case LOAD_GAME:    
            state->inst[COLM] = MIN_MOVE;
            state->inst[ROW] = MIN_MOVE;
            state->inst[ROTATE] = 0;
            state->instA2P1[COLM] = MIN_MOVE;
            state->instA2P1[ROW] = MIN_MOVE;
            state->instA2P1[ROTATE] = 0;
            state->instA2P2[COLM] = MAX_MOVE_C;
            state->instA2P2[ROW] = MAX_MOVE_R;
            state->instA2P2[ROTATE] = 0;
            state->tile = loadedFile->loadedTiles[loadedFile->index];
    }
    game_loop(state, loadedFile);
    return EXIT;
}

bool is_move_valid(char** tile, GameStateInfo* state, int* inst) {
    int y = MIN_MOVE;
    int x = MIN_MOVE;
    // looking for a '!' in the tile to test
    for (int tileColm = 0; tileColm <= TILE_COLM_MAX; tileColm++) {
        for (int tileRow = 0; tileRow <= TILE_ROW_MAX; tileRow++) {
            if (tile[tileColm][tileRow] == '!') {               
                // Checking if '!' is out of bounds
                if ((inst[COLM] + y) < 0 
                        || ((inst[ROW] + x) < 0)
                        || ((inst[COLM] + y) >= state->height) 
                        || ((inst[ROW] + x) >= state->width)) {             
                    return false;
                // checking if '!' overrides another move
                } else if (state->board[(inst[COLM] + y)]
                        [(inst[ROW] + x)] == PLAYER_1 || 
                        (state->board[(inst[COLM] + y)]
                        [(inst[ROW] + x)] == PLAYER_2)) {                   
                    return false;
                }
            }
            x++;
        }
        x = MIN_MOVE;
        y++;
    }
    return true;
}

////////////////////////////// Private Functions //////////////////////////////
//
static int game_loop(GameStateInfo* state, LoadedTilefile* loadedFile) {
    FOREVER {
        print_board(state);
        // check for game over
        if (is_game_over(state)) {
            printf("Player %c wins\n", state->player);
            return EXIT;
        }
        // get instructions for turn
        if (!state->turn) {
            // Player 1s turn
            state->player = PLAYER_1;           
            if (state->p1Type == 'h') {
                print_tile(loadedFile);
                process_h(state);
            } else {
                process_ap(state);              
            }
            state->turn = P2;
        } else {
            // Player 2s turn
            state->player = PLAYER_2;
            if (state->p2Type == 'h') {
                print_tile(loadedFile);
                process_h(state);
            } else {
                process_ap(state);              
            }
            state->turn = P1;
        }
        // update board and move to the next tile in the game
        update_board(state);
        increment_tiles(loadedFile, state);
    }
}

//
static void create_board(GameStateInfo* state) {
    // allocate memory the size of the board
    state->board = malloc(sizeof(char*) * state->height);
    for (int i = 0; i < state->height; i++) {
        state->board[i] = malloc(sizeof(char) * state->width);
    }
    // place a '.' in all positions on the board
    for (int colm = 0; colm < state->height; colm++) {
        for (int row = 0; row < state->width; row++) {
            state->board[colm][row] = '.';
        }
    }
}

//
static void print_board(GameStateInfo* state) {
    // prints every character contained on the board
    for (int colm = 0; colm < state->height; colm++) {
        for (int row = 0; row < state->width; row++) {
            printf("%c", state->board[colm][row]);
        }
        // new line after every row is printed
        printf("\n");
    }
}

//
static void increment_tiles(LoadedTilefile* loadedFile, GameStateInfo* state) {
    if (loadedFile->index >= loadedFile->size) {
        loadedFile->index = 0;
    } else {
        loadedFile->index++;
    }
    state->tile = loadedFile->loadedTiles[loadedFile->index];
    state->tileIndex = loadedFile->index;
}

//
static int update_board(GameStateInfo* state) {
    char** tile = rotate_tile(state->tile, state->inst[ROTATE], CALL);
    int y = MIN_MOVE;
    int x = MIN_MOVE;
    // looking for a '!' on the tile then adding it to the board
    for (int tileColm = 0; tileColm <= TILE_COLM_MAX; tileColm++) {
        for (int tileRow = 0; tileRow <= TILE_ROW_MAX; tileRow++) {
            if (tile[tileColm][tileRow] == '!') {
                state->board[state->inst[COLM] + y][state->inst[ROW] + x] =
                        state->player;
            }
            x++;
        }
        x = MIN_MOVE;
        y++;        
    }
    return EXIT;
}

//
static bool is_game_over(GameStateInfo* state) {
    int inst[INST_MAX];
    // increments through all tile rotations
    for (inst[ROTATE] = 0; inst[ROTATE] <= ROTATE_270; 
            inst[ROTATE] += ROTATE_90) {
        char** rotatedTile = rotate_tile(state->tile, inst[ROTATE], CALL);
        // increments through every column on the board
        for (inst[COLM] = MIN_MOVE; inst[COLM] < MAX_MOVE_C;
                inst[COLM]++) {
            // increments through every row on the board
            for (inst[ROW] = MIN_MOVE; 
                    inst[ROW] < MAX_MOVE_R; inst[ROW]++) {
                if (is_move_valid(rotatedTile, state, inst)) {
                    // there is a valid move available
                    return false;
                }
            }
        }
    }
    // no valid moves are available
    return true;
}