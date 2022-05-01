// ppm.h
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Aleksandr Shevchenko, FIT
// Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0

#ifndef PPM_H
#define PPM_H

#define PIC_LIM (8000*8000*3)

struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];    // RGB bajty, celkem 3*xsize*ysize
};

struct ppm * ppm_read(const char * filename);

void ppm_free(struct ppm *p);

#endif //PPM_H
