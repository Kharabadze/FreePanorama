#include"tiffile.h"
int tiffile_t::get_data(void *buf, int len, FILE *f){
    unsigned char c;
    unsigned char *uk = (unsigned char *)buf;
    if(be)uk+=(len-1);
    for(int i=0;i<len;i++){
        if(fread(uk,1,1,f)!=1)break;
        if(le)uk++;else uk--;
    }
    return len;
}
void tiffile_t::decode(FILE *f,int len,unsigned int *&buf,char pictype){
    int start = ftell(f);
    int finish = start + len;
    int fileuk = start;

    unsigned int header;
    fileuk+=fread(&header,1,4,f);
    printf("Header = %08x\n",header);
    if(header==0x2a004d4d){//Motorola
        le=false;be=true;
    }else if(header==0x002a4949){//Intel
        le=true;be=false;
    }else{
        printf("WRONG HEADER IN TIFF\n");
    }
    unsigned int addr=0;
    fileuk+=get_data(&addr,4,f);
    printf("main address = %08x\n",addr);
    fseek(f,start+addr,SEEK_SET);

    unsigned int number_of_ent=0;
    fileuk+=get_data(&number_of_ent,2,f);
    printf("number of entries = %i\n",number_of_ent);
    for(int i=0;i<number_of_ent;i++){
        unsigned int tag_name=0;
        fileuk+=get_data(&tag_name,2,f);
        unsigned int tag_type=0;
        fileuk+=get_data(&tag_type,2,f);
        unsigned int val_count=0;
        fileuk+=get_data(&val_count,4,f);

        unsigned int val_addr=0;
        unsigned int temp=0;
        unsigned int val=0;
        //old tiff :)
        int array_valsize[6] = {0,1,1,2,4,8};
        //calculate value size
        if(tag_type>5)printf("Error! tag_type = %i Only old tiff!!!\n",tag_type);
        int onesize = array_valsize[tag_type];
        int totsize = onesize * val_count;
        if(totsize>4){
            fileuk+=get_data(&val_addr,4,f);
        }else{
            val=0;
            fileuk+=get_data(&val,totsize,f);
            fileuk+=get_data(&temp,4-totsize,f);
        }
        //--- Display
        printf("%04x-%04x-%i,%i|%i\n",tag_name,tag_type,val_count,val,val_addr);
        //--- Processing
        if(tag_name==0x100){//Width
            if(totsize>4)printf("ERROR\n");
            width = val;
        }else if(tag_name==0x101){//Height
            if(totsize>4)printf("ERROR\n");
            height = val;
        }else if(tag_name==0x102){//Bit per sample
            if(val_count!=3)printf("ERROR\n");
            int myret = ftell(f);
            fseek(f,start+val_addr,SEEK_SET);
            for(int i=0;i<3;i++){
                bps[i]=0;
                get_data(&bps[i],onesize,f);
            }
            fseek(f,myret,SEEK_SET);
            //if(totsize>4)printf("ERROR\n");
            //height = val;
        }else if(tag_name==0x103){
            if(totsize>4)printf("ERROR\n");
            if(val!=1)printf("ERROR: Only uncompressed images are supported\n");
        }else if(tag_name == 0x106){
            if(totsize>4)printf("ERROR\n");
            if(val!=2)printf("ERROR: Only RGB image is supported");
        }else if(tag_name == 0x111){
            strips_number = val_count;

            if(totsize<4){
                printf("Too complex tiff :(\n");
            }else{
                int myret=ftell(f);
                fseek(f,start+val_addr,SEEK_SET);
                strips_offset = new unsigned int[strips_number];
                for(int i=0;i<strips_number;i++){
                    get_data(&strips_offset[i],onesize,f);
                }
                fseek(f,myret,SEEK_SET);
            }
        }else if(tag_name==0x115){
            if(val!=3)printf("Only RGB pictures are supported!\n");
        }else if(tag_name==0x116){
            raws_per_strips = val;
        }else if(tag_name==0x117){
            if(strips_number != val_count) printf("Non-equivalent 0x111 and 0x117\n");

            if(totsize<4){
                printf("Too complex tiff :(\n");
            }else{
                int myret=ftell(f);
                fseek(f,start+val_addr,SEEK_SET);
                strips_count = new unsigned int[strips_number];
                for(int i=0;i<strips_number;i++){
                    get_data(&strips_count[i],onesize,f);
                }
                fseek(f,myret,SEEK_SET);
            }
        }
        //process tag
    }
    //--- image info:
    printf("Image info:\n");
    printf("size: %i x %i\n",width,height);
    printf("bps = %i,%i,%i\n",bps[0],bps[1],bps[2]);
    for(int i=0;i<10;i++){
        printf("%i) %i - %i \n",i,strips_offset[i],strips_count[i]);
    }
    //--- image processing
    buf=new unsigned int[width*height];
    int cc=0,xx=0,yy=0;
    for(int i=0;i<strips_number;i++){
        printf("*");
        fseek(f,start+strips_offset[i],SEEK_SET);
        for(int j=0;j<strips_count[i]/3;j++){
            int color=0;
            fread(&color,1,3,f);
            int cc2=cc;
            if(pictype=='f')cc2=yy*width+width-1-xx;
            if(pictype=='l')cc2=xx*width+yy;
            if(pictype=='u')cc2=xx*width+width-1-yy;
            if(pictype=='d')cc2=(width-1-xx)*width+width-1-yy;
            if(pictype=='r')cc2=xx*width+width-1-yy;
            if(pictype=='b')cc2=(width-1-yy)*width+width-1-xx;
            buf[cc2]=
                ((color&0xff)<<16) |
                (color&0xff00) |
                ((color&0xff0000)>>16);
            cc++;
            if(++xx==width){xx=0;yy++;}
        }
    }
    /*
    int bc=0,sc=0;
    for(int i=0;i<height*width;i++){
        int pixel=0;
        for(int c=0;c<3;c++){
            if(bc==0){
                fseek(f,start+strips_offset[sc],SEEK_SET);
            }
            int ccc=0;
            get_data(&ccc,(bps[c]+7)/8,f);
            bc+=(bps[c]+7)/8;
            if(bps[c]>8)ccc>>(bps[c]-8);//16 bit
            (pixel<<=8)|=(ccc&0xff);

            if(bc==strips_count[sc]){
                printf("*");
                sc++;
                bc=0;
            }
        }
        buf[i]=pixel;
    }*/
    //finish
    //printf("\n");
    if(strips_count)delete[]strips_count;
    if(strips_offset)delete[]strips_offset;
    return;
}
