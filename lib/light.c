//
//  light.c
//  
//
//  Created by Will Kearney and Hunter Standen on 11/26/14.
//
//

#include "graphics.h"


// Light functions

/* initialize the light to default values. */
void light_init( Light *light ) {
    Vector v;
    vector_set(&v, 1.0, 1.0, 1.0);
    
    Point p;
    point_set(&p, 1.0, 1.0, 1.0, 1.0);
    
    light->type = LightNone;
    light->color = (Color){{1.0, 1.0, 1.0, 1.0}};
    vector_copy(&(light->direction), &(v));
    point_copy(&(light->position), &(p));
    light->cutoff = 0.0;
    light->sharpness = 1.0;
}

/* copy the light information. */
void light_copy( Light *to, Light *from ) {
    to->type = from->type;
    color_copy(&(to->color), &(from->color));
    vector_copy(&(to->direction), &(from->direction));
    point_copy(&(to->position), &(from->position));
    to->cutoff = from->cutoff;
    to->sharpness = from->sharpness;
}

// Lighting functions

/* allocate and return a new lighting structure set to default values. */
Lighting *lighting_create( void ) {
    Lighting *l = malloc( sizeof(Lighting) );
    //l->light=malloc(sizeof(Light)*64);
    lighting_init(l);
    return l;
}

/* initialize the lighting structure to default values. */
void lighting_init( Lighting *l ) {
    l->nLights = 0;
    //l->light=malloc(sizeof(Light)*64);
    for (int i=0;i<64;i++) {
        light_init( &(l->light[i]) );
    }
}

/* add a new light to the Lighting structure given the parameters, some of which may be NULL,
 depending upon the type. Make sure you don’t add more lights than MAX LIGHTS. */
void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness ) {
    int lightIdx = l->nLights;
    if (lightIdx >= 64) {
        printf("Reached max light limit of 64.\n");
        return;
    } else {
        l->light[lightIdx].type = type;
        color_copy( &(l->light[lightIdx].color), (c) );
        if(dir!=NULL) {
            vector_copy( &(l->light[lightIdx].direction), (dir) );
        }
        if(pos!=NULL) {
            point_copy( &(l->light[lightIdx].position), (pos) );
        }
        l->light[lightIdx].cutoff = cos(cutoff*M_PI/180.0);
        l->light[lightIdx].sharpness = sharpness;
        l->nLights = l->nLights + 1;
    }
    //printf("light %d: %f %f %f\n", l->nLights-1, l->light[l->nLights-1].color.c[0], l->light[l->nLights-1].color.c[1], l->light[l->nLights-1].color.c[2]);
}



