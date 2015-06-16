//  point.c
//  
//
//  Created by Hunter Standen on 9/22/14.
//

#include "graphics.h"



void point_print(Point *p, FILE *fp) {
  fprintf(fp, "(%f, %f, %f)\n", p->val[0], p->val[1], p->val[2] );
}

//sets the first two valyes of the vector to x and y, sets the 3rd and 4th to 0.0
//and 1.0
void point_set2D(Point *p, double x, double y) {
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=0.0;
    p->val[3]=1.0;
}

//sets the points vlayes to x and y and z and sets the homogenous coord to 1.0
void point_set3D(Point *p, double x, double y, double z) {
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=z;
    p->val[3]=1.0;

}

//sets the four values of the vector to x, y, z, and h respectively
void point_set(Point *p, double x, double y, double z, double h) {
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=z;
    p->val[3]=h;
}

//copies the point data structure
void point_copy(Point *to, Point *from) {
    to->val[0]=from->val[0];
    to->val[1]=from->val[1];
    to->val[2]=from->val[2];
    to->val[3]=from->val[3];
}

//draws the point into src using color c
void point_draw(Point *p, Image *src, Color c) {
    image_setz(src, p->val[1], p->val[0], 1/p->val[3]);
    image_setColor(src, p->val[1], p->val[0], c);
    
}

//draws the p into src using FPixel c
void point_drawf(Point *p, Image *src, FPixel c) {
    Color co;
    co.c[0]=c.rgb[0];
    co.c[1]=c.rgb[1];
    co.c[2]=c.rgb[2];
    image_setColor(src, p->val[1], p->val[0], co);
}

// Normalize point
void point_normalize(Point *pt) {
    pt->val[0] = (pt->val[0] / pt->val[3]);
    pt->val[1] = (pt->val[1] / pt->val[3]);
}



