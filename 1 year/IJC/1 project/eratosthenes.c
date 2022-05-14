// eratosthenes.c
// Řešení IJC-DU1, příklad a), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include "bitset.h"

void Eratosthenes(bitset_t bit_array){
    bitset_setbit(bit_array, 0, 1);
    bitset_setbit(bit_array, 1, 1);

    for (bitset_index_t i = 2; i * i <= bitset_size(bit_array); i++){
        if (!bitset_getbit(bit_array, i)){
            for (bitset_index_t j = 2 * i; j < bitset_size(bit_array); j += i) {
                bitset_setbit(bit_array, j, 1);
            }
        }
    }
}