#pragma once

#include"panfile.h"

#include<windows.h>

struct engine_t{
private:
    int buffersize;
    unsigned int *buffer;
    float phi,theta;
    int rrr;
    float rez_h_x[8192],rez_h_z[8192];
    float rez_v_x[8192],rez_v_y[8192],rez_v_z[8192];
    //unsigned char plane[8192];//max width
public:
    panfile_t pf;
    bool need_redraw;
    void on_draw(HWND *hWnd);
    void on_mouse_move(int x,int y);
    void on_mouse_button(int x,int y);
    void on_mouse_wheel(int rot);
    engine_t();
    ~engine_t();
};
