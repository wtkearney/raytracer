//  view.c
//  
//
//  Created by Hunter Standen on 10/6/14.
//

#include "graphics.h"

void matrix_setView2D(Matrix *vtm, View2D *view) {
    matrix_identity(vtm);
    
    float dy=(view->dx*view->screeny)/view->screenx;
    
    matrix_translate2D(vtm, -view->vrp.val[0], -view->vrp.val[1]);
    
    matrix_rotateZ(vtm, view->x.val[0], view->x.val[1]);
    
    matrix_scale2D(vtm, view->screenx/view->dx, -view->screeny/dy);

    matrix_translate2D(vtm, view->screenx/2, view->screeny/2);

}

void matrix_setView3D(Matrix *vtm, View3D *view) {
    matrix_identity(vtm);
    
    //step 1
    matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);
    
    
    //step 2
    Vector u;
    Vector vup;
    Vector vpn;
    
    vector_cross(&view->vup, &view->vpn, &u);
    
    vector_cross(&view->vpn, &u, &vup);

    vector_copy(&vpn, &view->vpn);
    
//    printf("Before normalizing: \n");
//    vector_print( &u, stdout);
//    vector_print( &vup, stdout);
//    vector_print( &vpn, stdout);
    
    vector_normalize(&vpn);
    vector_normalize(&vup);
    vector_normalize(&u);
//    printf("After normalizing: \n");
//    matrix_print( vtm, stdout);
    
    
    //step 3
//    printf("Step 3: \n");
    matrix_rotateXYZ(vtm, &u, &vup, &vpn);
//    matrix_print( vtm, stdout);
    

    //step 4
//    printf("Step 4: \n");
    matrix_translate(vtm, 0, 0, view->d);
//    matrix_print( vtm, stdout);
    

    //step 5
//    printf("Step 5: \n");
//    matrix_print( vtm, stdout);
    double b=view->d+view->b;
    
    //step 6
//    printf("Step 6: \n");
    matrix_scale(vtm, (2*view->d)/(b*view->du), (2*view->d)/(b*view->dv), 1/b);
//    matrix_print( vtm, stdout);
//    double f=(view->f+view->d)/b;
    
    
    //step 7
//    printf("Step 7: \n");
//    matrix_print( vtm, stdout);
    double d=view->d/b;
    
    //step 8
//    printf("Step 8: \n");
    matrix_perspective(vtm, d);
//    matrix_print( vtm, stdout);
    
    
    //step 9
//    printf("Step 9: \n");
    matrix_scale(vtm, -view->screenx/(2*d), -view->screeny/(2*d), 1);
    matrix_translate(vtm, view->screenx/2, view->screeny/2, 0);
//    matrix_print( vtm, stdout);
}

void view2D_set( View2D *view, Point *vrp, float dx, Vector *xaxis, int cols, int rows ) {

    point_copy(&view->vrp, vrp);
    
    view->dx = dx;
    vector_normalize(xaxis);
    vector_copy(&view->x, xaxis);
    
    view->screenx = cols;
    view->screeny = rows;
    
}



