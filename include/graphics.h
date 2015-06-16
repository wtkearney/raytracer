

#ifndef _graphics_h
#define _graphics_h

typedef struct {
    double val[4];
    int zBuffer;
} Point;

typedef Point Vector;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "image.h"
#include "point.h"
#include "matrix.h"
#include "primitives.h"
#include "color.h"
#include "view.h"
#include "light.h"

#include "ray.h"
#include "ppmIO.h"




#endif
