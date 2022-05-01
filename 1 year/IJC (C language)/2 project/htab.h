// htab.h
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __HTAB_H__      // protection from twice definition
#define __HTAB_H__

#include <string.h>     // size_t
#include <stdbool.h>    // bool

// Table:
struct htab;    // incomplete structure declaration-user does not see content
typedef struct htab htab_t;     // typedef by assignment

// Types:
typedef const char * htab_key_t;        // type key
typedef int htab_value_t;               // type value

// Data pairs in the table:
typedef struct htab_pair {
    htab_key_t    key;          // key
    htab_value_t  value;        // associated value
} htab_pair_t;                  // typedef by assignment

// Hash function (same for all tables in the program)
// If you define the same function, use yours.
size_t htab_hash_function(htab_key_t str);

// Functions for working with the table:
htab_t *htab_init(size_t n);                    // table constructor
size_t htab_size(const htab_t * t);             // number of items in the table
size_t htab_bucket_count(const htab_t * t);     // array size
void htab_resize(htab_t *t, size_t newn);       // resizing the array
// (allows you to book a place)

htab_pair_t * htab_find(htab_t * t, htab_key_t key);  // search
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key);

bool htab_erase(htab_t * t, htab_key_t key);    // delete item

// for_each: calling function f for each item
// Caution: f must not change the key and does not add / delete items
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data));

void htab_clear(htab_t * t);    // delete all items
void htab_free(htab_t * t);     // free allocated table

#endif // __HTAB_H__