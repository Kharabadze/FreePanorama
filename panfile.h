#pragma once
#include"tiffile.h"
struct panfile_t{
    bool not_loaded;
    tiffile_t tf;
    unsigned int width;
    unsigned int *u,*d,*f,*b,*l,*r;
    int load(const char *filename);
    void freemem(void);
    panfile_t(void);
};
