/*
 * tilefile.h
 * Author: Michael Bossner
 *
 * Header file for tilefile.c
 */

#ifndef TILEFILE_H
#define TILEFILE_H

#define CALL 0
#define ROTATE_90 90
#define ROTATE_180 180
#define ROTATE_270 270

typedef struct LoadedTilefile LoadedTilefile;

/*
 * Contains all tiles loaded from a tilefile as well as information about the
 * tilefile such as how many tile there are and the name of the file.
 */
struct LoadedTilefile {
    char* tilefileName; // Name of the tilefile
    char*** loadedTiles; // All tiles copied into memory ready for use in game
    char** tileRotated; // Storage for a tile that has been rotated
    int size; // How many tiles are stored in the tilefile
    int index; // Current tile selected for use

};

/*
 * Copies all tiles from the tilefile into memory ready for use in the game.
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * return: Returns 0 when completed
 *
 * error_2: The tilefile cannot be opened. Game ends.
 *
 * error_3: If the tile does not meet the definition of a tile.
 *         5x5 grid with either ('.' || '!') && ('\n' terminated). Game ends.
 */
int load_tilefile(LoadedTilefile* loadedFile);

/*
 * Prints all tiles loaded from the tilefile and there rotations to the console
 * Rotations will be space separated from the original tile
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * return: Returns 0 when completed
 */
int display_tilefile(LoadedTilefile* loadedFile);

/*
 * Rotates a tile clockwise in 90 degree increments to the specified angle.
 *
 * tile: Tile to be rotated
 *
 * angle: Angle the tile is to be rotated too. Must be in 90 degree increments
 *         starting at 0
 *
 * call: specifies that the function is being called. Must always be 0
 *
 * return: Returns the memory location of the rotated tile
 */
char** rotate_tile(char** tile, int angle, int call);

/*
 * Frees the memory holding all tiles loaded from the tilefile
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 *
 * return: Returns 0 when completed
 */
int free_loaded_tiles(LoadedTilefile* loadedFile);

/*
 * Prints the current tile that is ready to be used for play to the console
 *
 * loadedFile: Information about the tilefile as well as a copy of all tiles
 */
void print_tile(LoadedTilefile* loadedFile);

#endif