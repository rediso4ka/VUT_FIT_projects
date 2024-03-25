/*
 * File: error.h
 * Author: xshevc01
 * Description: header file for printing errors and warnings
 */

#ifndef ERROR_H
#define ERROR_H

/*
 * Function: print_warning
 * Description: prints warning
 * Parameters:
 *   - message: warning message to print
 * Return:
 *   - nothing
 */
void print_warning(const char *message);

/*
 * Function: print_error_and_exit
 * Description: prints error and exits
 * Parameters:
 *   - message: error message to print
 * Return:
 *   - nothing
 */
void print_error_and_exit(const char *message);

#endif