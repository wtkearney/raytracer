//
//  primitives.h
//  
//
//  Created by Will Kearney on 3/10/15.
//
//

#ifndef _primitives_h
#define _primitives_h

// defines type of primitives
typedef enum {
    SphereType,
    PlaneType,
} PrimitiveType;

//// Sphere primitive
//typedef struct {
//    PrimitiveType type;
//    Point center;
//    float radius;
//    
//    Color color;
//    float diffuse;
//    float specular;
//    float reflection;
//} Sphere;

//// Plane primitive
//typedef struct {
//    PrimitiveType type;
//    int nVertex;        // number of vertices
//    Point *vertex;      // vertex information
//    Vector *normal;     // surface normal information for each vertex
//    
//    Color color;
//    float diffuse;
//    float specular;
//    float reflection;
//} Polygon;


// Primitive struct
typedef struct {
    PrimitiveType type;
    
    // sphere info
    Point center;
    float radius;
    
    // polygon info
    int nVertex;        // number of vertices
    Point *vertex;      // vertex information
    Vector *normal;     // surface normal information for each vertex
    
    // shared info
    Color color;
    float diffuse;
    float specular;
    float reflection;
    
} Primitive;


Primitive *create_primitive(PrimitiveType type);
void set_parameters(Primitive *obj, Color color, float diffuse, float specular, float reflection);
void primitive_setColor(Primitive *obj, Color clr);
void primitive_setLightParam(Primitive *obj, float diffuse, float specular, float reflection);
void sphere_set(Primitive *obj, Point center, float radius);
void primitive_free(Primitive *obj);

#endif