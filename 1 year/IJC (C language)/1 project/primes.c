// primes.c
// Řešení IJC-DU1, příklad a), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include <time.h>
#include "bitset.h"
#include "eratosthenes.h"
#include "error.h"

#define BORDER 300000001
#define NUMB_NEEDED 10

/*
 * V pripade ze chceme udelat staticke pole:
 */
//bitset_create(bit_array, 100);

int main(){
    clock_t start = clock();

    /*
     * V pripade ze chceme udelat lokalni pole:
     */
    //bitset_create(bit_array, 100);

    bitset_alloc(bit_array, BORDER);

    Eratosthenes(bit_array);

    /*
     * Do pole 10-ti ul dame 10 prvocisel od konce filtrovaneho bit_array
     */
    bitset_index_t numbers[NUMB_NEEDED] = {0};
    int index = NUMB_NEEDED - 1;

    for (bitset_index_t i = BORDER - 1; i > 1 && index >= 0; i--){
        if (!bitset_getbit(bit_array, i)){
            numbers[index] = i;
            index--;
        }
    }

    /*
     * Vypis prvocisel do stdout:
     */
    for (int i = 0; i < NUMB_NEEDED; i++){
        printf("%lu\n",numbers[i]);
    }

    bitset_free(bit_array);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}
