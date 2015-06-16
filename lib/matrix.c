//  matrix.c
//  
//
//  Created by Hunter Standen and Will Kearney on 10/6/14.
//
//
// edited 10/16/14

#include "graphics.h"

/* Print out the matrix in a nice 4x4 arrangement with a blank line below */
void matrix_print(Matrix *m, FILE *fp) {
  fprintf( fp, "[");
  for(int i=0;i<4;i++) {
      fprintf( fp, "[");
      for(int j=0;j<4;j++) {
	fprintf( fp, "%lf ", m->m[i][j] );
      }
      fprintf( fp, "]\n");
    }
  fprintf( fp, "]\n" );
}

/* set the matrix to all zeros */
void matrix_clear(Matrix *m) {
  for(int i=0;i<4;i++) {
      for(int j=0;j<4;j++) {
	  m->m[i][j] = 0;
	}
    }
}

/* set the matrix to an identity matrix */
void matrix_identity(Matrix *m) {
    for(int i=0;i<4;i++) {
      for(int j=0;j<4;j++) {
	  if (i == j) {
	    m->m[i][j] = 1;
	  } else {
	    m->m[i][j] = 0;
	  }
      }
    }
}


/* return the element of the matrix at row r, column c */
double matrix_get(Matrix *m, int r, int c) {
  return m->m[r][c];
}

/* set the element of the matrix at row r, column c to v */
void matrix_set(Matrix *m, int r, int c, double v) {
  m->m[r][c] = v;
}

/* copy the src matrix into the dest matrix */
void matrix_copy(Matrix *dest, Matrix *src) {
  for(int i=0;i<4;i++) {
      for(int j=0;j<4;j++) {
	  dest->m[i][j] = src->m[i][j];
      }
  }
}

/* transpose the matrix m in place */
void matrix_transpose(Matrix *m) {
  int element1;
  int element2;
  for(int i=0;i<4;i++) {
    for(int j=0;j<4;j++) {
	  if (j > i) {
	    element1 = m->m[i][j];
	    element2 = m->m[j][i];
	    m->m[i][j] = element2;
	    m->m[j][i] = element1;
	  }
	}
    }
}

/* multiply left and right and put the result in m */
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m) {
  Matrix temp;
  for(int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      temp.m[i][j] = left->m[i][0] * right->m[0][j]
	+ left->m[i][1] * right->m[1][j]
	+ left->m[i][2] * right->m[2][j]
	+ left->m[i][3] * right->m[3][j];
    }
  }
  matrix_copy(m, &temp);
}

/* transform the point p by the matrix m and put the result in q */
void matrix_xformPoint(Matrix *m, Point *p, Point *q) {
  for(int i=0;i<4;i++) {

    q->val[i] = (m->m[i][0] * p->val[0])
      + (m->m[i][1] * p->val[1])
      + (m->m[i][2] * p->val[2])
      + (m->m[i][3] * p->val[3]);
 
  }
}

/* transform the point p by the matrix m and put the result in q */
void matrix_xformVector(Matrix *m, Vector *p, Vector *q) {
  for(int i=0;i<3;i++) {

    q->val[i] = (m->m[i][0] * p->val[0])
      + (m->m[i][1] * p->val[1])
      + (m->m[i][2] * p->val[2])
      + (m->m[i][3] * p->val[3]);

 
  }
    q->val[3] = 0.0;
}




/* Premultiply the matrix by a scale matrix parameterized by sx and sy */
void matrix_scale2D(Matrix *m, double sx, double sy) {
  for(int i=0;i<4;i++) {
    m->m[0][i] = m->m[0][i] * sx;
    m->m[1][i] = m->m[1][i] * sy;
  }
}

/* premultiply the matrix by a z-axis rotation matrix parameterized by cos(theta) and sin(theta), where theta is the angle of rotation about the z-axis */
void matrix_rotateZ(Matrix *m, double cth, double sth) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,3,3, 1.0);
    matrix_set(&T,2,2, 1.0);
    matrix_set(&T,0,0, cth);
    matrix_set(&T,0,1, -sth);
    matrix_set(&T,1,0, sth);
    matrix_set(&T,1,1, cth);
    matrix_multiply(&T, m, m);
}

/* Premultiply the matrix by a 2D translation matrix parameterized by tx and ty */
void matrix_translate2D(Matrix *m, double tx, double ty) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,0,3, tx);
    matrix_set(&T,1,3, ty);
    matrix_multiply(&T, m, m);
}

/* Premultiply the matrix by a 2D shear matrix parameterized by shx and shy */
void matrix_shear2D(Matrix *m, double shx, double shy) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,0,1,shx);
    matrix_set(&T,1,0,shy);
    matrix_multiply(&T, m, m);
}





