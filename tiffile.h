#pragma once
#include<stdio.h>
struct tiffile_t{
    bool le,be;
    unsigned int width,height;
    unsigned int bps[3];
    unsigned int strips_number,raws_per_strips;
    unsigned int *strips_offset;
    unsigned int *strips_count;
    void decode(FILE *f,int len,unsigned int *&buf,char pictype);
    int get_data(void *buf, int len, FILE *f);
};
