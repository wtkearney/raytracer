//
//  primitives.c
//  
//
//  Created by Will Kearney on 3/12/15.
//
//

#include "graphics.h"

/* returns a primitive struct of the specified type */
Primitive *create_primitive(PrimitiveType type) {
    Primitive *obj = malloc(sizeof(Primitive));
    obj->type = type;
    
    obj->color = (Color){{0.0, 0.0, 0.0, 0.0}};
    obj->diffuse = 0;
    obj->specular = 0;
    obj->reflection = 0;
    
    return obj;
}

/* sets color and lighting parameters for a primitive */
void set_parameters(Primitive *obj, Color color, float diffuse, float specular, float reflection) {
    if (obj!=NULL) {
        color_copy( &(obj->color), &(color) );
        obj->diffuse = diffuse;
        obj->specular = specular;
        obj->reflection = reflection;
    } else {
        printf("Error: memory not allocated.\n");
    }
}

/* sets the color of the primitive */
void primitive_setColor(Primitive *obj, Color clr) {
    color_copy( &(obj->color), &(clr));
}

/* sets the lighting parameters of the primitive */
void primitive_setLightParam(Primitive *obj, float diffuse, float specular, float reflection) {
    obj->diffuse = diffuse;
    obj->specular = specular;
    obj->reflection = reflection;
}


/* initializes the sphere to the specified center and radius */
void sphere_set(Primitive *obj, Point center, float radius) {
    if (obj!=NULL) {
        if (obj->type == SphereType) {
            point_copy( &(obj->center), &(center));
            obj->radius = radius;
        } else {
            printf("Type error: primitive specified not a sphere!\n");
            return;
        }
    } else {
        printf("Error: memory not allocated.\n");
    }
}


/*frees the Primitive pointer */
void primitive_free(Primitive *obj) {
    if(obj!=NULL) {
        free(obj);
    }
}

