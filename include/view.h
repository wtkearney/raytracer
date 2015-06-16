//
//  view.h
//  
//
//  Created by Hunter Standen on 10/6/14.
//
//

#ifndef _view_h
#define _view_h

typedef struct {
    Point vrp;
    float dx;
    Vector x;
    int screenx;
    int screeny;
} View2D;

typedef struct {
    Point vrp;
    Vector vpn;
    Vector vup;
    double d;
    double du;
    double dv;
    double f;
    double b;
    int screenx;
    int screeny;
} View3D;

void matrix_setView2D(Matrix *vtm, View2D *view);
void matrix_setView3D(Matrix *vtm, View3D *view);
void view2D_set( View2D *view, Point *vrp, float dx, Vector *xaxis, int cols, int rows );

#endif