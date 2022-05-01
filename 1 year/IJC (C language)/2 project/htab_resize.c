// htab_resize.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "htab.h"
#include "htab_private.h"


/**
 * Change size of hash table
 * @param t
 * @param newn
 */
void htab_resize(htab_t *t, size_t newn) {
    htab_t *new_table = htab_init(newn);
    if (new_table == NULL) {
        return;
    }
    for (size_t i = 0; i < t->arr_size; i++) {
        htab_item_t *item = t->arr_ptr[i];
        while (item != NULL) {
            htab_pair_t *new_pair = htab_lookup_add(new_table, item->pair->key);
            new_pair->value += 1;
            item = item->next;
        }
    }
    htab_t *tmp = t;
    t = new_table;
    htab_free(tmp);
}