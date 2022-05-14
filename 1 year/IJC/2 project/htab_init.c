// htab_init.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"


/**
 * Hash table constructor
 * @param n - arr_size
 * @return hash table
 */
htab_t *htab_init(size_t n) {
    htab_t *hash_table = (htab_t *)malloc(sizeof(htab_t) + sizeof(htab_item_t *) * n);
    if (hash_table == NULL) {
        return NULL;
    }
    hash_table->size = 0;
    hash_table->arr_size = n;
    for (size_t i = 0; i < n; i++) {
        hash_table->arr_ptr[i] = NULL;
    }
    return hash_table;
}