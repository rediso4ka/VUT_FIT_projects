// htab_pair_print.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include "htab.h"


/**
 * Print pair
 * @param pair
 */
void htab_pair_print(htab_pair_t *pair) {
    printf("%s:  %d\n", pair->key, pair->value);
}