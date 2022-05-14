// htab_for_each.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "htab.h"
#include "htab_private.h"


/**
 * Do function f for each pair
 * @param t
 * @param f
 */
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)) {
    for (size_t i = 0; i < t->arr_size; i++) {
        htab_item_t *item = t->arr_ptr[i];
        while (item != NULL) {
            (*f)(item->pair);
            item = item->next;
        }
    }
}
