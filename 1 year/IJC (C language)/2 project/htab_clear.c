// htab_clear.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"


/**
 * Delete all items in hash table
 * @param t
 */
void htab_clear(htab_t * t) {
    t->size = 0;
    for (size_t i = 0; i < t->arr_size; i++) {
        htab_item_t *item_to_delete = t->arr_ptr[i];
        htab_item_t *tmp;
        while (item_to_delete != NULL) {
            tmp = item_to_delete->next;
            free((char *) item_to_delete->pair->key);
            free(item_to_delete->pair);
            free(item_to_delete);
            item_to_delete = tmp;
        }
        t->arr_ptr[i] = NULL;
    }
}