//vector functions
//set the vector to x, y, and z
void vector_set(Vector *v, double x, double y, double z) {
    v->val[0]=x;
    v->val[1]=y;
    v->val[2]=z;
    v->val[3]=0.0;
}

//print out the Vector to stream fp in a pretty form
void vector_print(Vector *v, FILE *fp) {
    fprintf(fp, "(%f, %f, %f, %f)\n",v->val[0], v->val[1], v->val[2], v->val[3]);
}

//copy the src Vector into the dest Vector
void vector_copy(Vector *dest, Vector *src) {
    for(int i=0;i<4;i++) {
        dest->val[i]=src->val[i];
    }
}

//Returns the Euclidean distance of the vector assuming the homogenous
//coordinate is 1.0
double vector_length(Vector *v) {
    return sqrt(v->val[0]*v->val[0]+v->val[1]*v->val[1]+v->val[2]*v->val[2]);
}

//Normalize the Vector to Unit Length. Do not modify the homogenous coordinate
void vector_normalize(Vector *v) {
    double l = vector_length(v);
    for(int i=0; i<3; i++) {
        v->val[i]=v->val[i]/l;
    }
}

//Returns the scalar product of a and b
double vector_dot(Vector *a, Vector *b) {
    return a->val[0]*b->val[0]+a->val[1]*b->val[1]+a->val[2]*b->val[2];
}

//calculates the cross product of a and b and puts the result in c
void vector_cross(Vector *a, Vector *b, Vector *c) {
    c->val[0]=a->val[1]*b->val[2]-a->val[2]*b->val[1];
    c->val[1]=a->val[2]*b->val[0]-a->val[0]*b->val[2];
    c->val[2]=a->val[0]*b->val[1]-a->val[1]*b->val[0];
    c->val[3]=0.0;
}

//2D Matrix Functions


//3D Matrix Functions
//premultiplies the matrix by a translation matrix parameterized tx ty and tz
void matrix_translate(Matrix *m, double tx, double ty, double tz) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,0,3, tx);
    matrix_set(&T,1,3, ty);
    matrix_set(&T,2,3, tz);
    matrix_multiply(&T, m, m);
}

//premultipleis the matrix by a scale matrix parameterized by sx sy and sz
void matrix_scale(Matrix *m, double sx, double sy, double sz) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,0,0, sx);
    matrix_set(&T,1,1, sy);
    matrix_set(&T,2,2, sz);
    matrix_set(&T,3,3, 1.0);
    matrix_multiply(&T, m, m);
}

//premultiplies the matrix by a x-axis rotation matrix parameterized by cos0 and
//sin0 where 0 is the angle of rotation about the X-axis
void matrix_rotateX(Matrix *m, double cth, double sth) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,3,3, 1.0);
    matrix_set(&T,0,0, 1.0);
    matrix_set(&T,1,1, cth);
    matrix_set(&T,1,2, -sth);
    matrix_set(&T,2,1, sth);
    matrix_set(&T,2,2, cth);
    matrix_multiply(&T, m, m);
}

//premultuplies the matrix by a Y-axis rotation matrix parameterized by cos0
//and sin0 where 0 is the angle of rotation about the Y-axis
void matrix_rotateY(Matrix *m, double cth, double sth) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,1,1, 1.0);
    matrix_set(&T,3,3, 1.0);
    matrix_set(&T,0,0, cth);
    matrix_set(&T,2,0, -sth);
    matrix_set(&T,0,2, sth);
    matrix_set(&T,2,2, cth);
    matrix_multiply(&T, m, m);
    //matrix_print(&T, stdout);
}

//premultiples the matrix by an XYZ-axis rotation matrix parametereized by the
//vectors u, v, and w, where the three vectors represent an othonormal 3D basis
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w) {
    Matrix T;
    matrix_identity(&T);
    for(int i=0;i<3;i++) {
        printf("U->val[%d]: %f\n", i, u->val[i]);
        matrix_set(&T,0,i,u->val[i]);
        matrix_set(&T,1,i,v->val[i]);
        matrix_set(&T,2,i,w->val[i]);
        matrix_set(&T,3,i,0.0);
    }
    matrix_set(&T,3,3,1.0);
    printf("T: \n");
    matrix_print(&T, stdout);
    matrix_multiply(&T, m, m);
}

//premultiply the matrix by a shear Z matrix parameterized by shx and shy
void matrix_shearZ(Matrix *m, double shx, double shy) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,0,2,shx);
    matrix_set(&T,1,2,shy);
    matrix_multiply(&T, m, m);
}

//premultiplies the matrix by a perspective matrix parameterized by d
void matrix_perspective(Matrix *m, double d) {
    Matrix T;
    matrix_identity(&T);
    matrix_set(&T,3,3,0.0);
    matrix_set(&T,3,2,(1/d));
    matrix_multiply(&T, m, m);
}








