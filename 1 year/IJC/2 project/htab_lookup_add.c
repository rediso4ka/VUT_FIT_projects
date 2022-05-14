// htab_lookup_add.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"


/**
 * Searching for pair with "key" in hash table
 * If there is no match, creating new item in hash table
 * @param t
 * @param key
 * @return pair with "key"
 */
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key) {
    htab_pair_t *attempt = htab_find(t, key);
    if (attempt != NULL) {
        return attempt;
    }

    // making new item
    htab_item_t *item_to_add = malloc(sizeof(htab_item_t));
    if (item_to_add == NULL) {
        return NULL;
    }
    item_to_add->pair = malloc(sizeof(htab_pair_t));
    if (item_to_add->pair == NULL) {
        free(item_to_add);
        return NULL;
    }
    item_to_add->pair->key = malloc(strlen(key) + 1);
    if (item_to_add->pair->key == NULL) {
        free(item_to_add->pair);
        free(item_to_add);
        return NULL;
    }
    strcpy((char *) item_to_add->pair->key, key);
    item_to_add->pair->value = 0;
    item_to_add->next = NULL;

    size_t arr_index = htab_hash_function(key) % (t->arr_size);
    htab_item_t *tmp = t->arr_ptr[arr_index];

    // adding item_to_add as a first item of new array pointer
    if (tmp == NULL) {
        t->size += 1;

        int division = t->size / t->arr_size;
        if (division > AVG_LEN_MAX) {
            htab_resize(t, t->arr_size * 2);
        }

        t->arr_ptr[arr_index] = item_to_add;
        return item_to_add->pair;
    }

    // adding item_to_add to the end of existing array pointer
    while (tmp != NULL) {
        if (tmp->next == NULL) {
            t->size += 1;

            int division = t->size / t->arr_size;
            if (division > AVG_LEN_MAX) {
                htab_resize(t, t->arr_size * 2);
            }

            tmp->next = item_to_add;
            break;
        }
        tmp = tmp->next;
    }
    return item_to_add->pair;
}