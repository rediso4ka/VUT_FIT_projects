// htab_hash_function.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "htab.h"
#include "stdint.h"


/**
 * Suitable hash function for strings.
 * @param str
 * @return value, which then %arr_size gives an index to the hash table
 */
size_t htab_hash_function(const char *str) {
    uint32_t h=0;     // must contain 32 bits
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}