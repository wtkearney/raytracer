//
//  ray.h
//  
//
//  Created by Will Kearney and Hunter Standen on 12/11/14.
//
//
#include <stdio.h>
#include <stdlib.h>

#ifndef _ray_h
#define _ray_h

typedef enum {
    ShadowRay,
    RefractionRay,
    ReflectionRay,
} RayType;

// ray data structure
typedef struct {
    Vector direction;
    Point origin;
    Point end;
} Ray;

void ray_set( Ray *ray, Point *origin, Point *end );
void ray_copy( Ray *to, Ray *from );
void ray_trace(Image *src, Primitive *pList, int numP, Matrix *VTM, Matrix *GTM, Lighting *lighting, View3D *view);
Color rayIntersect(Ray *ray, Primitive *plist, int numP, Lighting *lighting, int level);

#endif
