#include"panfile.h"

#include<stdio.h>
#include<string.h>

//#include"tiff/tiff.h"
//#include"tiff/tiffio.h"

FILE *special_file=0;
long long start_pos = 0;
long long finish_pos = 0;
/*
tsize_t special_read(thandle_t st,tdata_t buffer,tsize_t size){
    long long bytes_read = fread(buffer,1,size,special_file);
    long long curpos = ftell(special_file);
    if (curpos > finish_pos){
        bytes_read -= curpos - finish_pos;
        fseek(special_file,finish_pos,SEEK_SET);
    }
    return bytes_read;
};

tsize_t special_write(thandle_t st,tdata_t buffer,tsize_t size){
    //Not Implemented :)
    return 0;
};

int special_close(thandle_t){
    fseek(special_file,finish_pos,SEEK_SET);
    return 0;
};

toff_t special_seek(thandle_t st,toff_t pos, int whence){
    if (pos == 0xFFFFFFFF)
       return 0xFFFFFFFF;
    switch (whence) {
        case SEEK_SET: {
            fseek(special_file,start_pos+pos,SEEK_SET);
            break;
        }
        case SEEK_CUR: {
            fseek(special_file,pos,SEEK_CUR);
            break;
        }
        case SEEK_END: {
            fseek(special_file,finish_pos+pos,SEEK_SET);
            break;
        }
    }

    //=== correct
    long long curpos = ftell(special_file);
    if (curpos > finish_pos){
        fseek(special_file,finish_pos,SEEK_SET);
        curpos = finish_pos;
    }
    if (curpos < start_pos){
        fseek(special_file,start_pos,SEEK_SET);
        curpos = start_pos;
    }
    curpos-=start_pos;

    return curpos;
};

toff_t special_size(thandle_t st){
    return (finish_pos-start_pos);
};

int special_map(thandle_t, tdata_t*, toff_t*){
    return 0;
};

void special_unmap(thandle_t, tdata_t, toff_t){
    return;
};
*/
int decode(unsigned char * &buf,FILE *f,int len){
    start_pos = ftell(f);
    finish_pos = start_pos + len;

    //--- Start decoding
    special_file = f;
/*
    TIFF* tif = TIFFClientOpen(
    "Memory", "r", (thandle_t)0,
    special_read, special_write, special_seek,
    special_close, special_size,
    special_map, special_unmap);


    unsigned char *temp = new unsigned char [len];
*/
    //--- Finish decoding


    //if(fread(*temp,1,comp_size,f)!=comp_size)break;
    //debug saving
    //FILE *debf=fopen("test.tif","wb");
    //fwrite(*temp,1,comp_size,debf);
    //fclose(debf);

    fseek(f,finish_pos,SEEK_SET);
    return 0;
}


int panfile_t::load(const char *filename){
    u=d=f=b=l=r=0;//Empty pointers
    FILE *fi=fopen(filename,"rb");

    unsigned int header=0;
    unsigned short vers_ext=0;
    unsigned short gp_bit_flag=0;
    unsigned short compression_method=0;
    unsigned short mod_time=0;
    unsigned short mod_date=0;
    unsigned int crc32=0;
    unsigned int comp_size=0;
    unsigned int uncomp_size=0;
    unsigned short file_name_len=0;
    unsigned short extra_len=0;
    char *loc_filename=0;
    char temp=0;

    while(1){//FILE LOOP
        if(fread(&header,1,4,fi)!=4)break;
        if(header==0x04034b50){//File header
            if(fread(&vers_ext,1,2,fi)!=2)break;
            if(fread(&gp_bit_flag,1,2,fi)!=2)break;
            if(fread(&compression_method,1,2,fi)!=2)break;
            if(fread(&mod_time,1,2,fi)!=2)break;
            if(fread(&mod_date,1,2,fi)!=2)break;
            if(fread(&crc32,1,4,fi)!=4)break;
            if(fread(&comp_size,1,4,fi)!=4)break;
            if(fread(&uncomp_size,1,4,fi)!=4)break;
            printf("size = %i = %i (bytes)\n",comp_size,uncomp_size);
            if(fread(&file_name_len,1,2,fi)!=2)break;
            if(fread(&extra_len,1,2,fi)!=2)break;
            loc_filename = new char[file_name_len+1];
            for(int i=0;i<file_name_len;i++){
                if(fread(&loc_filename[i],1,1,fi)!=1)break;
                loc_filename[i+1]=0;
            }
            printf("filename = %s\n",loc_filename);
            for(int i=0;i<extra_len;i++){
                if(fread(&temp,1,1,fi)!=1)break;
            }

            if(gp_bit_flag!=0)break;//Only simple file
            //if(compression_method!=0)break;//Only uncompressed file
            //if(comp_size!=uncomp_size)break;//Only uncompressed file

            unsigned char **temp=0;
            if(!strcmp(loc_filename,"u.tif")){
                decode(u,fi,comp_size);
            }else if(!strcmp(loc_filename,"d.tif")){
                decode(d,fi,comp_size);
            }else if(!strcmp(loc_filename,"f.tif")){
                decode(f,fi,comp_size);
            }else if(!strcmp(loc_filename,"b.tif")){
                decode(b,fi,comp_size);
            }else if(!strcmp(loc_filename,"l.tif")){
                decode(l,fi,comp_size);
            }else if(!strcmp(loc_filename,"r.tif")){
                decode(r,fi,comp_size);
            }else{
                fseek(fi,comp_size,SEEK_CUR);
            }
        }else if(header==0x08074b50){//After-File info
            unsigned char c=0;
            for(int i=0;i<12;i++){//skip
                if(fread(&c,1,1,fi)!=1)break;
            }
        }else if(header==0x02014b50){//Central directory item
            break;//Ignore
        }else if(header==0x06054b50){//Finish Central directory
            break;//Ignore
        }else break;//incorrect file
    }
    fclose(fi);
    return 0;
}
