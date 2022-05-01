// sem.h
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __SEM_H__
#define __SEM_H__

#include <stdbool.h>
#include "proj2.h"

bool my_sem_make(shar_var_t *sharVar);

bool my_sem_delete(shar_var_t *sharVar);

#endif //__SEM_H__
