/*
 * saveGame.c
 * Author: Michael Bossner
 *
 * This file contains all functions related to saving and loading a game
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "saveGame.h"
#include "error.h"
#include "parseFile.h"

#define LINE_1 0
#define INDEX 0
#define TURN 1
#define BOARD_ROW 3
#define BOARD_COLM 2
#define MIN_FILE_LINES 2
#define MIN_LINE_LEN 7
#define MAX_LINE_LEN 13
#define VALID_WORD_COUNT 3
#define NON_BOARD_LINES 2

//////////////////////// Private Function Prototypes //////////////////////////

/*
 * Checks the contents of a file to see if it is a valid save file
 *
 * splitFile: a container that holds the file split by lines
 *
 * state: The current state of the game
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * return: Returns true if the splitFile is a valid save file.
 *         Returns false if the splitFile is not a valid save file.
 */
static bool is_save_file_valid(FileCont* splitFile, GameStateInfo* state, 
        LoadedTilefile* loadedFile);
/*
 * Checks the first line of a split file to see if it is a valid line.
 * Updates the game state with the information in line 1
 *
 * line1: The first line of the save file
 *
 * state: The current state of the game
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * return: Returns 0 if the line is Invalid || 1 if the line is valid.
 */
static int check_line1(char* line1, GameStateInfo* state, 
        LoadedTilefile* loadedFile);
/*
 * Checks the contents of the save file to see if the board is valid.
 * Creates a board from the save file and stores it in the game state.
 *
 * splitFile: a container that holds the file split by lines
 *
 * state: The current state of the game
 *
 * return: Returns 0 if the board is Invalid || 1 if the board is valid
 */
static int load_board(FileCont* splitFile, GameStateInfo* state);

//////////////////////////////// Functions ////////////////////////////////////

int save_game(char* fileName, GameStateInfo* state) {
    FILE* saveFile = fopen(fileName, "w");
    if (saveFile == NULL) {
        return INVALID;
    }

    fprintf(saveFile, "%d %d %d %d\n", state->tileIndex, state->turn, 
            state->height, state->width);
    // writes the board to the save file
    for (int colm = 0; colm < state->height; colm++) {
        for (int row = 0; row < state->width; row++) {
            fprintf(saveFile, "%c", state->board[colm][row]);
        }
        fprintf(saveFile, "\n");
    }
    fclose(saveFile);
    return VALID;
}

int load_game(char* fileName, GameStateInfo* state,
        LoadedTilefile* loadedFile) {
    FILE* saveFile = fopen(fileName, "r");
    FileCont splitFile;
    if (saveFile == NULL) {
        free_loaded_tiles(loadedFile);
        error_6();
    } else {
        split_file(saveFile, &splitFile);
        if (!is_save_file_valid(&splitFile, state, loadedFile)) {
            // invalid file contents
            error_7();
        }
    }

    fclose(saveFile);
    return VALID;
}

////////////////////////////// Private Functions //////////////////////////////
//
static bool is_save_file_valid(FileCont* splitFile, GameStateInfo* state, 
        LoadedTilefile* loadedFile) {
    if ((splitFile->sizeOfOut < MIN_FILE_LINES) || 
            (strlen(splitFile->output[LINE_1]) < MIN_LINE_LEN) ||
            (strlen(splitFile->output[LINE_1]) > MAX_LINE_LEN)) {
        return false;
    } else {        
        if (!check_line1(splitFile->output[LINE_1], state, loadedFile) ||
                !load_board(splitFile, state)) {
            return false;
        }        
    }
    return true;
}

//
static int check_line1(char* line1, GameStateInfo* state, 
        LoadedTilefile* loadedFile) {
    int maxLen = strlen(line1);
    char parsedLine[maxLen][maxLen];
    int count = 0;
    // Splits line 1 into space separated words and stores it in parsedLine
    for (int str = 0; ; str++) {
        for (int i = 0; ; i++) {
            if (line1[count] == ' ' || line1[count] == '\0') {
                parsedLine[str][i] = '\0';
                break;
            } else {
                // Checks if each word is a digit
                if (!isdigit(line1[count])) {
                    return INVALID;
                }
                parsedLine[str][i] = line1[count];
            }
            count++;
        }
        // only valid if there are 4 words when the line ends
        if (str == VALID_WORD_COUNT && line1[count] == '\0') {
            break;
        } else if (line1[count] == '\0') {
            return INVALID;
        } else {
            count++;
        }
    }
    // checks consistency and stores into the game state
    state->tileIndex = atoi(parsedLine[INDEX]);
    if (state->tileIndex > loadedFile->size) {
        return INVALID;
    }
    state->turn = atoi(parsedLine[TURN]);
    if ((state->turn != P1) && (state->turn != P2)) {
        return INVALID;
    }
    state->width = atoi(parsedLine[BOARD_ROW]);
    state->height = atoi(parsedLine[BOARD_COLM]);
    return VALID;
}

//
static int load_board(FileCont* splitFile, GameStateInfo* state) {
    if (state->height != (splitFile->sizeOfOut - NON_BOARD_LINES)) {
        return INVALID;
    }
    // Create board from save file and store it in the game state
    state->board = malloc(sizeof(char*) * state->height);
    for (int colm = 0; colm < state->height; colm++) {
        state->board[colm] = malloc(sizeof(char) * state->width);           
        if (state->width != strlen(splitFile->output[colm + 1])) {
            return INVALID;
        } else {
            for (int row = 0; row < state->width; row++) {
                state->board[colm][row] = splitFile->output[colm + 1][row];
            }
        }
    }
    return VALID;
}