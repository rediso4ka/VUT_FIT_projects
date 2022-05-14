// htab_private.h
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __HTAB_PRIVATE_H__
#define __HTAB_PRIVATE_H__

#include "htab.h"

#define AVG_LEN_MIN 1
#define AVG_LEN_MAX 2

// Item structure:
struct htab_item {
    struct htab_item *next;
    htab_pair_t *pair;
};

// Definition of item structure:
typedef struct htab_item htab_item_t;

// Hast table structure:
struct htab {
    size_t size;
    size_t arr_size;
    htab_item_t *arr_ptr[];
};

#endif //__HTAB_PRIVATE_H__
