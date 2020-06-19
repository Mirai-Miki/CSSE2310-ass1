/*
 * tilefile.c
 * Author: Michael Bossner
 *
 * This file contains all functions related to the tilefile
 */

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "tilefile.h"
#include "error.h"

#define MIN_TILES 1
#define ROW_MAX 6
#define COLOMN_MAX 5

#define TILE_END 1
#define FILE_END 0


///////////////////////// Private Function Prototypes /////////////////////////

/*
 * adds a tile from the tilefile and stores a copy of it in loadedFile
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * tilefile: The file to be read from
 *
 * return: Returns 1 if the tile has ended. Returns 0 if the file has ended. 
 */
static int add_tile(LoadedTilefile* loadedFile, FILE* tilefile);

/*
 * Frees a tiles storage from memory use.
 *
 * tile: Tile to be freed from memory use
 */
static void free_tile(char** tile);

/*
 * Checks a character in a tile for correctness.
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * tilefile: The file to be read from
 *
 * next: character in the tile to be checked
 *
 * row: row position of the character
 *
 * colm: column position of the character
 *
 * error_3: If the tile does not meet the definition of a tile.
 *         5x5 grid with either ('.' || '!') && ('\n' terminated)
 */
static void file_check(LoadedTilefile* loadedFile, int next, int row, 
        int colm);

/*
 * Prints a tile and all rotations of the tile space separated.
 * tile   t90  t180  t270
 * !,,,, ,,,,! ,,,,, ,,,,,
 * ,,,,, ,,,,, ,,,,, ,,,,,
 * ,,,,, ,,,,, ,,,,, ,,,,,
 * ,,,,, ,,,,, ,,,,, ,,,,,
 * ,,,,, ,,,,, ,,,,! !,,,,
 *
 * tile: tile to be printed
 *
 * tileRotated90: tile rotated 90 degrees to be printed
 *
 * tileRotated180: tile rotated 180 degrees to be printed
 *
 * tileRotated270: tile rotated 270 degrees to be printed
 *
 * return: Returns 0 when completed
 */
static int print_tiles(char** tile, char** tileRotated90, 
        char** tilerotated180, char** tileRotated270);

/*
 * Creates storage for a tile to be copied too.
 * 
 * return: Returns the location of the tile storage
 */
static char** alloc_tile(void);

//////////////////////////////// Functions ////////////////////////////////////

int load_tilefile(LoadedTilefile* loadedFile) {
    FILE* tilefile = fopen(loadedFile->tilefileName, "r");
    if (tilefile == NULL) {
        // file cannot be opened
        error_2();
    }

    loadedFile->size = 0;
    // Creates storage for a single tile
    loadedFile->loadedTiles = malloc(sizeof(char**) * MIN_TILES);   
    loadedFile->loadedTiles[loadedFile->size] = alloc_tile();
    // Adds all tiles to the loadedFile storage and keeps count
    while (add_tile(loadedFile, tilefile)) {
        loadedFile->size++;
        loadedFile->loadedTiles = realloc(loadedFile->loadedTiles, 
                sizeof(char**) * (loadedFile->size + 1));
        loadedFile->loadedTiles[loadedFile->size] = alloc_tile();
    }

    fclose(tilefile);
    return EXIT;
}

int display_tilefile(LoadedTilefile* loadedFile) {
    // goes through ever tile loaded into loadedFile.
    for (int i = 0; i <= loadedFile->size; i++) {
        // rotates the current tile to all angles then prints the tiles
        char** tileRotated90 = rotate_tile(loadedFile->loadedTiles[i],
                ROTATE_90, CALL);
        char** tileRotated180 = rotate_tile(loadedFile->loadedTiles[i],
                ROTATE_180, CALL);
        char** tileRotated270 = rotate_tile(loadedFile->loadedTiles[i],
                ROTATE_270, CALL);
        print_tiles(loadedFile->loadedTiles[i], tileRotated90, tileRotated180,
                tileRotated270);
        if (i < loadedFile->size) {
            printf("\n");
        }       
        free_tile(tileRotated90);
        free_tile(tileRotated180);
        free_tile(tileRotated270);
    }
    return EXIT;
}

