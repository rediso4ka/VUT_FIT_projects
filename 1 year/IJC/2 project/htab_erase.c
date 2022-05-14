// htab_erase.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"


/**
 * Delete item from hash table
 * @param t
 * @param key
 * @return true if success
 */
bool htab_erase(htab_t * t, htab_key_t key) {
    size_t arr_index = htab_hash_function(key) % (t->arr_size);
    htab_item_t *item_to_delete = t->arr_ptr[arr_index];
    if (item_to_delete == NULL) {
        return false;
    }
    if (strcmp(item_to_delete->pair->key, key) == 0) {
        t->size -= 1;

        int division = t->size / t->arr_size;
        if (division < AVG_LEN_MIN) {
            htab_resize(t, t->arr_size / 2);
        }

        t->arr_ptr[arr_index] = item_to_delete->next;
        free((char *) item_to_delete->pair->key);
        free(item_to_delete->pair);
        free(item_to_delete);
        return true;
    }
    htab_item_t *back = item_to_delete;
    item_to_delete = item_to_delete->next;
    while (item_to_delete != NULL) {
        if (strcmp(item_to_delete->pair->key, key) == 0) {
            t->size -= 1;
            back->next = item_to_delete->next;
            free((char *) item_to_delete->pair->key);
            free(item_to_delete->pair);
            free(item_to_delete);
            return true;
        }
        back = item_to_delete;
        item_to_delete = item_to_delete->next;
    }
    return false;
}