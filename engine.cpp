#include"engine.h"


#include<math.h>

void engine_t::on_draw(HWND *hWnd){
	PAINTSTRUCT ps;
    ps.fErase=false;
    BeginPaint(*hWnd, &ps);
    HDC CDC=GetDC(*hWnd);

	RECT my_rect;//Size of window
	GetClientRect(*hWnd,&my_rect);

	int width=my_rect.right-my_rect.left;
	int height=my_rect.bottom-my_rect.top;

    int newsize=width*height;
    if(newsize>buffersize){
        if((buffer!=0)&&(buffersize!=0))delete[]buffer;
        buffersize=newsize*2;//Double it :)
        buffer=new unsigned int[buffersize];
    }
    unsigned char copter=0b000110;
    //--- calc
    float sin_phi = sin(phi);
    float cos_phi = cos(phi);
    float sin_theta = sin(theta);
    float cos_theta = cos(theta);

    float mat_xx=cos_phi;
    float mat_xy=sin_phi*sin_theta;
    float mat_xz=sin_phi*cos_theta;

    float mat_yx=0.0f;
    float mat_yy=cos_theta;
    float mat_yz=-sin_theta;

    float mat_zx=-sin_phi;
    float mat_zy=cos_phi*sin_theta;
    float mat_zz=cos_phi*cos_theta;

    float rez_0_x=-mat_xz*r;
    float rez_0_y=-mat_yz*r;
    float rez_0_z=-mat_zz*r;

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
            int loop = 0;
            while(1){
                float xf = coord[coordshift[0]];
                float yf = coord[coordshift[1]];
                float zf = coord[coordshift[2]];
                float coef = pf.width*0.4999f/zf;
                int xi=int(xf*coef + pf.width*0.5f + 0.5f);
                int yi=int(yf*coef + pf.width*0.5f + 0.5f);
                //int xi = x; int yi = y;
                if((xi>=0)&&(xi<pf.width)&&(yi>=0)&&(yi<pf.width)){
                    *cur_pix = pictar[(zf<0)?0:1][yi*pf.width+xi];
                    break;
                }else{
                    unsigned int *temp;
                    temp=pictar[0];pictar[0]=pictar[2];pictar[2]=pictar[4];pictar[4]=temp;
                    temp=pictar[1];pictar[1]=pictar[3];pictar[3]=pictar[5];pictar[5]=temp;
                    int ctemp=coordshift[0];
                    coordshift[0]=coordshift[1];
                    coordshift[1]=coordshift[2];
                    coordshift[2]=ctemp;
                    if(++loop==3){
                        *cur_pix = 0xff3456;
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
    return;
}
void engine_t::on_mouse_button(int x,int y){
    return;
}
void engine_t::on_mouse_wheel(int r){
    return;
}

//---
engine_t::engine_t(){
    buffersize=0;
    buffer=0;
    theta = 0.3;
    phi = 3.14/2;
    r = 100;
    return;
}
engine_t::~engine_t(){
    return;
}
