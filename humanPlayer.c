/*
 * humanPlayer.c
 * Author: Michael Bossner
 *
 * This file contains all functions that involve a human players turn
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "humanPlayer.h"
#include "game.h"
#include "error.h"
#include "saveGame.h"
#include "parseFile.h"

#define ZERO 48
#define NINE 57
#define FILE_NAME 0
#define SAVE_INPUT 1
#define MOVE_INPUT 3
#define S 0
#define A 1
#define V 2
#define E 3

///////////////////////// Private Function Prototypes /////////////////////////

/*
 * Checks the input given is a valid command for the game.
 * Updates the game state if the input is a valid move command.
 * Saves the game if the input is a valid save command
 *
 * sizeOfOut: The number of words in the input provided
 *
 * input: All the words from the input
 *
 * state: The current state of the game
 *
 * return: Returns 1 if the input is a valid command.
 *         Returns 0 if the input is not a valid command.
 *
 * err_save_fail: The game could not be saved
 */
static int is_input_valid(int sizeOfOut, char** input, GameStateInfo* state);

/*
 * Checks if the input is a valid move command.
 * Updates the game state if it is a valid move command.
 *
 * input: All the words from the input
 *
 * state: The current state of the game
 *
 * return: Returns 1 if the input is a valid move command.
 *         Returns 0 if the input is not a valid move command.
 */
static int check_move_input(char** input, GameStateInfo* state);

/*
 * Checks if the input is a valid save command.
 *
 * input: All the words from the input
 *
 * return: Returns 1 if the input is a valid save command.
 *         Returns 0 if the input is not a valid save command.
 */
static int check_save_input(char* input);

//////////////////////////////// Functions ////////////////////////////////////

void process_h(GameStateInfo* state) {
    // keep asking for input until a valid command is given or EOF is received
    FOREVER {
        printf("Player %c] ", state->player);
        FileCont splitStdIn;
        // ask for input
        split_stdin(&splitStdIn);
        // check input
        if (is_input_valid(splitStdIn.sizeOfOut, splitStdIn.output, state)) {
            char** tile = rotate_tile(state->tile, state->inst[ROTATE], CALL);
            // input is valid check move instructions
            if (is_move_valid(tile, state, state->inst)) {
                break;
            }           
        }
    } 
}

////////////////////////////// Private Functions //////////////////////////////
//
static int is_input_valid(int sizeOfOut, char** input, GameStateInfo* state) {
    switch (sizeOfOut) {
        case SAVE_INPUT:
            if (!check_save_input(input[FILE_NAME])) {
                return INVALID;
            } else {                
                if (!save_game(&input[FILE_NAME][SAVE_NAME_START], state)) {
                    err_save_fail();
                }
                // Game saved send invalid back to prompt for another input
                return INVALID;
            }
        case MOVE_INPUT:
            if (!check_move_input(input, state)) {
                return INVALID;
            }
            break;
        default:
            // input size is invalid
            return INVALID;
    }
    return VALID;
}

//
static int check_move_input(char** input, GameStateInfo* state) {
    // check each string in the move instruction
    for (int i = 0; i < INST_MAX; i++) {
        // check each character in each string
        for (int r = 0; r < strlen(input[i]); r++) {
            if ((input[i][r] < ZERO || input[i][r] > NINE) 
                    && (input[i][r] != '-')) {
                // string is not a valid digit
                return INVALID;
            }
        }
    }
    // move instructions are valid digits
    state->inst[COLM] = atoi(input[COLM]);
    state->inst[ROW] = atoi(input[ROW]);
    state->inst[ROTATE] = atoi(input[ROTATE]);
    // checking if the digits are valid move commands
    if (state->inst[COLM] < MIN_MOVE || state->inst[COLM] > MAX_MOVE_C) {
        return INVALID;
    } else if (state->inst[ROW] < MIN_MOVE || state->inst[ROW] > MAX_MOVE_R) {
        return INVALID;
    } else if (state->inst[ROTATE] != 0 && state->inst[ROTATE] != 
            ROTATE_90 && state->inst[ROTATE] != ROTATE_180 && 
            state->inst[ROTATE] != ROTATE_270) {
        return INVALID;
    } else {
        return VALID;
    }
}

//
static int check_save_input(char* input) {
    int stringLen = strlen(input);
    if (stringLen > SAVE_NAME_START) {
        if ((input[S] == 's') && (input[A] == 'a') && 
                (input[V] == 'v') && (input[E] == 'e')) {
            return VALID;
        }
    }
    return INVALID;
}