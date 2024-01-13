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

matrix *matrix_col_get(const matrix *mat, unsigned int col_num) {
    if (col_num >= mat->num_cols) {
        return NULL; // Column number is out of bounds
    }

    matrix *col_matrix = matrix_new(mat->num_rows, 1, sizeof(double));
    if (!col_matrix) {
        return NULL; // Failed to allocate new matrix
    }

    for (unsigned int i = 0; i < mat->num_rows; ++i) {
        double *src = (double *)mat->data + i * mat->num_cols + col_num;
        double *dest = (double *)col_matrix->data + i;
        *dest = *src;
    }

    return col_matrix;
}

matrix *matrix_row_get(const matrix *mat, unsigned int row_num) {
    if (row_num >= mat->num_rows) {
        return NULL; // Row number is out of bounds
    }

    matrix *row_matrix = matrix_new(1, mat->num_cols, sizeof(double));
    if (!row_matrix) {
        return NULL; // Failed to allocate new matrix
    }

    double *src = (double *)mat->data + row_num * mat->num_cols;
    memcpy(row_matrix->data, src, mat->num_cols * sizeof(double));

    return row_matrix;
}

double matrix_at(const matrix *mat, unsigned int i, unsigned int j) {
    if (i >= mat->num_rows || j >= mat->num_cols) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return ((double*)mat->data)[i * mat->num_cols + j];
}

matrix *matrix_slice(const matrix *mat, Range row_range, Range col_range) {
    // Check if all rows are needed
    if (row_range.start == -1) {
        if (col_range.start == -1) { // All rows, all columns
            return mat; // For now will return just mat itself. May make a copy and return in future.
        } else { 
          // All other cases of single column or range of columns will just be a matrix_submatrix call
            return matrix_submatrix(mat, row_range, col_range);
        }
    }
    printf("matrix slice not successfully executed.");
    return NULL;   
}

matrix *matrix_submatrix(const matrix *mat, Range row_range, Range col_range) {
     // Check for negative indices for start
    if (row_range.start < 0 || col_range.start < 0) {
        fprintf(stderr, "Negative start index in range\n");
        return NULL;
    }

    // Check for negative indices for end, not including -1
    if ((row_range.end < 0 && row_range.end != -1) || 
        (col_range.end < 0 && col_range.end != -1)) {
        fprintf(stderr, "Invalid end index in range\n");
        return NULL;
    }

    // Determine the end indices, handling the -1 case
    unsigned int row_end = (row_range.end == -1) ? mat->num_rows : (unsigned int)row_range.end;
    unsigned int col_end = (col_range.end == -1) ? mat->num_cols : (unsigned int)col_range.end;

    // Check if end indices exceed matrix dimensions
    if (row_end > mat->num_rows || col_end > mat->num_cols) {
        fprintf(stderr, "Range end exceeds matrix dimensions\n");
        return NULL;
    }

    // Calculate new dimensions
    unsigned int new_rows = row_end - row_range.start;
    unsigned int new_cols = col_end - col_range.start;    


    matrix *submat = matrix_new(new_rows, new_cols, sizeof(double));
    if (!submat) {
        fprintf(stderr, "Failed to allocate memory for submatrix\n");
        return NULL;
    }

    // Copy data from the original matrix to the submatrix
    for (unsigned int i = 0; i < new_rows; ++i) {
        for (unsigned int j = 0; j < new_cols; ++j) {
            ((double*)submat->data)[i * new_cols + j] = 
                ((double*)mat->data)[(row_range.start + i) * mat->num_cols + (col_range.start + j)];
        }
    }

    return submat;
}

void matrix_all_set(matrix *mat, const void *value, size_t value_size) {
    for (unsigned int i = 0; i < mat->num_rows; ++i) {
        for (unsigned int j = 0; j < mat->num_cols; ++j) {
            unsigned int index = i * mat->num_cols + j;
            memcpy((char *)mat->data + index * value_size, value, value_size);
        }
    }
}

void matrix_diag_set(matrix *mat, const void *value, size_t value_size) {
    unsigned int min_dim = mat->num_rows < mat->num_cols ? mat->num_rows : mat->num_cols;
    for (unsigned int i = 0; i < min_dim; ++i) {
        unsigned int index = i * mat->num_cols + i;
        memcpy((char *)mat->data + index * value_size, value, value_size);
    }
}



