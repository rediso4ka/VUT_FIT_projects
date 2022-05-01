// tail.c
// Solution IJC-DU2, task 1), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

#define DELIM '\n'

void print_last_rows(char *str, int n);
char *arguments_parsing(int argc, char *argv[], int *amount_of_rows);
char *file_reading(char *file);


int main(int argc, char *argv[]) {
    int amount_of_rows = 10;
    char *file_name = arguments_parsing(argc, argv, &amount_of_rows);
    char *text = file_reading(file_name);
    print_last_rows(text,amount_of_rows);
    free(text);
    return 0;
}


/**
 * Parses arguments from command line
 * @param argc
 * @param argv
 * @param amount_of_rows
 * @return file name
 */
char *arguments_parsing(int argc, char *argv[], int *amount_of_rows) {
    int flag = 1;
    char *file_name = NULL;
    while (flag < argc) {
        if (strcmp(argv[flag], "-n") == 0) {            // if there is -n, we change
            if (flag + 1 == argc) {                     // amount of rows
                error_exit("%s: no number after -n!\n",__func__);
            }
            *amount_of_rows = atoi(argv[flag+1]);
            if (*amount_of_rows < 0) {
                error_exit("%s: wrong number of rows!\n",__func__);
            }
            flag += 2;
        } else {
            file_name = argv[flag];                     // if it is not -n,
            flag++;                                     // it is a file name
        }
    }
    return file_name;
}


/**
 * Takes string from file or stdin
 * @param file
 * @return file content
 */
char *file_reading(char *file) {
    char *text;
    long length;
    FILE *fd;
    if (file == NULL) {     // if we did not receive a file name from user,
        fd = stdin;         // we use stdin
    } else {
        fd = fopen(file, "rb");
    }
    if (fd == NULL) {
        error_exit("%s: error during file reading!\n",__func__);
    }
    fseek(fd, 0, SEEK_END);     // going to the end of file
    length = ftell(fd);         // and taking its length
    fseek(fd, 0, SEEK_SET);     // going back to the beginning
    text = (char *)malloc(length + 1);
    if (text == NULL) {
        error_exit("%s: malloc error!\n",__func__);
    }
    fread(text, 1, length, fd); // reading file content to text
    fclose(fd);
    text[length - 1] = '\0';    // ending text
    return text;
}


/**
 * Prints n last rows from a string to stdout
 * @param str
 * @param n
 */
void print_last_rows(char *str, int n) {
    if (n < 0) {
        error_exit("%s: wrong number of rows!\n",__func__);
    }
    size_t cnt = 0;                      	// counter of DELIM
    char *target_pos = NULL;            	// To store the output position in str
    target_pos = strrchr(str, DELIM);   	// find the last occurrence of DELIM
    if (target_pos == NULL) {
        error_exit("%s: string doesn't contain '\\n' character!\n",__func__);
    }
    while (cnt < (long unsigned int) n) {                   // finding position to start
        while (str < target_pos && *target_pos != DELIM) {  // searching for DELIM
            --target_pos;
        }
        if (*target_pos == DELIM) {                         // jump over \n
            --target_pos;
            ++cnt;
        } else if ((long unsigned int) n - cnt < 2) {
            break;
        } else {
            warning_msg("%s: there are less rows, but ok...\n",__func__);
            break;
        }
    }
    if (str < target_pos || cnt == (long unsigned int) n) {      // we jumped over the first needed row,
        target_pos += 2;                                         // so we go back with 2 letters
    }
    printf("%s\n", target_pos);
}