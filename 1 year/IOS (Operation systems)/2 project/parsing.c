// parsing.c
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "message.h"
#include "proj2.h"


/**
 * Read a numeral argument from string
 * @param argument
 * @return value of argument
 */
long argument_reading(const char *argument) {
    if (argument == NULL || *argument == '\0') {
        return -1;
    }
    char *trash;
    long num = strtol(argument, &trash, 10);
    if (*trash != '\0' || num < LONG_MIN || num > LONG_MAX) {
        return -1;
    }
    return num;
}


/**
 * Parse user's arguments into params structure
 * @param argc
 * @param argv
 * @return structure of params
 */
params_t * parse_args(int argc, char *argv[]) {
    if (argc != 5) {
        warning_msg("%s: wrong amount of arguments!\n", __func__);
        return NULL;
    }
    params_t *params = calloc(1, sizeof(params_t));
    if (params == NULL) {
        warning_msg("%s: memory allocation error!\n", __func__);
        return NULL;
    }
    params->oxygen_cnt = argument_reading(argv[1]);
    params->hydrogen_cnt = argument_reading(argv[2]);
    params->atom_time = (int)argument_reading(argv[3]);
    params->molecule_time = (int)argument_reading(argv[4]);
    if (params->oxygen_cnt < 1) {
        warning_msg("%s: bad format of oxygen!\n", __func__);
        free(params);
        return NULL;
    }
    if (params->hydrogen_cnt < 1) {
        warning_msg("%s: bad format of hydrogen!\n", __func__);
        free(params);
        return NULL;
    }
    if (params->atom_time < 0 || params->atom_time > MAX_ATOM_TIME) {
        warning_msg("%s: bad format of atom time!\n", __func__);
        free(params);
        return NULL;
    }
    if (params->molecule_time < 0 || params->molecule_time > MAX_MOLECULE_TIME) {
        warning_msg("%s: bad format of molecule time!\n", __func__);
        free(params);
        return NULL;
    }
    return params;
}