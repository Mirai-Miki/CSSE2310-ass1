/*
 * autoPlayer.h
 * Author: Michael Bossner
 *
 * Header file for autoPlayer.c
 */

#ifndef AUTO_PLAYER_H
#define AUTO_PLAYER_H

#include "game.h"

#define APT1 '1'
#define APT2 '2'

/*
 * Processes a computer players turn. The way a computer player chooses it's
 * next move is based of it's player type and for the case of type 2 whether
 * or not it is the first player or the second.
 * Updates the game state when a valid move is found.
 *
 * state: The current state of the game
 */
void process_ap(GameStateInfo* state);

#endif