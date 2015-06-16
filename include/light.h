//
//  light.h
//  
//
//  Created by Will Kearney and Hunter Standen on 11/26/14.
//
//

#ifndef _light_h
#define _light_h

//static int MAX_LIGHTS = 64;

// defines type of lights available
typedef enum {
    LightNone,
    LightAmbient,
    LightDirect,
    LightPoint,
    LightSpot,
} LightType;

// light source
typedef struct {
    LightType type;
    Color color;
    Vector direction;
    Point position;
    float cutoff; // stores the cosine of the cutoff angle of a spotlight
    float sharpness; // coefficient of the falloff function (power for cosine)
} Light;

// holds all the lights in a scene
typedef struct {
    int nLights;
    Light light[64];
} Lighting;


// Light functions
void light_init( Light *light );
void light_copy( Light *to, Light *from );

// Lighting functions
Lighting *lighting_create( void );
void lighting_init( Lighting *l );
void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness );
//void lighting_shading( Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c, Polygon *polyList, int numP );

#endif

