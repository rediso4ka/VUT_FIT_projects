// message.h
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

void warning_msg(const char *fmt, ...);

void print_output(const char *fmt, FILE *f, ...);

void error_exit(const char *fmt, ...);

#endif //__MESSAGE_H__