// wordcount.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include "htab.h"
#include "io.h"
#include "error.h"

#define MAX_WORD 12800

void htab_pair_print(htab_pair_t *pair);

int main() {
    htab_t *hash_table = htab_init(10007);
    if (hash_table == NULL) {
        error_exit("%s: hash table not initialised!\n",__func__);
    }
    char word[MAX_WORD];
    bool warning = false;
    int letters;
    while ((letters = read_word(word, MAX_WORD, stdin)) != EOF) {
        if (letters == MAX_WORD && warning == false) {
            warning_msg("%s: the word is too long!\n",__func__);
            warning = true;
        }
        htab_pair_t *pair = htab_lookup_add(hash_table, word);
        if (pair == NULL) {
            htab_free(hash_table);
            error_exit("%s: \n",__func__);
        }
        pair->value += 1;
    }
    htab_for_each(hash_table, htab_pair_print);
    htab_free(hash_table);
    return 0;
}