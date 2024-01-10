#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double matrix_rand_interval(double min, double max) {
    double d = (double) rand() / ((double) RAND_MAX + 1);
    return min + d * (max - min);
}


matrix *matrix_new(unsigned int num_rows, unsigned int num_cols, size_t element_size) {

  matrix *mat = malloc(sizeof(matrix));
  if (!mat) {
    return NULL;
  }

  mat->num_rows = num_rows;
  mat->num_cols = num_cols;
  mat->is_square = (num_rows == num_cols);

  mat->data = malloc(num_rows * num_cols * element_size);

  return mat;
}

matrix *matrix_rand(unsigned int num_rows, 
                    unsigned int num_cols, 
                    double min, double max, 
                    size_t element_size) {

    matrix *r = matrix_new(num_rows, num_cols, element_size); // Allocate new matrix
    if (!r) {
      return NULL;
    }

    double *data = (double *)r->data; // Cast to double* for direct access
    for (int i = 0; i < (int)num_rows; i++) {
        for (int j = 0; j < (int)num_cols; j++) {
            data[i * num_cols + j] = matrix_rand_interval(min, max);
        }
    }
    return r;
}


matrix *matrix_sqr(unsigned int size, size_t element_size) {
  matrix *r = matrix_new(size, size, element_size);
  if (!r) {
    return NULL;
  }

  memset(r->data, 0, size * size * element_size);
  return r;
}

matrix *matrix_eye(unsigned int size, size_t element_size, const void *identity_element) {
    matrix *r = matrix_new(size, size, element_size);
    if (!r) {
        return NULL;
    }

    // Initialize all elements to 0
    memset(r->data, 0, size * size * element_size);

    // Set diagonal elements to the identity element
    for (unsigned int i = 0; i < size; i++) {
        memcpy((char*)r->data + (i * size + i) * element_size, identity_element, element_size);
    }

    return r;
}


void matrix_free(matrix *mat) {
  if (!mat) {
    return;
  }

  free(mat->data); 
  free(mat);
}

void matrix_print(const matrix *matrix) {
  matrix_printf(matrix, "%lf\t"); 
}

void matrix_printf(const matrix *matrix, const char *d_fmt) {

  int i, j;
  fprintf(stdout, "\n");
  
  for(i = 0; i <(int)(matrix->num_rows); ++i) {
    for(j = 0; j < (int)(matrix->num_cols); ++j) {
      double value = ((double *)(matrix->data))[i * matrix->num_cols + j];
      fprintf(stdout, d_fmt, value); 
    }
    fprintf(stdout, "\n");
  }
  
  fprintf(stdout, "\n");
}


int matrix_eqdim(const matrix *m1, const matrix *m2) {
    return (m1->num_cols == m2->num_cols) && (m1->num_rows == m2->num_rows);
}

int matrix_eq(const matrix *m1, const matrix *m2, double tolerance) {
    if (!matrix_eqdim(m1, m2)) {
        printf("Matrices are not the same dimension!\n"); 
        return 0;
    }

    for (unsigned int i = 0; i < m1->num_rows; i++) {
        for (unsigned int j = 0; j < m1->num_cols; j++) {
            size_t element_size = sizeof(double); // Assuming elements are type double
            size_t index = i * m1->num_cols * element_size + j * element_size;
            double *elem1 = (double*)((char*)m1->data + index);
            double *elem2 = (double*)((char*)m2->data + index);
            
            if (fabs(*elem1 - *elem2) > tolerance) {
                printf("Mismatch in row %u and column %u\n", i, j); // Use %u for unsigned integers
                return 0;
            }
        }
    }

    return 1; // If we reach here, matrices are equal within the specified tolerance
}
