/*
 * parseFile.h
 * Author: Michael Bossner
 * 
 * Header file for parseFile.c
 */

#ifndef PARSE_FILE_H
#define PARSE_FILE_H

#include <stdio.h>

typedef struct FileCont FileCont;

/*
 * A container for holding the output of parsed input
 */
struct FileCont {
    int sizeOfOut; // size of container
    char** output; // Output from the parsed input
};

/*
 * Splits input given from stdin into space separated words.
 * If there is more then one space in a row then the following spaces will be
 * considered words and added to output.
 *
 * splitFile: the container for the output of the split input
 *
 * error_10: If EOF is received while waiting for input by stdin. Game ends.
 */
void split_stdin(FileCont* splitStdIn);

/*
 * Splits input provided by a file into lines.
 *
 * file: the file to be parsed
 * 
 * splitFile: the container to place the output of the parsing
 */
void split_file(FILE* file, FileCont* splitFile);

#endif