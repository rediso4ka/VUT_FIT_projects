// shared.c
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "proj2.h"


/**
 * Adding start values of shared memory
 * @param sharVar
 */
void shar_var_start(shar_var_t *sharVar) {
    sharVar->actions = 0;
    sharVar->oxy_ready = 0;
    sharVar->hyd_ready = 0;
    sharVar->atom_created = 0;
    sharVar->not_enough_h = false;
    sharVar->not_enough_ho = false;
}


/**
 * Opening output file
 * @param sharVar
 * @return true if success
 */
bool shar_var_open_output(shar_var_t *sharVar) {
    sharVar->output = fopen(OUTPUT_FILE, "w");
    if (sharVar->output == NULL) {
        return false;
    }
    return true;
}


/**
 * Closing output file
 * @param sharVar
 * @return
 */
bool shar_var_close_output(shar_var_t *sharVar) {
    if (fclose(sharVar->output) == EOF) {
        return false;
    }
    return true;
}


/**
 * Delete shared variable
 * @param sharVar
 * @return true if success
 */
bool shar_var_delete(shar_var_t *sharVar) {
    int err = munmap(sharVar, sizeof(* sharVar));
    if (err != 0) {
        return false;
    }
    return true;
}
