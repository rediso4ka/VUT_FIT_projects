// htab_find.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "htab.h"
#include "htab_private.h"


/**
 * Searching for pair with "key" in hash table
 * @param t
 * @param key
 * @return pair with "key"
 */
htab_pair_t * htab_find(htab_t * t, htab_key_t key) {
    size_t arr_index = htab_hash_function(key) % (t->arr_size);
    htab_item_t *item = t->arr_ptr[arr_index];
    while (item != NULL) {
        if (strcmp(item->pair->key, key) == 0) {
            return item->pair;
        }
        item = item->next;
    }
    return NULL;
}