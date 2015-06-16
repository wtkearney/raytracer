//
//  matrix.h
//  
//
//  Created by Hunter Standen on 10/6/14.
//
//

#ifndef _matrix_h
#define _matrix_h

typedef struct {
    double m[4][4];
} Matrix;


void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);

//2D and Generic Matrix Functions
void matrix_print(Matrix *m, FILE *fp);
void matrix_clear(Matrix *m);
void matrix_identity(Matrix *m);
double matrix_get(Matrix *m, int r, int c);
void matrix_set(Matrix *m, int r, int c, double v);
void matrix_copy(Matrix *dest, Matrix *src);
void matrix_transpose(Matrix *m);
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m);
void matrix_xformPoint(Matrix *m, Point *p, Point *q);
void matrix_xformVector(Matrix *m, Vector *p, Vector *q);
void matrix_scale2D(Matrix *m, double sx, double sy);
void matrix_rotateZ(Matrix *m, double cth, double sth);
void matrix_translate2D(Matrix *m, double tx, double ty);
void matrix_shear2D(Matrix *m, double shx, double shy);

//3D matrix function
void matrix_translate(Matrix *m, double tx, double ty, double tz);
void matrix_scale(Matrix *m, double tx, double ty, double tz);
void matrix_rotateX(Matrix *m, double cth, double sth);
void matrix_rotateY(Matrix *m, double cth, double sth);
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w);
void matrix_shearZ(Matrix *m, double shx, double shy);
void matrix_perspective(Matrix *m, double d);


#endif
