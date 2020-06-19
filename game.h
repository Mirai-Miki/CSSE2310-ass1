/*
 * game.h
 * Author: Michael Bossner
 *
 * Header file for game.c
 */

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "tilefile.h"

#define FOREVER for (;;)
#define EXIT 0
#define MIN_MOVE -2
#define MAX_MOVE_C (state->height + 2)
#define MAX_MOVE_R (state->width + 2)
#define INST_MAX 3
#define COLM 0
#define ROW 1
#define ROTATE 2
#define PLAYER_1 '*'
#define PLAYER_2 '#'
#define INVALID 0
#define VALID 1
#define NEW_GAME 6
#define LOAD_GAME 5
#define P1 0
#define P2 1

typedef struct GameStateInfo GameStateInfo;

/*
 * Contains all relevant information about the current state of the game
 */
struct GameStateInfo {
    char p1Type; // player 1s Type
    char p2Type; // player 2s Type
    int turn;    // Player to have there turn
    char player; // Current players name
    /* Instructions for the move to be made */
    int inst[INST_MAX], instA2P1[INST_MAX], instA2P2[INST_MAX];
    int height; // height of the board
    int width; // width of the board
    char** board; // The game board
    char** tile; // copy of the current tile to be used
    int tileIndex; // index of the current tile to be used
};

/*
 * Initializes the game state with information to be used during play.
 * Then runs the Game loop
 *
 * state: The current state of the game
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * gameType: The game type specifies what type of game you are starting. 
 *         Either 6 for a New Game || 5 for game loaded from a save file.
 *
 * returns: Returns 0 when completed
 */
int init_game(GameStateInfo* state, LoadedTilefile* loadedFile, int gameType);

/*
 * Checks whether a tile and a set of move instructions is allowed to be 
 * placed on the current board state.
 *
 * tile: The tile to be placed on the board
 *
 * state: The current state of the game
 *
 * inst: A set of movement instruction to be used
 *
 * return: Returns false if the tile provided cannot be placed on the board
 *         with the instructions provided. Else true is returned.
 *
 * error_10: EOF is received while waiting for input from stdin
 *
 * err_save_fail: The game could not be saved
 */
bool is_move_valid(char** tile, GameStateInfo* state, int* inst);

#endif