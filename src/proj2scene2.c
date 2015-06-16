/*
	Will Kearney
	Fall 2014
 
	Uses ray tracing to render basic scene; rotate view around origin
 
 */

#include "graphics.h"

int main(int argc, char *argv[]) {
    
    // create and init image
    Image *src;
    int rows = 1000;
    int cols = 1000;
    src = image_create(rows, cols);
    
    char buffer[256];
    
    // viewing matrices
    Matrix VTM, GTM;
    matrix_identity(&VTM);
    matrix_identity(&GTM);
    
    // lighting
    Lighting *light;
    light = lighting_create();
    Point lightLoc;
    
    // scene objects
    Primitive *sphere1, *sphere2;
    Point center1;
    float radius1;
    
    // primitive list
    int numP = 2;
    Primitive *pList;
    pList = malloc( sizeof(Primitive)*numP);
    
    // colors
    Color Red, Blue, PointColor, PointColor2, AmbientColor;
    color_set(&Red, 0.9, 0.1, 0.1, 1);
    color_set(&Blue, 0.1, 0.1, 0.9, 1);
    color_set(&AmbientColor, 0.15, 0.15, 0.15, 1);
    color_set(&PointColor, 0.6, 0.5, 0.5, 1);
    color_set(&PointColor2, 0.3, 0.3, 0.3, 1);
    
    // set the View parameters
    View3D view;
    point_set3D(&(view.vrp), 0.0, 2.0, 5.0);
    vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);  // look at origin
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 2.0;
    view.du = 5.0;
    view.dv = 5.0;
    view.f = 1.0;
    view.b = 100;
    view.screenx = cols;
    view.screeny = rows;
    //matrix_setView3D(&VTM, &view);
    
    // print out VTM
    printf("Final VTM: \n");
    matrix_print(&VTM, stdout);

    
    // create sphere1
    point_set3D(&center1, -3.0, -0.5, 0.0);
    radius1 = 2.f;
    sphere1 = create_primitive(SphereType);
    sphere_set(sphere1, center1, radius1);
    // set diffuse, specular, and reflection coefficients
    set_parameters(sphere1, Blue, 0.9, 0.25, 0.4);
    
    // create sphere2
    point_set3D(&center1, 3.0, 0.0, 0.0);
    radius1 = 2.f;
    sphere2 = create_primitive(SphereType);
    sphere_set(sphere2, center1, radius1);
    // set diffuse, specular, and reflection coefficients
    set_parameters(sphere2, Red, 0.5, 0.5, 0.05);
    
    
    
    // populate primitive list
    pList[0] = *sphere1;
    pList[1] = *sphere2;
    

    // point lights
    point_set3D(&lightLoc, -4.0, -2.0, 2.0);
    lighting_add( light, LightPoint, &PointColor, NULL, &lightLoc, 0, 0 );
    
    point_set3D(&lightLoc, 4.0, -2.0, 2.0);
    lighting_add( light, LightPoint, &PointColor2, NULL, &lightLoc, 0, 0 );
    
    point_set3D(&lightLoc, 0.0, 6.0, 5.0);
    lighting_add( light, LightPoint, &PointColor2, NULL, &lightLoc, 0, 0 );
    
    lighting_add( light, LightAmbient, &AmbientColor, NULL, NULL, 0, 0 );
    
    printf("Num Lights: %d\n", light->nLights);
    
    // ray tracing algorithm
    ray_trace(src, pList, numP, &VTM, &GTM, light, &view);
    
    // write out, convert, and open image
    image_write(src, "proj2scene2.ppm");
    system("convert proj2scene2.ppm proj2scene2.png");
    system("open proj2scene2.png");
    

    // ought to free stuff here
    primitive_free(sphere1);
    image_free(src);
    
    return(0);
}
