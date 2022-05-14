// error.h
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef ERROR_H
#define ERROR_H

void warning_msg(const char *fmt, ...);

void error_exit(const char *fmt, ...);

#endif //ERROR_H
