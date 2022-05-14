// process.c
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "proj2.h"
#include "message.h"


/**
 * Random time to sleep
 * @param maxTime
 * @return time to sleep
 */
int random_sleep(int maxTime) {
    if (maxTime == 0) {
        return 0;
    }
    srand((unsigned) time(NULL));
    return rand() % (maxTime);
}


/**
 * Process oxygen
 * @param params
 * @param sharVar
 * @param oxy_ind
 */
void process_oxy(params_t *params, shar_var_t *sharVar, long oxy_ind) {
    // Oxygen start
    sem_wait(sharVar->semList.mutex_write_sem);
    sharVar->actions++;
    print_output("%ld: O %lu: started\n", sharVar->output, sharVar->actions, oxy_ind);
    sem_post(sharVar->semList.mutex_write_sem);

    // Oxygen waiting
    usleep(random_sleep(params->atom_time));

    // Oxygen going to queue
    sem_wait(sharVar->semList.mutex_write_sem);
    sharVar->actions++;
    sharVar->oxy_ready++;
    print_output("%ld: O %lu: going to queue\n", sharVar->output, sharVar->actions, oxy_ind);
    sem_post(sharVar->semList.oxygen_ready_sem);
    sem_post(sharVar->semList.oxygen_ready_sem);
    sharVar->oxy_ready--;
    sem_post(sharVar->semList.mutex_write_sem);

    // Oxygen creating a molecule
    sem_wait(sharVar->semList.oxygen_involved_sem);
    sem_wait(sharVar->semList.hydrogen_ready_sem);
    sem_wait(sharVar->semList.molecule_in_process_sem);
    sem_wait(sharVar->semList.mutex_write_sem);
    sharVar->actions++;
    if (sharVar->not_enough_h == true) {
        print_output("%ld: O %lu: not enough H\n", sharVar->output, sharVar->actions, oxy_ind);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.hydrogen_ready_sem);
        sem_post(sharVar->semList.oxygen_involved_sem);
        sem_post(sharVar->semList.mutex_write_sem);
        exit(0);
    }
    print_output("%ld: O %lu: creating molecule %d\n", sharVar->output, sharVar->actions, oxy_ind, sharVar->atom_created / 3 + 1);
    sem_post(sharVar->semList.mutex_write_sem);
    sem_post(sharVar->semList.oxygen_in_molecule_sem);
    sem_post(sharVar->semList.oxygen_in_molecule_sem);


    // Molecule waiting
    sem_wait(sharVar->semList.hydrogen_in_molecule_sem);
    sem_wait(sharVar->semList.hydrogen_in_molecule_sem);
    usleep(random_sleep(params->molecule_time));
    sem_post(sharVar->semList.molecule_done_sem);
    sem_post(sharVar->semList.molecule_done_sem);

    // Oxygen created a molecule
    sem_wait(sharVar->semList.mutex_write_sem);
    sharVar->actions++;
    print_output("%ld: O %lu: molecule %d created\n", sharVar->output, sharVar->actions, oxy_ind, sharVar->atom_created / 3 + 1);
    sharVar->atom_created++;
    if (sharVar->atom_created % 3 == 0) {
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
    }
    if (params->hydrogen_cnt - ((sharVar->atom_created - 1) / 3 + 1) * 2 <= 1) {
        sem_post(sharVar->semList.hydrogen_ready_sem);
        sem_post(sharVar->semList.hydrogen_in_molecule_sem);
        sem_post(sharVar->semList.hydrogen_in_molecule_sem);
        sharVar->not_enough_h = true;
    }
    sem_post(sharVar->semList.oxygen_involved_sem);
    sem_post(sharVar->semList.mutex_write_sem);
    exit(0);
}


/**
 * Process hydrogen
 * @param params
 * @param sharVar
 * @param hyd_ind
 */
