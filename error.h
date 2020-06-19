/* 
 * error.h
 * Author: Michael Bossner
 *
 * Error.h is a header file for the error.c file.
 */

#ifndef ERROR_H
#define ERROR_H

/*
 * Used when an invalid amount of command line arguments are given on the start
 * of the program. The function will print an error message to stderr and exit 
 * the program giving the exit status of 1.
 */
void error_1(void);

/*
 * Used when the Tile File cannot be accessed. The function will print an
 * error message to stderr and exit the program giving the exit status of 2.
 */
void error_2(void);

/*
 * Used when the contents of the Tile File is invalid. The function 
 * will print an error message to stderr and exit the program giving the 
 * exit status of 3.
 */ 
void error_3(void);

/*
 * Used when an invalid player type has been entered via a
 * command line argument. The function will print an error message to stderr 
 * and exit the program giving the exit status of 4.
 */
void error_4(void);

/*
 * Used when invalid dimension have been entered for the board via
 * the command line arguments. The function will print an error message to
 * stderr and exit the program giving the exit status of 5.
 */ 
void error_5(void);

/*
 * Used when a save file provided cannot be accessed. The function
 * will print an error message to stderr and exit the program giving the exit
 * status of 6.
 */
void error_6(void);

/*
 * Used when the contents of a save file are invalid. The function
 * will print an error message to stderr and exit the program giving the exit
 * status of 7.
 */ 
void error_7(void);

/*
 * Used when an EOF signal is sent while waiting for user input.
 * it will print an error message to stderr and exit the program giving the
 * exit status of 10.
 */ 
void error_10(void);

/*
 * Used when an attempt to save the game fails.
 * An error will be printed to stderr.
 */
void err_save_fail(void);

#endif