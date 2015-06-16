//
//  ray.c
//  
//
//  Created by Will Kearney on 12/11/14.
//
//
// References:
//
// Moller, Tomas and Ben Trumbore. "Fast, Minimum Storage Ray/Triangle Intersection." Journal of Graphics Tools 2.1 (1997): 21-28. Online. http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
//
//
//
//

#include "graphics.h"
#define EPSILON 0.01
#define CUTOFF 5

void ray_set( Ray *ray, Point *origin, Point *end ) {
    point_copy( &(ray->origin), (origin));
    point_copy( &(ray->end), (end));
    
    // calculate vector and set in ray
    vector_set(&(ray->direction), (end->val[0] - origin->val[0]), (end->val[1] - origin->val[1]), (end->val[2] - origin->val[2]));
}


/* Copies ray */
void ray_copy( Ray *to, Ray *from ) {
    vector_copy( &(to->direction), &(from->direction));
    point_copy( &(to->origin), &(from->origin));
    point_copy( &(to->end), &(from->end));
}

void ray_trace(Image *src, Primitive *pList, int numP, Matrix *VTM, Matrix *GTM, Lighting *lighting, View3D *view) {
    Ray ray;
    Color clr;
    Point eye, pLocation;
    float COPx, COPy, COPz;

    // set center of projection (or "eye")
    vector_normalize(&view->vpn);
    COPx = view->vrp.val[0] - (view->vpn.val[0]*view->d);
    COPy = view->vrp.val[1] - (view->vpn.val[1]*view->d);
    COPz = view->vrp.val[2] - (view->vpn.val[2]*view->d);
    point_set3D(&eye, COPx, COPy, COPz);
    
//    printf("eye: \n");
//    vector_print(&eye, stdout);
    
    Vector u;
    Vector vup;
    Vector vpn;
    
    vector_cross(&view->vup, &view->vpn, &u);
    
    vector_cross(&view->vpn, &u, &vup);
    
    vector_copy(&vpn, &view->vpn);
    
//    vector_print(&vup, stdout);
//    vector_print(&u, stdout);
    
    // calculate how to step across view plane
    double stepX = (view->du) / (double)(src->cols);
    double stepY = (view->dv) / (double)(src->rows);
    
    double VPx = 0.0;
    double VPy = 0.0;
    double VPz = 0.0;
    
    
    int col = 0;
    for (double x=0;x<view->du;x=x+stepX) {
        int row = 0;
        for (double y=0;y<view->dv;y=y+stepY) {
            
            VPx = view->vrp.val[0] -
                    ( x - (view->du/2) ) * u.val[0] -
                    ( y - (view->dv/2) ) * -vup.val[0];
            
            VPy = view->vrp.val[1] +
                    ( x - (view->du/2) ) * u.val[1] +
                    ( y - (view->dv/2) ) * -vup.val[1];
            
            VPz = view->vrp.val[2] +
                ( x - (view->du/2) ) * u.val[2] +
                ( y - (view->dv/2) ) * -vup.val[2];
            
            //printf("VP %d, %d: (%lf, %lf, %lf)\n", col, row, VPx, VPy, VPz);
            
            point_set3D(&pLocation, VPx, VPy, VPz);
            
            // Calculate the ray vi,j from the eye through pixel (i, j)
            ray_set(&ray, &eye, &pLocation );
            vector_normalize( &(ray.direction) );
            
            // Color the pixel with the result of calling RayIntersect
            clr = rayIntersect(&ray, pList, numP, lighting, 0);
            image_setColor(src, row, col, clr);
            
            row = row + 1;
        }
        col = col + 1;
        
    }

}


