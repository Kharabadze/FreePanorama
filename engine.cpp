#include"engine.h"


#include<math.h>
void engine_t::calculate_matrix(void){
    float rlength = sqrt(width*height);
	float r = rlength * pow(2.0f, rrr / 12.0f);

    //--- calc
    sin_phi = sin(phi);
    cos_phi = cos(phi);
    sin_theta = sin(theta);
    cos_theta = cos(theta);

    mat_xx=cos_phi;
    mat_xy=sin_phi*sin_theta;
    mat_xz=sin_phi*cos_theta;

    mat_yx=0.0f;
    mat_yy=cos_theta;
    mat_yz=-sin_theta;

    mat_zx=-sin_phi;
    mat_zy=cos_phi*sin_theta;
    mat_zz=cos_phi*cos_theta;

    rez_0_x=-mat_xz*r;
    rez_0_y=-mat_yz*r;
    rez_0_z=-mat_zz*r;

    return;
}

void engine_t::on_draw(HWND *hWnd){
	PAINTSTRUCT ps;
    ps.fErase=false;
    BeginPaint(*hWnd, &ps);
    HDC CDC=GetDC(*hWnd);

	RECT my_rect;//Size of window
	GetClientRect(*hWnd,&my_rect);

	width=my_rect.right-my_rect.left;
	height=my_rect.bottom-my_rect.top;

	// Length calculation

    int newsize=width*height;
    if(newsize>buffersize){
        if((buffer!=0)&&(buffersize!=0))delete[]buffer;
        buffersize=newsize*2;//Double it :)
        buffer=new unsigned int[buffersize];
    }

    //float rlength = sqrt(width*height);
    calculate_matrix();

    for(int i=0;i<width;i++){
        float xr = i-width*0.5f+0.5f;
        rez_h_x[i] = mat_xx * xr;
        rez_h_z[i] = mat_zx * xr;
    }

    for(int i=0;i<height;i++){
        float yr = i-height*0.5f+0.5f;
        rez_v_x[i] = mat_xy * yr;
        rez_v_y[i] = mat_yy * yr;
        rez_v_z[i] = mat_zy * yr;
    }

    unsigned int *cur_pix=buffer;//pixel
    float coordA[3],coord[3];
    unsigned int *pictar[6]={pf.f,pf.b,pf.l,pf.r,pf.d,pf.u};
    int coordshift[3]={0,1,2};
    for(int y=0;y<height;y++){
        coordA[0]=rez_0_x+rez_v_x[y];
        coord[1] =rez_0_y+rez_v_y[y];
        coordA[2]=rez_0_z+rez_v_z[y];
        for(int x=0;x<width;x++){
            coord[0]=coordA[0]+rez_h_x[x];
            coord[2]=coordA[2]+rez_h_z[x];

            //--- Artefact remove
            int loop = 0;
            //int saved_colors[3];
            //int *saco=saved_colors;

            while(1){
                float xf = coord[coordshift[0]];
                float yf = coord[coordshift[1]];
                float zf = coord[coordshift[2]];
                float coef = pf.width*0.5f/zf;
                int xi=int(xf*coef + pf.width*0.5f );
                int yi=int(yf*coef + pf.width*0.5f );
                //int xi = x; int yi = y;
                if((xi>=0)&&(xi<pf.width)&&(yi>=0)&&(yi<pf.width)){
                    *cur_pix = pictar[(zf<0)?0:1][yi*pf.width+xi];
                    break;
                }else{
                    //Artefact remove
                    //int xart=xi,yart=yi;
                    //if(xart<0)xart=0;if(xart>=pf.width)xart=pf.width-1;
                    //if(yart<0)yart=0;if(yart>=pf.width)yart=pf.width-1;
                    //*(saco++) = pictar[(zf<0)?0:1][yart*pf.width+xart];

                    unsigned int *temp;
                    temp=pictar[0];pictar[0]=pictar[2];pictar[2]=pictar[4];pictar[4]=temp;
                    temp=pictar[1];pictar[1]=pictar[3];pictar[3]=pictar[5];pictar[5]=temp;
                    int ctemp=coordshift[0];
                    coordshift[0]=coordshift[1];
                    coordshift[1]=coordshift[2];
                    coordshift[2]=ctemp;
                    if(++loop==3){
                        *cur_pix = 0xff3456;//Show Artefact

                        //Artefact remove
                        //int r=0,g=0,b=0;
                        //for(int i=0;i<3;i++){
                        //    r+=saved_colors[i]&0xff;
                        //    g+=(saved_colors[i]>>8)&0xff;
                        //    b+=(saved_colors[i]>>16)&0xff;
                        //}
                        //r/=3;g/=3;b/=3;
                        //*cur_pix = (b<<16)|(g<<8)|r;

                        break;
                    }
                }
                //break;
            }
            cur_pix++;
        }
    }

    //--- Output
	BITMAPINFO lpbmi;
	//===
	//lpbmi.bmiColors[0].rgbBlue=0;
	//lpbmi.bmiColors[0].rgbGreen=0;
	//lpbmi.bmiColors[0].rgbRed=0;
	//lpbmi.bmiColors=NULL;
	lpbmi.bmiHeader.biBitCount=32;//24;
	lpbmi.bmiHeader.biClrImportant=1;
	lpbmi.bmiHeader.biClrUsed=1;
	lpbmi.bmiHeader.biCompression=BI_RGB;//0;
	lpbmi.bmiHeader.biHeight=height;//head2.biHeight;
	lpbmi.bmiHeader.biPlanes=1;
	lpbmi.bmiHeader.biSize=sizeof(lpbmi);
	lpbmi.bmiHeader.biSizeImage=width*height*4;//head2.biHeight*head2.biWidth*4;
	lpbmi.bmiHeader.biWidth=width;//head2.biWidth;
	lpbmi.bmiHeader.biXPelsPerMeter=1;
	lpbmi.bmiHeader.biYPelsPerMeter=1;
	//HDC dc = GetDC(0);//Thanx to my friends :)
	int x_dot=0,y_dot=0;
	SetDIBitsToDevice(
		CDC, //dc,//Device Context (Full screen)
		0,//X dest
		0,//Y dest
		width,//head2.biWidth,//XWidth
		height,//head2.biHeight,//YWidth
		0,//??? Xsrc
		0,//??? Ysrc
		0,//??? uStartScan
		height,//head2.biHeight,//ScanLines
		buffer,//My own idea
		&lpbmi,
		DIB_RGB_COLORS//No Palette
	);

	ReleaseDC(*hWnd,CDC);
	EndPaint(*hWnd, &ps);
    return;
}
void engine_t::on_mouse_move(int x,int y){
    float xr = x-width*0.5f+0.5f;
    float yr = y-height*0.5f+0.5f;

    float step = 0.15;//Rad
    while(step>0.00001){
        float old_theta = theta;
        float old_phi = phi;
        int mini=0,minj=0;
        float minrez = 10000.0f;
        for(int i=-1;i<=1;i++){
            phi = old_phi+step*i;
            if(phi>M_PI)phi-=2*M_PI;
            if(phi<-M_PI)phi+=2*M_PI;
            for(int j=-1;j<=1;j++){
                theta = old_theta+step*j;
                if(theta>M_PI*0.5){theta=M_PI-theta;phi+=M_PI;}
                if(theta<-M_PI*0.5){theta=-M_PI-theta;phi+=M_PI;}
                if(phi>M_PI)phi-=2*M_PI;

                calculate_matrix();
                float new_x = mat_xx * xr + mat_xy * yr + rez_0_x;
                float new_y = mat_yx * xr + mat_yy * yr + rez_0_y;
                float new_z = mat_zx * xr + mat_zy * yr + rez_0_z;
                float norm = sqrt(new_x*new_x +
                    new_y*new_y + new_z*new_z);
                new_x /= norm;
                new_y /= norm;
                new_z /= norm;
                float d_x = new_x-fixed_x;
                float d_y = new_y-fixed_y;
                float d_z = new_z-fixed_z;
                float diff = d_x*d_x+d_y*d_y+d_z*d_z;
                if(diff<minrez){
                    minrez = diff;
                    mini=i;
                    minj=j;
                }
            }
        }
        if((mini==0)&&(minj==0))step*=0.3;
        else{
            phi = old_phi+step*mini;
            if(phi>M_PI)phi-=2*M_PI;
            if(phi<-M_PI)phi+=2*M_PI;
            theta = old_theta+step*minj;
            if(theta>M_PI*0.5){theta=M_PI-theta;phi+=M_PI;}
            if(theta<-M_PI*0.5){theta=-M_PI-theta;phi+=M_PI;}
            if(phi>M_PI)phi-=2*M_PI;
            step*=0.98;
        }
    }
    need_redraw=true;
    return;
}
void engine_t::on_mouse_button(int x,int y){
    float xr = x-width*0.5f+0.5f;
    float yr = y-height*0.5f+0.5f;

    calculate_matrix();
    fixed_x = mat_xx * xr + mat_xy * yr + rez_0_x;
    fixed_y = mat_yx * xr + mat_yy * yr + rez_0_y;
    fixed_z = mat_zx * xr + mat_zy * yr + rez_0_z;
    float norm = sqrt(fixed_x*fixed_x+
        fixed_y*fixed_y+fixed_z*fixed_z);
    fixed_x/=norm;
    fixed_y/=norm;
    fixed_z/=norm;
    return;
}
void engine_t::on_mouse_wheel(int rot){
    rrr+=rot/120;
    need_redraw=true;
    return;
}

//---
engine_t::engine_t(){
    buffersize=0;
    buffer=0;
    theta = 0.3;
    phi = 3.14/2;
    rrr = 0;
    return;
}
engine_t::~engine_t(){
    return;
}
