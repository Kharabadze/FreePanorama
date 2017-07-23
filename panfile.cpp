#include"panfile.h"

#include<stdio.h>
#include<string.h>

#include"tiffile.h"

FILE *special_file=0;
long long start_pos = 0;
long long finish_pos = 0;


int panfile_t::load(const char *filename){
    width = 0;
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
            int new_pointer = ftell(fi)+comp_size;
            if(!strcmp(loc_filename,"u.tif")){
                tf.decode(fi,comp_size,u,'u');
                if(tf.width!=tf.height)printf("Only Square TIFF!\n");
                if(width==0)width=tf.width;
            }else if(!strcmp(loc_filename,"d.tif")){
                tf.decode(fi,comp_size,d,'d');
                if(tf.width!=tf.height)printf("Only Square TIFF!\n");
                if(width==0)width=tf.width;
            }else if(!strcmp(loc_filename,"f.tif")){
                tf.decode(fi,comp_size,f,'f');
                if(tf.width!=tf.height)printf("Only Square TIFF!\n");
                if(width==0)width=tf.width;
            }else if(!strcmp(loc_filename,"b.tif")){
                tf.decode(fi,comp_size,b,'b');
                if(tf.width!=tf.height)printf("Only Square TIFF!\n");
                if(width==0)width=tf.width;
            }else if(!strcmp(loc_filename,"l.tif")){
                tf.decode(fi,comp_size,l,'l');
                if(tf.width!=tf.height)printf("Only Square TIFF!\n");
                if(width==0)width=tf.width;
            }else if(!strcmp(loc_filename,"r.tif")){
                tf.decode(fi,comp_size,r,'r');
                if(tf.width!=tf.height)printf("Only Square TIFF!\n");
                if(width==0)width=tf.width;
            }else{;}
            fseek(fi,new_pointer,SEEK_SET);
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
