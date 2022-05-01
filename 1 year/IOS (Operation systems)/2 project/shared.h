// shared.h
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __SHARED_H__
#define __SHARED_H__

#include <stdbool.h>
#include "proj2.h"

#define SHAR_VAR_INIT(sharVar) {sharVar = mmap(NULL, sizeof(*sharVar), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);}

void shar_var_start(shar_var_t *sharVar);

bool shar_var_open_output(shar_var_t *sharVar);

bool shar_var_close_output(shar_var_t *sharVar);

bool shar_var_delete(shar_var_t *sharVar);

#endif //__SHARED_H__
