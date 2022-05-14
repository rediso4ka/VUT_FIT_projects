// ppm.c
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ppm.h"
#include "error.h"

struct ppm * ppm_read(const char *filename){
    /*
     * Cteni souboru:
     */
    FILE *f = fopen(filename, "rb");
    if (f == NULL){
        warning_msg("%s: file se nepodarilo otevrit!\n",__func__);
        goto error_state;
    }

    /*
     * Cteni "Metadat" ze souboru:
     */
    char ppm_type[3];
    unsigned xsize;
    unsigned ysize;
    unsigned max_color;
    if (fscanf(f, "%2s %u %u %u", ppm_type, &xsize, &ysize, &max_color) != 4 ){
        warning_msg("%s: ppm info error!\n",__func__);
        goto error_state;
    }

    /*
     * Pocet bajtu dat obrazku:
     */
    unsigned long size = xsize * ysize * 3;
    if (size > PIC_LIM){
        warning_msg("%s: spatna velikost ppm!",__func__);
        goto error_state;
    }

    if (strcmp(ppm_type, "P6") != 0){
        goto exit_state;
    }

    if (max_color != 255){
        warning_msg("%s: spatny format barev!\n",__func__);
        goto error_state;
    }

    struct ppm *pic = (struct ppm *)calloc(1, sizeof(struct ppm) + size);
    if (pic == NULL){
        warning_msg("%s: calloc se neprovedl!\n",__func__);
        free(pic);
        goto error_state;
    }
    pic->xsize = xsize;
    pic->ysize = ysize;

    /*
     * Cteme whitespace pred daty:
     */
    fgetc(f);

    /*
     * Cteme data z ppm:
     */
    if (fread(pic->data, 1, size, f) != size){
        warning_msg("%s: mala velikost ppm!\n",__func__);
        free(pic);
        goto error_state;
    }
    if (fgetc(f) != EOF){
        warning_msg("%s: prilis velka velikost ppm!\n",__func__);
        free(pic);
        goto error_state;
    }
    fclose(f);
    return pic;

    exit_state:
    fclose(f);
    error_exit("chybny format ppm!\n");

    error_state:
    fclose(f);
    return NULL;
}

void ppm_free(struct ppm *p){
    free(p);
}