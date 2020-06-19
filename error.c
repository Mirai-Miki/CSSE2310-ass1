/*
 * error.c
 * Author: Michael Bossner
 * 
 * Error.c is a file that contains all errors that the program should handle.
 */

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "tilefile.h"

#define ERR_1 1
#define ERR_2 2
#define ERR_3 3
#define ERR_4 4
#define ERR_5 5
#define ERR_6 6
#define ERR_7 7
#define ERR_10 10

void error_1(void) {
    fprintf(stderr, "Usage: fitz tilefile [p1type p2type "
            "[height width | filename]]\n");
    exit(ERR_1);
}

void error_2(void) {
    fprintf(stderr, "Can't access tile file\n");
    exit(ERR_2);
}

void error_3(void) {
    fprintf(stderr, "Invalid tile file contents\n");
    exit(ERR_3);
}

void error_4(void) {
    fprintf(stderr, "Invalid player type\n");
    exit(ERR_4);
}

void error_5(void) {
    fprintf(stderr, "Invalid dimensions\n");
    exit(ERR_5);
}

void error_6(void) {
    fprintf(stderr, "Can't access save file\n");
    exit(ERR_6);
}

void error_7(void) {
    fprintf(stderr, "Invalid save file contents\n");
    exit(ERR_7);
}

void error_10(void) {
    fprintf(stderr, "End of input\n");
    exit(ERR_10);
}

void err_save_fail(void) {
    fprintf(stderr, "Unable to save game");
}