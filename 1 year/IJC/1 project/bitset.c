// bitset.c
// Řešení IJC-DU1, příklad a), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "bitset.h"

#ifdef USE_INLINE

extern void bitset_free(bitset_t bit_array);

extern bitset_index_t bitset_size(bitset_t bit_array);

extern void bitset_setbit(bitset_t bit_array, bitset_index_t inidex, unsigned value);

extern bitset_index_t bitset_getbit(bitset_t bit_array, bitset_index_t index);

#endif