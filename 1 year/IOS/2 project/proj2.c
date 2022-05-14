// proj2.c
// Solution IOS-project 2, 25.4.2022
// Author: Aleksandr Shevchenko, FIT
// Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "proj2.h"
#include "parsing.h"
#include "shared.h"
#include "sem.h"
#include "process.h"
#include "message.h"


shar_var_t *sharVar = NULL;


int main(int argc, char *argv[]) {
    // Safety unlinking
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

    // Parsing arguments
    params_t *params = parse_args(argc, argv);
    if (params == NULL) {
        error_exit("%s: parameters are not allocated!\n", __func__);
    }

    // Initialising shared memory
    SHAR_VAR_INIT(sharVar);
    if (sharVar == MAP_FAILED) {
        free(params);
        error_exit("%s: shared variable is not initialised!\n", __func__);
    }

    // Start values
    shar_var_start(sharVar);

    // Opening output file
    if (shar_var_open_output(sharVar) == false) {
        free(params);
        error_exit("%s: output file is not open!\n");
    }

    // Create semaphores
    if (my_sem_make(sharVar) == false) {
        my_sem_delete(sharVar);
        free(params);
        error_exit("%s: semaphores are not created!\n");
    }

    // Starting processes
    process_init(params, sharVar);

    // Waiting for children
    while (wait(NULL) >= 0);

    // Delete parameters
    free(params);

    // Closing semaphores
    my_sem_delete(sharVar);

    // Closing output file
    if (shar_var_close_output(sharVar) == false) {
        error_exit("%s: output file is not closed!\n", __func__);
    }

    // Delete shared variable
    if (shar_var_delete(sharVar) == false) {
        error_exit("%s: shared variable is not deleted!\n", __func__);
    }
    return 0;
}