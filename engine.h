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

    //---
    int width,height;

    //--- Matrix
    float sin_phi;
    float cos_phi;
    float sin_theta;
    float cos_theta;

    float mat_xx;
    float mat_xy;
    float mat_xz;

    float mat_yx;
    float mat_yy;
    float mat_yz;

    float mat_zx;
    float mat_zy;
    float mat_zz;

    float rez_0_x;
    float rez_0_y;
    float rez_0_z;

    void calculate_matrix(void);

    //--- Rotation
    float fixed_x,fixed_y,fixed_z;

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
