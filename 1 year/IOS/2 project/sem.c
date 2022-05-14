// sem.c
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <fcntl.h>
#include <stdbool.h>
#include <semaphore.h>
#include "proj2.h"


/**
 * Initialising semaphores in the list
 * @param sharVar
 * @return true if success
 */
bool my_sem_make(shar_var_t *sharVar) {
    sharVar->semList.mutex_write_sem = sem_open("~xshevc01.mutex_write_sem~", O_CREAT, S_IRUSR | S_IWUSR, 1);
    sharVar->semList.oxygen_ready_sem = sem_open("~xshevc01.oxygen_ready_sem~", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sharVar->semList.oxygen_involved_sem = sem_open("~xshevc01.oxygen_involved_sem~", O_CREAT, S_IRUSR | S_IWUSR, 1);
    sharVar->semList.hydrogen_ready_sem = sem_open("~xshevc01.hydrogen_ready_sem~", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sharVar->semList.hydrogen_involved_sem = sem_open("~xshevc01.hydrogen_involved_sem~", O_CREAT, S_IRUSR | S_IWUSR, 2);
    sharVar->semList.molecule_in_process_sem = sem_open("~xshevc01.molecule_in_process_sem~", O_CREAT, S_IRUSR | S_IWUSR, 3);
    sharVar->semList.pair_of_hydrogen_sem = sem_open("~xshevc01.pair_of_hydrogen_sem~", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sharVar->semList.molecule_done_sem = sem_open("~xshevc01.molecule_done_sem~", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sharVar->semList.oxygen_in_molecule_sem = sem_open("~xshevc01.oxygen_in_molecule_sem~", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sharVar->semList.hydrogen_in_molecule_sem = sem_open("~xshevc01.hydrogen_in_molecule_sem~", O_CREAT, S_IRUSR | S_IWUSR, 0);

    if (sharVar->semList.mutex_write_sem == SEM_FAILED ||
        sharVar->semList.oxygen_ready_sem == SEM_FAILED ||
        sharVar->semList.oxygen_involved_sem == SEM_FAILED ||
        sharVar->semList.hydrogen_ready_sem == SEM_FAILED ||
        sharVar->semList.hydrogen_involved_sem == SEM_FAILED ||
        sharVar->semList.molecule_in_process_sem == SEM_FAILED ||
        sharVar->semList.pair_of_hydrogen_sem == SEM_FAILED ||
        sharVar->semList.molecule_done_sem == SEM_FAILED ||
        sharVar->semList.oxygen_in_molecule_sem == SEM_FAILED ||
        sharVar->semList.hydrogen_in_molecule_sem == SEM_FAILED) {
        return false;
    }
    return true;
}


/**
 * Closing semaphores in the list
 * @param sharVar
 * @return
 */
bool my_sem_delete(shar_var_t *sharVar) {
    if (sem_close(sharVar->semList.mutex_write_sem) == -1 ||
        sem_close(sharVar->semList.oxygen_ready_sem) == -1 ||
        sem_close(sharVar->semList.oxygen_involved_sem) == -1 ||
        sem_close(sharVar->semList.hydrogen_ready_sem) == -1 ||
        sem_close(sharVar->semList.hydrogen_involved_sem) == -1 ||
        sem_close(sharVar->semList.molecule_in_process_sem) == -1 ||
        sem_close(sharVar->semList.pair_of_hydrogen_sem) == -1 ||
        sem_close(sharVar->semList.molecule_done_sem) == -1 ||
        sem_close(sharVar->semList.oxygen_in_molecule_sem) == -1 ||
        sem_close(sharVar->semList.hydrogen_in_molecule_sem) == -1) {
        return false;
    }
    sem_unlink("~xshevc01.mutex_write_sem~");
    sem_unlink("~xshevc01.oxygen_ready_sem~");
    sem_unlink("~xshevc01.oxygen_involved_sem~");
    sem_unlink("~xshevc01.hydrogen_ready_sem~");
    sem_unlink("~xshevc01.hydrogen_involved_sem~");
    sem_unlink("~xshevc01.molecule_in_process_sem~");
    sem_unlink("~xshevc01.pair_of_hydrogen_sem~");
    sem_unlink("~xshevc01.molecule_done_sem~");
    sem_unlink("~xshevc01.oxygen_in_molecule_sem~");
    sem_unlink("~xshevc01.hydrogen_in_molecule_sem~");
    return true;
}