char** rotate_tile(char** tile, int angle, int call) {
    if (angle > 0) {
        char** tileRotated = alloc_tile();
        // Rotates the original tile 90 degrees clockwise skipping the '\0'
        int row = (COLOMN_MAX - 1);
        for (int colm = 0; colm < COLOMN_MAX; colm++) {

            for (int i = 0; i < ROW_MAX; i++) {
                if (i == (ROW_MAX - 1)) {
                    // adds the '\0' to the end of each line of the new tile
                    tileRotated[colm][i] = '\0';
                } else {
                    tileRotated[i][row] = tile[colm][i];
                }
            }   
            row--;
        }
        // We want to free any unused tiles while leaving the original and
        // final tile untouched
        if (call) {
            free_tile(tile);
        }
        angle -= ROTATE_90;
        call++;
        // Call the function again if the tile needs to be rotated further.
        return rotate_tile(tileRotated, angle, call);
    } else {
        return tile;
    }
}

int free_loaded_tiles(LoadedTilefile* loadedFile) {
    // frees memory in a 3D array
    for (; loadedFile->size >= 0; loadedFile->size--) {
        
        for (int i = 0; i < COLOMN_MAX; i++) {
            free(loadedFile->loadedTiles[loadedFile->size][i]);
        }
        free(loadedFile->loadedTiles[loadedFile->size]);
    }
    free(loadedFile->loadedTiles);
    return EXIT;
}

void print_tile(LoadedTilefile* loadedFile) {
    for (int colm = 0; colm < COLOMN_MAX; colm++) {
        printf("%s\n", loadedFile->loadedTiles[loadedFile->index][colm]);
    }
}

////////////////////////////// Private Functions //////////////////////////////
//
static int add_tile(LoadedTilefile* loadedFile, FILE* tilefile) {
    int next;
    int row = 0;
    int colomn = 0;
    // Adds the tile to state unless the file is invalid
    FOREVER {
        next = fgetc(tilefile);
        file_check(loadedFile, next, row, colomn);
        if (next == EOF) {
            return FILE_END;
        } else if (colomn >= COLOMN_MAX) {
            return TILE_END;
        } else if (next == '\n') {
            // end of line. add null terminator and move to next line
            loadedFile->loadedTiles[loadedFile->size][colomn][row] = '\0';
            row = 0;            
            colomn++;
        } else {
            loadedFile->loadedTiles[loadedFile->size][colomn][row] = next;
            row++;
        }   
    }
}

//
static void free_tile(char** tile) {
    for (int i = 0; i < COLOMN_MAX; i++) {
        free(tile[i]);
    }
    free(tile);
}

//
static void file_check(LoadedTilefile* loadedFile, int next, int row, 
        int colm) {
    if (row == (ROW_MAX - 1) && next != '\n') {
        // tile line is not the correct size
        free_loaded_tiles(loadedFile);
        error_3();
    } else if ((row < (ROW_MAX - 1) && colm < COLOMN_MAX) && 
            (next != ',' && next != '!')) {
        // Tile does not contain the correct format
        free_loaded_tiles(loadedFile);
        error_3();
    } else if (colm >= COLOMN_MAX && (next != EOF && next != '\n')) {
        // tile height is not the correct size
        free_loaded_tiles(loadedFile);
        error_3();
    }
}

//
static int print_tiles(char** tile, char** tileRotated90, 
        char** tilerotated180, char** tileRotated270) {
    // prints every line of the tiles
    for (int i = 0; i < COLOMN_MAX; i++) {
        printf("%s %s %s %s\n", tile[i], tileRotated90[i], 
                tilerotated180[i], tileRotated270[i]);
    }
    return EXIT;
}

//
static char** alloc_tile(void) {
    char** tile = malloc(sizeof(char*) * COLOMN_MAX);
    for (int i = 0; i < COLOMN_MAX; i++) {
        tile[i] = malloc(sizeof(char) * ROW_MAX);
    }
    return tile;
}