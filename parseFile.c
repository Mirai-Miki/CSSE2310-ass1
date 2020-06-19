/*
 * parseFile.c
 * Author: Michael Bossner
 *
 * This file contains functions for parsing input provided.
 */

#include <stdio.h>
#include <stdlib.h>

#include "parseFile.h"
#include "error.h"

#define FOREVER for (;;)

//////////////////////////////// Functions ////////////////////////////////////

void split_stdin(FileCont* splitStdIn) {
    splitStdIn->sizeOfOut = 0;
    splitStdIn->output = malloc(sizeof(char*) * 1);
    splitStdIn->output[0] = malloc(sizeof(char) * 1);            
    int next;
    // parse through stdin and split each word into it's own string
    FOREVER {
        for (int strLen = 0; ; strLen++) {
            next = fgetc(stdin);
            if ((splitStdIn->sizeOfOut == 0 && strLen == 0 && next == EOF)) {
                // EOF received while waiting for input
                error_10();
            } else if ((next == '\n') || (next == ' ') || (next == EOF)) {
                // End of the word. Finish string and break loop
                splitStdIn->output[splitStdIn->sizeOfOut] = 
                        realloc(splitStdIn->output[splitStdIn->sizeOfOut], 
                        sizeof(char) * (strLen + 1));
                if (strLen) {
                    splitStdIn->output[splitStdIn->sizeOfOut][strLen] = '\0';
                } else {
                    // multiple spaces in a row.
                    splitStdIn->output[splitStdIn->sizeOfOut][strLen] = ' ';
                }
                break;
            } else {
                // Middle of a word add character to string
                splitStdIn->output[splitStdIn->sizeOfOut] = 
                        realloc(splitStdIn->output[splitStdIn->sizeOfOut], 
                        sizeof(char) * (strLen + 1));
                splitStdIn->output[splitStdIn->sizeOfOut][strLen] = next;
            }
        }
        splitStdIn->sizeOfOut++;
        if (next == '\n') {
            break;
        } else if (next == EOF) {
            printf("\n");
            break;
        } else {
            // More words remaining. Add additional storage to store new string
            splitStdIn->output = realloc(splitStdIn->output, 
                    sizeof(char*) * (splitStdIn->sizeOfOut + 1));
            splitStdIn->output[splitStdIn->sizeOfOut] = 
                    malloc(sizeof(char) * 1);
        }
    }
}

void split_file(FILE* file, FileCont* splitFile) {
    splitFile->sizeOfOut = 0;
    splitFile->output = malloc(sizeof(char*) * 1);
    splitFile->output[0] = malloc(sizeof(char) * 1);            
    int next;
    // parse file and split each line into it's own string
    FOREVER {
        for (int strLen = 0; ; strLen++) {
            next = fgetc(file);
            splitFile->output[splitFile->sizeOfOut] = 
                    realloc(splitFile->output[splitFile->sizeOfOut], 
                    sizeof(char) * (strLen + 1));
            if ((next == '\n') || (next == EOF)) {
                // End of line null terminate string and break         
                splitFile->output[splitFile->sizeOfOut][strLen] = '\0';
                break;
            } else {
                splitFile->output[splitFile->sizeOfOut][strLen] = next;
            }
        }
        splitFile->sizeOfOut++;
        if (next == EOF) {
            break;
        } else {
            // More lines remaining. Add additional storage for new string.
            splitFile->output = realloc(splitFile->output, 
                    sizeof(char*) * (splitFile->sizeOfOut + 1));
            splitFile->output[splitFile->sizeOfOut] = 
                    malloc(sizeof(char) * 1);
        }
    }
}