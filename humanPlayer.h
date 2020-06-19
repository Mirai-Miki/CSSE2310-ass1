/*
 * humanPlayer.h
 * Author: Michael Bossner
 *
 * Header file for humanPlayer.c
 */

#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "game.h"

#define HUMAN 'h'

/*
 * Processes the human players turn.
 * Checks if input given by the human is a valid command.
 * Updates the game state if the input is a valid move command.
 * Saves the game if the input is a valid save command.
 *
 * state: The current state of the game
 * 
 * error_10: EOF is received while waiting for input from stdin
 *
 * err_save_fail: The game could not be saved
 */
void process_h(GameStateInfo* state);

#endif