Color rayIntersect(Ray *ray, Primitive *plist, int numP, Lighting *lighting, int level) {
    
    if (level == CUTOFF) return (Color){{0.0, 0.0, 0.0, 0.0}};
    
    //Color clr = (Color){{0.0, 0.0, 0.0, 1.0}};
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
    float curMinDist = 999999999;
    int curMinIdx = -1;
    Point intersection;
    intersection.val[0] = 0;
    intersection.val[1] = 0;
    intersection.val[2] = 0;
    double cx, cy, cz, R, x0, y0, z0, dx, dy, dz, aa, bb, cc, discriminant;
    int shadow;
    float t0, t1;

    
    /* Intersect the ray with the primitives in the scene, identifying the closest intersection */
    for (int i=0;i<numP;i++) {
        
        switch ( (plist[i]).type ) {
            case SphereType : {
                //printf("SphereType\n");
                // do sphere intersection
                
                
                // localize sphere info
                cx = (plist[i]).center.val[0];
                cy = (plist[i]).center.val[1];
                cz = (plist[i]).center.val[2];
                R = (plist[i]).radius;
                
                // localize ray info
                x0 = ray->origin.val[0];
                y0 = ray->origin.val[1];
                z0 = ray->origin.val[2];
                dx = ray->direction.val[0];
                dy = ray->direction.val[1];
                dz = ray->direction.val[2];
                
                // do maths
                aa = dx*dx + dy*dy + dz*dz;
                bb = 2*dx*(x0-cx) +  2*dy*(y0-cy) +  2*dz*(z0-cz);
                cc = cx*cx + cy*cy + cz*cz + x0*x0 + y0*y0 + z0*z0 + -2*(cx*x0 + cy*y0 + cz*z0) - R*R;
                discriminant = bb*bb - 4*aa*cc;
                
                if (discriminant <= 0) {
                    break; // no intersection, or ray is tangent to sphere
                } else {
                    
                    t0 = (-bb + sqrt(discriminant)) / (2*aa);
                    t1 = (-bb - sqrt(discriminant)) / (2*aa);
                    
                    // set new minimum distance, if needed
                    if (t0 < t1 && t0 < curMinDist) {
                        intersection.val[0] = x0 + (double)t0*dx;
                        intersection.val[1] = y0 + (double)t0*dy;
                        intersection.val[2] = z0 + (double)t0*dz;
                        curMinDist = t0;
                        curMinIdx = i;
                    } else if (t1 < t0 && t1 < curMinDist) {
                        intersection.val[0] = x0 + (double)t1*dx;
                        intersection.val[1] = y0 + (double)t1*dy;
                        intersection.val[2] = z0 + (double)t1*dz;
                        curMinDist = t1;
                        curMinIdx = i;
                    }

//                    printf("CurMinDist: %f\n", curMinDist);
//                    printf("CurMinIdx: %d\n\n", curMinIdx);
                    
                    //printf("Color: (%f, %f, %f)\n", (plist[curMinIdx]).color.c[0], (plist[curMinIdx]).color.c[1], (plist[curMinIdx]).color.c[2]);
                    
                    
                }
                break;
            } case PlaneType : {
                // do polygon intersection
                break;
            }
            
        }

        
        
    }
    if (curMinIdx < 0) {
        return (Color){{r, g, b, 1.0}};
    }
    // Find unit normal vector to the sphere
    Vector normal;
    normal.val[0] = (intersection.val[0] - plist[curMinIdx].center.val[0]) / plist[curMinIdx].radius;
    normal.val[1] = (intersection.val[1] - plist[curMinIdx].center.val[1]) / plist[curMinIdx].radius;
    normal.val[2] = (intersection.val[2] - plist[curMinIdx].center.val[2]) / plist[curMinIdx].radius;
    
    vector_normalize(&normal);
    
    // For each light source Li
    for (int lightIdx=0;lightIdx<lighting->nLights;lightIdx++) {
        
        // ambient lighting
        if (lighting->light[lightIdx].type == LightAmbient) {
            r = r + lighting->light[lightIdx].color.c[0] * plist[curMinIdx].color.c[0];  // r
            g = g + lighting->light[lightIdx].color.c[1] * plist[curMinIdx].color.c[1];  // g
            b = b + lighting->light[lightIdx].color.c[2] * plist[curMinIdx].color.c[2];  // b
            continue;
        }
        
        // Get unit vector to light
        Vector L;
        vector_set(&L, lighting->light[lightIdx].position.val[0] - intersection.val[0],
                   lighting->light[lightIdx].position.val[1] - intersection.val[1],
                   lighting->light[lightIdx].position.val[2] - intersection.val[2]);
        
        vector_normalize( &L);
        
        
        /* Shadow attempt; image keeps coming up with only ambient light. */
        
        // check if location in shadow by intersecting light ray with all other primitives in scene
//        shadow = 0;
//        for (int j=0;j<numP;j++) {
//            switch ( (plist[j]).type ) {
//                case SphereType : {
//                    // do sphere intersection
//                    
//                    // localize sphere info
//                    cx = (plist[j]).center.val[0];
//                    cy = (plist[j]).center.val[1];
//                    cz = (plist[j]).center.val[2];
//                    R = (plist[j]).radius;
//                    
//                    // localize ray info
//                    x0 = intersection.val[0] + EPSILON*L.val[0];
//                    y0 = intersection.val[1] + EPSILON*L.val[1];
//                    z0 = intersection.val[2] + EPSILON*L.val[2];
//                    dx = L.val[0];
//                    dy = L.val[1];
//                    dz = L.val[2];
//                    
//                    // do maths
//                    aa = dx*dx + dy*dy + dz*dz;
//                    bb = 2*dx*(x0-cx) +  2*dy*(y0-cy) +  2*dz*(z0-cz);
//                    cc = cx*cx + cy*cy + cz*cz + x0*x0 + y0*y0 + z0*z0 + -2*(cx*x0 + cy*y0 + cz*z0) - R*R;
//                    discriminant = bb*bb - 4*aa*cc;
//                    
//                    if (discriminant > 0) {
//                        shadow = 1; // location is in shadow from this light source
//                        break;
//                    }
//                } case PlaneType : {
//                    // do polygon intersection
//                    break;
//                }
//            }
//        }
        
        
        // compute cos of the angle between N and L
        double factor = vector_dot(&normal, &L);
        if (factor > 0) {
            // diffuse term depends on the color of light, diffuse intensity, and direction of light (factor)
            r = r + lighting->light[lightIdx].color.c[0] * plist[curMinIdx].diffuse * factor;
            g = g + lighting->light[lightIdx].color.c[1] * plist[curMinIdx].diffuse * factor;
            b = b + lighting->light[lightIdx].color.c[2] * plist[curMinIdx].diffuse * factor;
            

            // intensity = diffuse * (L.N) + specular * (V.R)n
            // specular term depends on view vector, reflection in surface, and highlight exponent
            
            // L reflected in the surface
            Vector reflect;
            reflect.val[0] = L.val[0] - 2.0f * factor * normal.val[0];
            reflect.val[1] = L.val[1] - 2.0f * factor * normal.val[1];
            reflect.val[2] = L.val[2] - 2.0f * factor * normal.val[2];
            reflect.val[3] = 0;
            
            double dot = vector_dot( &reflect, &ray->direction);
            
            r = r + lighting->light[lightIdx].color.c[0] * plist[curMinIdx].specular * pow(dot, 20);
            g = g + lighting->light[lightIdx].color.c[1] * plist[curMinIdx].specular * pow(dot, 20);
            b = b + lighting->light[lightIdx].color.c[2] * plist[curMinIdx].specular * pow(dot, 20);
            
        }
        
    }
    
    Ray reflectionRay;
    Point reflectionDest;
    
    double dDotN = vector_dot(&ray->direction, &normal);
    
    Vector reflect;
    reflect.val[0] = ray->direction.val[0] - 2.0f * dDotN * normal.val[0];
    reflect.val[1] = ray->direction.val[1] - 2.0f * dDotN * normal.val[1];
    reflect.val[2] = ray->direction.val[2] - 2.0f * dDotN * normal.val[2];
    reflect.val[3] = 0;
    
    vector_normalize( &reflect);
    
    point_set3D(&reflectionDest, intersection.val[0] - reflect.val[0]
                , intersection.val[1] - reflect.val[1]
                , intersection.val[2] - reflect.val[2]);

    ray_set(&reflectionRay, &intersection, &reflectionDest);
    
    vector_normalize( &reflectionRay.direction);
    
    Color reflection = rayIntersect(&reflectionRay, plist, numP, lighting, level+1);
    
    
    r = r + plist[curMinIdx].reflection * reflection.c[0];
    g = g + plist[curMinIdx].reflection * reflection.c[1];
    b = b + plist[curMinIdx].reflection * reflection.c[2];
    
    return (Color){{r, g, b, 1.0}};

}




