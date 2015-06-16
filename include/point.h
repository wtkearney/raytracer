//  point.h
//  
//
//  Created by Hunter Standen on 9/22/14.
//

#ifndef _point_h
#define _point_h

#include "graphics.h"


void point_print(Point *p, FILE *fp);
//sets the first two valyes of the vector to x and y, sets the 3rd and 4th to 0.0
//and 1.0
void point_set2D(Point *p, double x, double y);

//sets the points vlayes to x and y and z and sets the homogenous coord to 1.0
void point_set3D(Point *p, double x, double y, double z);

//sets the four values of the vector to x, y, z, and h respectively
void point_set(Point *p, double x, double y, double z, double h);

//copies the point data structure
void point_copy(Point *to, Point *from);

//draws the point into src using color c
void point_draw(Point *p, Image *src, Color c);

//draws the p into src using FPixel c
void point_drawf(Point *p, Image *src, FPixel c);

void point_normalize(Point *pt);

#endif
