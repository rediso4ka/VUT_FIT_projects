// bitset.h
// Řešení IJC-DU1, příklad a), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef BITSET_H
#define BITSET_H

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include "error.h"

typedef unsigned long *bitset_t;
typedef unsigned long bitset_index_t;

#define BIT_LIMIT 350000000
#define UL_BYTES_NEED(bits) ((bits) / (sizeof(unsigned long) * CHAR_BIT) + (((bits) % (sizeof(unsigned long) * CHAR_BIT)) > 0))

#define bitset_create(bit_array, array_size)                                                        \
                            static_assert((array_size) > 0, "Spatna velikost pole!");               \
                            bitset_index_t bit_array[UL_BYTES_NEED(array_size) + 1] = {array_size};

#define bitset_alloc(bit_array, array_size)                                                                                 \
                            assert(array_size > 0 && array_size < BIT_LIMIT);                                               \
                            bitset_t bit_array = (bitset_t)calloc(UL_BYTES_NEED(array_size) + 1, sizeof(bitset_index_t) );  \
                            if (bit_array == NULL){                                                                         \
                                error_exit("bitset_alloc: Chyba alokace pameti!\n");                                        \
                            }                                                                                               \
                            bit_array[0] = array_size;

#ifndef USE_INLINE
/*
 * Nemuzeme udelat inline funkce z bitset_create a bitset_alloc,
 * Protoze nemame konktretni objekt pro predavani
 * (jenom jmeno zatim neexistujiciho arraye)
 */

#define bitset_free(bit_array) (free(bit_array))

#define bitset_size(bit_array) ((bit_array)[0])

#define bitset_setbit(bit_array, index, value) {                                                                                                    \
                            if (value == 0){                                                                                                        \
                                bit_array[index/(CHAR_BIT * sizeof(bitset_index_t)) + 1] &= ~(1ul << (index%(CHAR_BIT * sizeof(bitset_index_t))));  \
                            } else {                                                                                                                \
                                bit_array[index/(CHAR_BIT * sizeof(bitset_index_t)) + 1] |= (1ul << (index%(CHAR_BIT * sizeof(bitset_index_t))));   \
                            }                                                                                                                       \
                        }

#define bitset_getbit(bit_array, index) (                                                                                                           \
                            (index > bitset_size(bit_array)) ?                                                                                      \
                            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, bitset_size(bit_array)), 1 :             \
                            (bit_array[index/(CHAR_BIT * sizeof(bitset_index_t)) + 1] & (1ul << (index%(CHAR_BIT * sizeof(bitset_index_t))))) > 0   \
                        )

#else

inline void bitset_free(bitset_t bit_array){
    free(bit_array);
}

inline bitset_index_t bitset_size(bitset_t bit_array){
    return bit_array[0];
}

inline void bitset_setbit(bitset_t bit_array, bitset_index_t index, unsigned value){
    if (value == 0){
        bit_array[(index)/(CHAR_BIT * sizeof(bitset_index_t)) + 1] &= ~(1ul << ((index)%(CHAR_BIT * sizeof(bitset_index_t))));
    } else {
        bit_array[(index)/(CHAR_BIT * sizeof(bitset_index_t)) + 1] |= (1ul << ((index)%(CHAR_BIT * sizeof(bitset_index_t))));
    }
}

inline bitset_index_t bitset_getbit(bitset_t bit_array, bitset_index_t index){
    return (index > bitset_size(bit_array)) ? error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, bitset_size(bit_array)), 1 : (bit_array[index/(CHAR_BIT * sizeof(bitset_index_t)) + 1] & (1ul << (index%(CHAR_BIT * sizeof(bitset_index_t))))) > 0;
}

#endif
#endif //BITSET_H
