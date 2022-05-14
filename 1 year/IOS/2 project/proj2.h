// proj2.h
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef __PROJ2_H__
#define __PROJ2_H__

#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_ATOM_TIME 1000
#define MAX_MOLECULE_TIME 1000
#define OUTPUT_FILE "proj2.out"

typedef struct {
    sem_t *mutex_write_sem;             // Safety writing
    sem_t *oxygen_ready_sem;            // Amount of oxygen in the queue
    sem_t *oxygen_involved_sem;         // Amount of places for oxygen in molecule
    sem_t *hydrogen_ready_sem;          // Amount of hydrogen in the queue (for oxygen)
    sem_t *hydrogen_involved_sem;       // Amount of places for hydrogen in molecule
    sem_t *molecule_in_process_sem;     // Amount of places for atoms in molecule
    sem_t *pair_of_hydrogen_sem;        // Amount of hydrogen in the queue (for hydrogen)
    sem_t *molecule_done_sem;           // Signal from oxygen for hydrogens that molecule is done
    sem_t *oxygen_in_molecule_sem;      // Signal for hydrogen that oxygen is in molecule
    sem_t *hydrogen_in_molecule_sem;    // Signal for oxygen that hydrogens are in molecule
} sem_list_t;

typedef struct {
    size_t actions;         // Amount of all actions
    size_t oxy_ready;       // Amount of ready oxygens
    size_t hyd_ready;       // Amount of ready hydrogens
    size_t atom_created;    // Amount of created atoms (molecule * 3)
    bool not_enough_h;      // If there is no H for O
    bool not_enough_ho;     // If there is no H or O for H
    sem_list_t semList;     // Semaphores
    FILE *output;           // Output file
} shar_var_t;

typedef struct {
    long oxygen_cnt;
    long hydrogen_cnt;
    int atom_time;
    int molecule_time;
} params_t;


#endif //__PROJ2_H__
