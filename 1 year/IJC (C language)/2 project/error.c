// error.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


/**
 * Prints warning message to stderr
 * @param fmt
 * @param ...
 */
void warning_msg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "WARNING: ");
    vfprintf(stderr, fmt, args);

    va_end(args);
}


/**
 * Prints error message to stderr
 * @param fmt
 * @param ...
 */
void error_exit(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, args);

    va_end(args);
    exit(1);
}