void process_hyd(params_t *params, shar_var_t *sharVar, long hyd_ind) {
    // Hydrogen start
    sem_wait(sharVar->semList.mutex_write_sem);
    sharVar->actions++;
    print_output("%ld: H %lu: started\n", sharVar->output, sharVar->actions, hyd_ind);
    sem_post(sharVar->semList.mutex_write_sem);

    // Hydrogen waiting
    usleep(random_sleep(params->atom_time));

    // Hydrogen going to queue
    sem_wait(sharVar->semList.mutex_write_sem);
    sharVar->actions++;
    sharVar->hyd_ready++;
    print_output("%ld: H %lu: going to queue\n", sharVar->output, sharVar->actions, hyd_ind);
    sem_post(sharVar->semList.mutex_write_sem);

    if (sharVar->hyd_ready >= 2){
        sem_post(sharVar->semList.hydrogen_ready_sem);
        sem_post(sharVar->semList.pair_of_hydrogen_sem);
        sem_post(sharVar->semList.pair_of_hydrogen_sem);
        sharVar->hyd_ready -= 2;
    }

    // Hydrogen creating a molecule
    sem_wait(sharVar->semList.oxygen_ready_sem);
    sem_wait(sharVar->semList.pair_of_hydrogen_sem);
    sem_wait(sharVar->semList.hydrogen_involved_sem);
    sem_wait(sharVar->semList.molecule_in_process_sem);
    sem_wait(sharVar->semList.mutex_write_sem);

    sharVar->actions++;
    if (sharVar->not_enough_ho == true) {
        print_output("%ld: H %lu: not enough O or H\n", sharVar->output, sharVar->actions, hyd_ind);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.oxygen_ready_sem);
        sem_post(sharVar->semList.hydrogen_involved_sem);
        sem_post(sharVar->semList.mutex_write_sem);
        exit(0);
    }
    print_output("%d: H %lu: creating molecule %d\n", sharVar->output, sharVar->actions, hyd_ind, sharVar->atom_created / 3 + 1);
    sem_post(sharVar->semList.mutex_write_sem);
    sem_post(sharVar->semList.hydrogen_in_molecule_sem);

    // Molecule waiting
    sem_wait(sharVar->semList.oxygen_in_molecule_sem);
    sem_wait(sharVar->semList.molecule_done_sem);

    // Hydrogen created a molecule
    sem_wait(sharVar->semList.mutex_write_sem);
    sharVar->actions++;
    print_output("%ld: H %lu: molecule %d created\n", sharVar->output, sharVar->actions, hyd_ind, sharVar->atom_created / 3 + 1);
    sharVar->atom_created++;
    if (sharVar->atom_created % 3 == 0) {
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
        sem_post(sharVar->semList.molecule_in_process_sem);
    }
    if (params->oxygen_cnt - ((sharVar->atom_created - 1) / 3 + 1) == 0 || params->hydrogen_cnt - ((sharVar->atom_created - 1) / 3 + 1) * 2 <= 1) {
        sem_post(sharVar->semList.oxygen_ready_sem);
        sem_post(sharVar->semList.pair_of_hydrogen_sem);
        sem_post(sharVar->semList.oxygen_in_molecule_sem);
        sem_post(sharVar->semList.molecule_done_sem);
        sharVar->not_enough_ho = true;
    }
    sem_post(sharVar->semList.hydrogen_involved_sem);
    sem_post(sharVar->semList.mutex_write_sem);
    exit(0);
}


/**
 * Main process initialisation
 * @param params
 * @param sharVar
 */
void process_init(params_t *params, shar_var_t *sharVar) {
    // When there will be no molecule created
    if (params->hydrogen_cnt <= 1) {
        sem_post(sharVar->semList.hydrogen_ready_sem);
        sem_post(sharVar->semList.pair_of_hydrogen_sem);
        sharVar->not_enough_h = true;
        sharVar->not_enough_ho = true;
    }
    if (params->oxygen_cnt == 0) {
        sem_post(sharVar->semList.oxygen_ready_sem);
        sharVar->not_enough_ho = true;
    }

    // Processes oxygen
    for (long i = 0; i < params->oxygen_cnt; i++) {
        pid_t oxy_pr = fork();
        if (oxy_pr < 0) {
            warning_msg("%s: process oxygen not created!\n");
        } else if (oxy_pr == 0) {
            process_oxy(params, sharVar, i + 1);
        }
    }

    // Processes hydrogen
    for (long i = 0; i < params->hydrogen_cnt; i++) {
        pid_t hyd_pr = fork();
        if (hyd_pr < 0) {
            warning_msg("%s: process hydrogen not created!\n");
        } else if (hyd_pr == 0) {
            process_hyd(params, sharVar, i + 1);
        }
    }
}

