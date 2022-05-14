// htab_size.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "htab.h"
#include "htab_private.h"


/**
 * Number of items in the hash table
 * @param t
 * @return size
 */
size_t htab_size(const htab_t * t) {
    return t->size;
}