// htab_bucket_count.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "htab.h"
#include "htab_private.h"


/**
 * Array size
 * @param t
 * @return arr_size
 */
size_t htab_bucket_count(const htab_t * t) {
    return t->arr_size;
}