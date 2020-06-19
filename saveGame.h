/*
 * saveGame.h
 * Author: Michael Bossner
 *
 * Header file for saveGame.c
 */

#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#define SAVE_NAME_START 4

#include "game.h"
#include "tilefile.h"

/*
 * Saves the current state of the game into a file.
 *
 * fileName: Name of the file to save the game too. If no file already exists
 *         with that name a new one will be made else the old file will be
 *         overwritten.
 *
 * state: The current state of the game
 *
 * return: Returns 0 if the file cannot be opened to save too. 
 *         Returns 1 when the game is saved. 
 */
int save_game(char* fileName, GameStateInfo* state);

/*
 * Attempts to load a save game file. Checks the file to see if it is valid.
 * Updates the game state with the information contained in the file.
 *
 * fileName: Name of the save game file to be loaded
 *
 * state: The current state of the game
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * return: Returns 1 if the file has been successfully loaded
 *
 * error_6: The save file cannot be accessed. Game ends.
 *
 * error_7: the contents of the file is not a valid save file. Game ends.
 */
int load_game(char* fileName, GameStateInfo* state, 
        LoadedTilefile* loadedFile);

#endif