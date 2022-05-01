// steg-decode.c
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm.h"
#include "error.h"
#include "bitset.h"
#include "eratosthenes.h"

int main(int argc, char *argv[]){
    if (argc != 2){
        error_exit("%s: chybny pocet argumentu!\n",__func__);
    }

    struct ppm *pic = ppm_read(argv[1]);
    if (pic == NULL){
        error_exit("%s: chyba pri cteni souboru!\n",__func__);
    }

    /*
     * Pocet bitu dat obrazku:
     */
    unsigned long pic_size = pic->xsize * pic->ysize * 3 * CHAR_BIT;

    bitset_alloc(ppm_bit_array, pic_size + 1);

    Eratosthenes(ppm_bit_array);

    /*
     * Cteme jednotliva pismena a davame je na stdout:
     */
    char letter = 0;
    unsigned shift = 0;
    for (bitset_index_t i = 29; i < pic_size + 1; i++){
        if (!bitset_getbit(ppm_bit_array, i)){
            letter |= (pic->data[i] & 1) << shift;
            shift++;
            if (shift == 8){
                if (letter == '\0'){
                    goto skip;
                }
                printf("%c",letter);
                shift  = 0;
                letter = 0;
            }
        }
    }
    error_exit("nenalezen konec message!\n");

    skip:
    /*
     * Pro prehlednost vysledku:
     */
    printf("\n");

    ppm_free(pic);
    bitset_free(ppm_bit_array);
    return 0;
}