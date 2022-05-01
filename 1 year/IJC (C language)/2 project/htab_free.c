// htab_free.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"


/**
 * Free allocated hash table
 * @param t
 */
void htab_free(htab_t * t) {
    htab_clear(t);
    free(t);
}