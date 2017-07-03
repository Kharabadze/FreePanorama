#pragma once

struct panfile_t{
    unsigned char *u,*d,*f,*b,*l,*r;
    int load(const char *filename);
    void freemem(void);
};
