// error.h
// Solution IJC-DU2, task 2), 17.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __ERROR_H__
#define __ERROR_H__

void warning_msg(const char *fmt, ...);

void error_exit(const char *fmt, ...);

#endif //__ERROR_H__
