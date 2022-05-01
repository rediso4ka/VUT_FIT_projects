// io.c
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include <ctype.h>


/**
 * Read one word from f into s
 * @param s
 * @param max
 * @param f
 * @return amount of read letters
 */
int read_word(char *s, int max, FILE *f) {
    int letter;
    int count = 0;
    while ((letter = fgetc(f)) != EOF) {
        if (isspace(letter)) {
            if (count == 0) {
                continue;
            } else {
                break;
            }
        }
        if (count >= max - 1) {
            count++;
            break;
        }
        s[count++] = letter;
    }
    if (count == 0) {
        return -1;
    }
    if (count == max) {
        s[count - 1] = '\0';
    } else {
        s[count] = '\0';
    }
    return count;
}