/* Given a start point and end point, calculates if the ray between them passes through a polygon in plist.
 Returns 0 if the ray does hit a polygon, 1 otherwise */
//int ray_shadow(Point *start, Point *end, Polygon *plist, int numP) {
//    
//    Ray ray;
//    ray_set( &ray, start, end );
//    
//    vector_normalize(&(ray.direction));
//    
//    /* for each polygon in plist */
//    for (int i=0;i<numP;i++) {
//     
//        /* find vectors for two edges sharing vertex 1 */
//        Vector edge1, edge2;
//        vector_set(&edge1, plist[i].vertex[0].val[0] - plist[i].vertex[1].val[0], plist[i].vertex[0].val[1] - plist[i].vertex[1].val[1], plist[i].vertex[0].val[2] - plist[i].vertex[1].val[2]);
//        
//        vector_set(&edge2, plist[i].vertex[2].val[0] - plist[i].vertex[1].val[0], plist[i].vertex[2].val[1] - plist[i].vertex[1].val[1], plist[i].vertex[2].val[2] - plist[i].vertex[1].val[2]);
//        
//        
//        /* check to see if start point is in polygon. In that case, don't check */
//        Vector normal;
//        vector_cross(&edge1, &edge2, &normal);
//        
//        /* calculate distance from vertex 1 to ray origin */
//        Vector tvec;
//        vector_set(&tvec, ray.origin.val[0] - plist[i].vertex[1].val[0], ray.origin.val[1] - plist[i].vertex[1].val[1], ray.origin.val[2] - plist[i].vertex[1].val[2]);
//        
//        float dot = vector_dot(&normal, &tvec);
//        if (dot > -EPSILON && dot < EPSILON)
//            continue;
//        
//        /* begin calculating determinant - also used to calculate U parameter */
//        Vector pvec;
//        vector_cross(&(ray.direction), &edge2, &pvec);
//        
//        /* if determinant is near zero, ray lies in plane of triangle */
//        float det = vector_dot(&edge1, &pvec);
//        
//        /* culling algorithm follows */
//        if (det > -EPSILON && det < EPSILON)
//            continue;
//
//        float inv_det = 1.0 / det;
//        
//        /* calculate U parameter and test bounds */
//        float u = vector_dot(&tvec, &pvec) * inv_det;
//        if (u < 0.0 || u > 1.0)
//            continue;
//        
//        /* prepare to test V parameter */
//        Vector qvec;
//        vector_cross(&tvec, &edge1, &qvec);
//        
//        /* calculate V paramter and test bounds */
//        float v = vector_dot(&(ray.direction), &qvec) * inv_det;
//        if (v < 0.0 || u + v > 1.0) {
//            continue;
//        }
//        
//        /* check if polygon is actually in between light and point */
//        float t = vector_dot(&edge2, &qvec) * inv_det;
//        if (t < EPSILON)
//            continue;
//        
//        /* if polygon is a square, check other triangle */
//        if (plist[i].nVertex == 4) {
//        
//            /* find vectors for two edges sharing vertex 3 */
//            Vector edge1, edge2;
//            vector_set(&edge1, plist[i].vertex[0].val[0] - plist[i].vertex[3].val[0], plist[i].vertex[0].val[1] - plist[i].vertex[3].val[1], plist[i].vertex[0].val[2] - plist[i].vertex[3].val[2]);
//            
//            vector_set(&edge2, plist[i].vertex[2].val[0] - plist[i].vertex[3].val[0], plist[i].vertex[2].val[1] - plist[i].vertex[3].val[1], plist[i].vertex[2].val[2] - plist[i].vertex[3].val[2]);
//            
//            /* check to see if start point is in polygon. In that case, don't check */
//            Vector normal;
//            vector_cross(&edge1, &edge2, &normal);
//            
//            /* calculate distance from vertex 1 to ray origin */
//            Vector tvec;
//            vector_set(&tvec, ray.origin.val[0] - plist[i].vertex[1].val[0], ray.origin.val[1] - plist[i].vertex[1].val[1], ray.origin.val[2] - plist[i].vertex[1].val[2]);
//            
//            float dot = vector_dot(&normal, &tvec);
//            if (dot > -EPSILON && dot < EPSILON)
//                continue;
//            
//            /* begin calculating determinant - also used to calculate U parameter */
//            Vector pvec;
//            vector_cross(&(ray.direction), &edge2, &pvec);
//            
//            /* if determinant is near zero, ray lies in plane of triangle */
//            float det = vector_dot(&edge1, &pvec);
//            
//            /* culling algorithm follows */
//            if (det > -EPSILON && det < EPSILON)
//                continue;
//            
//            float inv_det = 1.0 / det;
//            
//            /* calculate U parameter and test bounds */
//            float u = vector_dot(&tvec, &pvec) * inv_det;
//            if (u < 0.0 || u > 1.0)
//                continue;
//            
//            /* prepare to test V parameter */
//            Vector qvec;
//            vector_cross(&tvec, &edge1, &qvec);
//            
//            /* calculate V paramter and test bounds */
//            float v = vector_dot(&(ray.direction), &qvec) * inv_det;
//            if (v < 0.0 || u + v > 1.0) {
//                continue;
//            }
//            
//            /* check if polygon is actually in between light and point */
//            float t = vector_dot(&edge2, &qvec) * inv_det;
//            if (t < EPSILON)
//                continue;
//        
//        }
//        
//        /* ray intersects triangle */
//        return 0;
//        
//    }
//    
//    return 1;
//    
//}




