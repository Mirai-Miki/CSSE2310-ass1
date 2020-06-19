/*
 * autoPlayer.c
 * Author: Michael Bossner
 *
 * This file contains all functions for an Automatic Players turn
 */

#include <stdio.h>

#include "autoPlayer.h"
#include "game.h"
#include "tilefile.h"

///////////////////////// Private Function Prototypes /////////////////////////

/* 
 * Automatic Player Type 1
 * Updates game state when a valid move is found.
 *
 * state: The current state of the game
 *
 * return: Returns 1 if a there is a valid move.
 *         Returns 0 if there is no valid move available.
 */
static int auto_type1(GameStateInfo* state);

/* 
 * Automatic Player Type 2 Player 1
 * Updates game state when a valid move is found.
 *
 * state: The current state of the game
 *
 * return: Returns 1 if a there is a valid move.
 *         Returns 0 if there is no valid move available.
 */
static int auto_type2_p1(GameStateInfo* state);

/* 
 * Automatic Player Type 2 Player 2
 * Updates game state when a valid move is found.
 *
 * state: The current state of the game
 *
 * return: Returns 1 if a there is a valid move.
 *         Returns 0 if there is no valid move available.
 */
static int auto_type2_p2(GameStateInfo* state);

//////////////////////////////// Functions ////////////////////////////////////

void process_ap(GameStateInfo* state) {
    if (state->player == PLAYER_1) {
        // Player 1s turn
        if (state->p1Type == APT1) {
            auto_type1(state);
        } else {
            auto_type2_p1(state);
        }
    } else {
        // Player 2s turn
        if (state->p2Type == APT1) {
            auto_type1(state);
        } else {
            auto_type2_p2(state);
        }
    }
    printf("Player %c => %d %d rotated %d\n", state->player, state->inst[COLM],
            state->inst[ROW], state->inst[ROTATE]);
}

////////////////////////////// Private Functions //////////////////////////////
//
static int auto_type1(GameStateInfo* state) {
    int r = state->inst[ROW];
    int c = state->inst[COLM];
    // rotate through each tile
    for (state->inst[ROTATE] = 0; state->inst[ROTATE] <= ROTATE_270; 
            state->inst[ROTATE] += ROTATE_90) {
        char** tile = rotate_tile(state->tile, state->inst[ROTATE], CALL);

        do {
            if (is_move_valid(tile, state, state->inst)) {
                return VALID;
            } else {
                // increment through each row
                state->inst[ROW]++;
                if (state->inst[ROW] > MAX_MOVE_R) {
                    // increment through each column
                    state->inst[ROW] = MIN_MOVE;
                    state->inst[COLM]++;
                }
                if (state->inst[COLM] > MAX_MOVE_C) {
                    state->inst[COLM] = MIN_MOVE;                       
                }
            }
        // stop when every move has been checked
        } while ((r != state->inst[ROW]) || (c != state->inst[COLM]));      
    }
    // no valid moves found
    return INVALID;
}

//
static int auto_type2_p1(GameStateInfo* state) {
    int rStart = state->instA2P1[ROW];
    int cStart = state->instA2P1[COLM];
    do {
        // rotate through each tile
        for (state->instA2P1[ROTATE] = 0; state->instA2P1[ROTATE] <= 
                ROTATE_270; state->instA2P1[ROTATE] += ROTATE_90) {
            char** tile = rotate_tile(state->tile, state->instA2P1[ROTATE],
                    CALL);
            if (is_move_valid(tile, state, state->instA2P1)) {
                // update the move instructions into the game state for use
                state->inst[COLM] = state->instA2P1[COLM];
                state->inst[ROW] = state->instA2P1[ROW];
                state->inst[ROTATE] = state->instA2P1[ROTATE];
                return VALID;
            }
        }
        // increment through each row
        state->instA2P1[ROW]++;
        if (state->instA2P1[ROW] > MAX_MOVE_R) {
            // increment through each column
            state->instA2P1[ROW] = MIN_MOVE;
            state->instA2P1[COLM]++;
        }
        if (state->instA2P1[COLM] > MAX_MOVE_C) {
            state->instA2P1[COLM] = MIN_MOVE;
        }
    // stop when every move has been checked      
    } while ((state->instA2P1[ROW] != rStart) || 
            (state->instA2P1[COLM] != cStart));
    // no valid moves found
    return INVALID;
}

//
static int auto_type2_p2(GameStateInfo* state) {
    int rStart = state->instA2P2[ROW];
    int cStart = state->instA2P2[COLM];
    do {
        // rotate through each tile
        for (state->instA2P2[ROTATE] = 0; state->instA2P2[ROTATE] <= 
                ROTATE_270; state->instA2P2[ROTATE] += ROTATE_90) {
            char** tile = rotate_tile(state->tile, state->instA2P2[ROTATE],
                    CALL);
            if (is_move_valid(tile, state, state->instA2P2)) {
                // update the move instructions into the game state for use
                state->inst[COLM] = state->instA2P2[COLM];
                state->inst[ROW] = state->instA2P2[ROW];
                state->inst[ROTATE] = state->instA2P2[ROTATE];
                return VALID;
            }
        }
        // increment through each row backwards         
        state->instA2P2[ROW]--;
        if (state->instA2P2[ROW] < MIN_MOVE) {
            // increment through each column backwards
            state->instA2P2[ROW] = MAX_MOVE_R;
            state->instA2P2[COLM]--;
        }
        if (state->instA2P2[COLM] < MIN_MOVE) {
            state->instA2P2[COLM] = MAX_MOVE_C;
        }
    // stop when every move has been checked    
    } while ((state->instA2P2[ROW] != rStart) || 
            (state->instA2P2[COLM] != cStart));
    // no valid moves found
    return INVALID;
}