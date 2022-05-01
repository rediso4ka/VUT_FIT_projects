// process.h
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "proj2.h"

int random_sleep(int maxTime);

void process_oxy(params_t *params, shar_var_t *sharVar, long oxy_ind);

void process_hyd(params_t *params, shar_var_t *sharVar, long hyd_ind);

void process_init(params_t *params, shar_var_t *sharVar);

#endif //__PROCESS_H__
