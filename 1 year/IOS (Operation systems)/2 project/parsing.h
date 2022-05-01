// parsing.h
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __PARSING_H__
#define __PARSING_H__

#include "proj2.h"

long argument_reading(const char *argument);

params_t * parse_args(int argc, char *argv[]);

#endif //__PARSING_H__
