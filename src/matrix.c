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

  matrix *mat = calloc(1, sizeof(matrix));
  if (!mat) {
    return NULL;
  }

  mat->num_rows = num_rows;
  mat->num_cols = num_cols;
  mat->is_square = (num_rows == num_cols);

  mat->data = calloc(num_rows * num_cols, element_size);
  if (!mat->data) {
    free(mat);
    return NULL;
  }

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

double matrix_at(const matrix *mat, unsigned int i, unsigned int j) {
    if (i >= mat->num_rows || j >= mat->num_cols) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return ((double*)mat->data)[i * mat->num_cols + j];
}

matrix *matrix_slice(matrix *mat, Range row_range, Range col_range) {
    // Check if all rows are needed
    if (row_range.start == -1) {
        if (col_range.start == -1) { // All rows, all columns
            return matrix_copy(mat);
        } else {
            return matrix_submatrix(mat, row_range, col_range);
        }
    } else { 
      // All other cases of single column or range of columns will just be a matrix_submatrix call
            return matrix_submatrix(mat, row_range, col_range);
    }
    
    printf("matrix slice not successfully executed.");
    return NULL;   
}

matrix *matrix_submatrix(const matrix *mat, Range row_range, Range col_range) {
     // Check for negative indices for start
    if (row_range.start < -1 || col_range.start < -1) {
        fprintf(stderr, "Invalid start index in range\n");
        return NULL;
    }

    // Check for negative indices for end, not including -1
    if ((row_range.end < -1) || (col_range.end < -1)) {
        fprintf(stderr, "Invalid end index in range\n");
        return NULL;
    }

    // Determine the start indices, handling the -1 case
    unsigned int row_start = (row_range.start == -1) ? 0 : row_range.start;
    unsigned int col_start = (col_range.start == -1) ? 0 : col_range.start;

    // Determine the end indices, handling the -1 case
    unsigned int row_end = (row_range.end == -1) ? (mat->num_rows) : (unsigned int)row_range.end;
    unsigned int col_end = (col_range.end == -1) ? (mat->num_cols) : (unsigned int)col_range.end;

    // Check if end indices exceed matrix dimensions
    if ((row_end - 1) > mat->num_rows || (col_end - 1) > mat->num_cols) {
        fprintf(stderr, "Range end exceeds matrix dimensions\n");
        return NULL;
    }

    // Calculate new dimensions
    unsigned int new_rows = (row_range.end == -1) ? row_end : row_end - row_start;
    unsigned int new_cols = (col_range.end == -1) ? col_end : col_end - col_start;    


    matrix *submat = matrix_new(new_rows, new_cols, sizeof(double));
    if (!submat) {
        fprintf(stderr, "Failed to allocate memory for submatrix\n");
        return NULL;
    }

    // Copy data from the original matrix to the submatrix
    for (unsigned int i = 0; i < new_rows; ++i) {
        for (unsigned int j = 0; j < new_cols; ++j) {
            ((double*)submat->data)[i * new_cols + j] = 
                ((double*)mat->data)[(row_start + i) * mat->num_cols + (col_start + j)];
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
    // Check if the matrix is square
    if (mat->num_rows != mat->num_cols) {
        fprintf(stderr, "Error: Matrix is not square.\n");
        return;
    }

    unsigned int min_dim = mat->num_rows;  // Since the matrix is square, num_rows == num_cols
    for (unsigned int i = 0; i < min_dim; ++i) {
        unsigned int index = i * mat->num_cols + i;
        memcpy((char *)mat->data + index * value_size, value, value_size);
    }
}

matrix *matrix_copy(const matrix *src) {
    if (src == NULL) {
        return NULL; // Handle null source matrix
    }

    // Manually allocate memory for the new matrix structure
    matrix *copy = malloc(sizeof(matrix));
    if (!copy) {
        return NULL; // Handle memory allocation failure
    }

    // Copy the matrix structure (excluding the data pointer)
    *copy = *src;

    // Allocate memory for the data array and copy the data
    copy->data = malloc(src->num_rows * src->num_cols * sizeof(double));
    if (!copy->data) {
        free(copy); // Clean up if data allocation fails
        return NULL;
    }
    memcpy(copy->data, src->data, src->num_rows * src->num_cols * sizeof(double));

    return copy;
}

void matrix_transpose(matrix *mat) {
    if (mat == NULL) {
        return; // Handle null matrix
    }

    // Create a new matrix to store the transpose
    matrix *transposed = matrix_new(mat->num_cols, mat->num_rows, sizeof(double));
    if (!transposed) {
        return; // Handle memory allocation failure
    }

    // Transpose the data
    for (unsigned int i = 0; i < mat->num_rows; ++i) {
        for (unsigned int j = 0; j < mat->num_cols; ++j) {
            ((double*)transposed->data)[j * mat->num_rows + i] = 
                ((double*)mat->data)[i * mat->num_cols + j];
        }
    }

    // Replace original matrix data with transposed data
    free(mat->data);
    mat->data = transposed->data;
    mat->num_rows = transposed->num_rows;
    mat->num_cols = transposed->num_cols;

    // Free the transposed matrix structure without freeing its data
    free(transposed);
}

matrix *matrix_stackv(const matrix *mat1, const matrix *mat2) {
    if (mat1 == NULL || mat2 == NULL) {
        return NULL; // Handle null matrices
    }

    // Check if both matrices have the same number of columns
    if (mat1->num_cols != mat2->num_cols) {
        fprintf(stderr, "Error: Matrices must have the same number of columns to stack.\n");
        return NULL;
    }

    // Create a new matrix to store the stacked matrices
    unsigned int new_rows = mat1->num_rows + mat2->num_rows;
    matrix *stacked = matrix_new(new_rows, mat1->num_cols, sizeof(double));
    if (!stacked) {
        return NULL; // Handle memory allocation failure
    }

    // Copy data from mat1 into the first part of stacked
    memcpy(stacked->data, mat1->data, mat1->num_rows * mat1->num_cols * sizeof(double));
    
    // Copy data from mat2 into the second part of stacked
    memcpy((char*)stacked->data + mat1->num_rows * mat1->num_cols * sizeof(double), 
           mat2->data, mat2->num_rows * mat2->num_cols * sizeof(double));

    return stacked;
}

matrix *matrix_stackh(const matrix *mat1, const matrix *mat2) {
    if (mat1 == NULL || mat2 == NULL) {
        return NULL; // Handle null matrices
    }

    // Check if both matrices have the same number of rows
    if (mat1->num_rows != mat2->num_rows) {
        fprintf(stderr, "Error: Matrices must have the same number of rows to stack horizontally.\n");
        return NULL;
    }

    // Create a new matrix to store the horizontally stacked matrices
    unsigned int new_cols = mat1->num_cols + mat2->num_cols;
    matrix *stacked = matrix_new(mat1->num_rows, new_cols, sizeof(double));
    if (!stacked) {
        return NULL; // Handle memory allocation failure
    }

    // Copy data from mat1 and mat2 into the stacked matrix
    for (unsigned int i = 0; i < mat1->num_rows; ++i) {
        // Copy data from mat1
        memcpy((char*)stacked->data + i * new_cols * sizeof(double),
            (char*)mat1->data + i * mat1->num_cols * sizeof(double),
            mat1->num_cols * sizeof(double));


        // Copy data from mat2
        memcpy((char*)stacked->data + (i * new_cols + mat1->num_cols) * sizeof(double), 
           (char*)mat2->data + i * mat2->num_cols * sizeof(double), 
           mat2->num_cols * sizeof(double));
    } 

    return stacked;

}

/*Matrix math operations*/

void matrix_row_mult_r(matrix *mat, unsigned int row, double value) {
    if (row >= mat->num_rows) {
        fprintf(stderr, "Row index out of bounds\n");
        return;
    }

    for (unsigned int j = 0; j < mat->num_cols; ++j) {
        ((double *)mat->data)[row * mat->num_cols + j] *= value;
    }
}

void matrix_col_mult_r(matrix *mat, unsigned int col, double value) {
    if (col >= mat->num_cols) {
        fprintf(stderr, "Column index out of bounds\n");
        return;
    }

    for (unsigned int i = 0; i < mat->num_rows; ++i) {
        ((double *)mat->data)[i * mat->num_cols + col] *= value;
    }
}

void matrix_mult_r(matrix *mat, double value) {
    for (unsigned int i = 0; i < mat->num_rows; ++i) {
        for (unsigned int j = 0; j < mat->num_cols; ++j) {
            ((double *)mat->data)[i * mat->num_cols + j] *= value;
        }
    }
}


void matrix_row_addrow(matrix *mat, unsigned int row1_index, unsigned int row2_index, unsigned int result_row_index) {
    if (mat == NULL) {
        return; // Handle null matrix
    }

    // Check if row indices are within the bounds of the matrix
    if (row1_index >= mat->num_rows || row2_index >= mat->num_rows) {
        fprintf(stderr, "Error: Your matrix 1 or matrix 2 Row index selection is out of bounds.\n");
        return;
    }

    if (result_row_index >= mat->num_rows) {
        fprintf(stderr, "Error: Your result row selection index is out of bounds.\n");
        return;
    }

    // Add rows and store the result
    for (unsigned int i = 0; i < mat->num_cols; ++i) {
        ((double*)mat->data)[result_row_index * mat->num_cols + i] = 
            ((double*)mat->data)[row1_index * mat->num_cols + i] + 
            ((double*)mat->data)[row2_index * mat->num_cols + i];
    }
}

matrix *matrix_row_rem(matrix *mat, unsigned int row) {
    if (row >= mat->num_rows) {
        fprintf(stderr, "Row index out of bounds\n");
        return mat;
    }

    unsigned int new_rows = mat->num_rows - 1;
    matrix *new_mat = matrix_new(new_rows, mat->num_cols, sizeof(double));
    if (!new_mat) {
        return mat; // Failed to allocate new matrix, return the original
    }

    double *new_data = (double *)new_mat->data;
    double *old_data = (double *)mat->data;

    for (unsigned int i = 0, new_i = 0; i < mat->num_rows; i++) {
        if (i == row) { 
            continue; // Skip the row to be removed
        }
        for (unsigned int j = 0; j < mat->num_cols; j++) {
            new_data[new_i * mat->num_cols + j] = old_data[i * mat->num_cols + j];
        }
        new_i++;
    }

    matrix_free(mat); // Free the old matrix
    return new_mat;   // Return the new matrix
}


matrix *matrix_col_rem(matrix *mat, unsigned int col) {
    if (col >= mat->num_cols) {
        fprintf(stderr, "Column index out of bounds\n");
        return mat;
    }

    unsigned int new_cols = mat->num_cols - 1;
    matrix *new_mat = matrix_new(mat->num_rows, new_cols, sizeof(double));
    if (!new_mat) {
        return mat; // Failed to allocate new matrix, return the original
    }

    double *new_data = (double *)new_mat->data;
    double *old_data = (double *)mat->data;

    for (unsigned int i = 0; i < mat->num_rows; i++) {
    	for (unsigned int j = 0, new_j = 0; j < mat->num_cols; j++) {
        	if (j == col) {
            	    continue; // Skip the column to be removed
        	}
        	new_data[i * new_cols + new_j] = old_data[i * mat->num_cols + j];
                new_j++;
    	}
    }

    matrix_free(mat); // Free the old matrix
    return new_mat;   // Return the new matrix
}

void matrix_swap_rows(matrix *mat, unsigned int row1, unsigned int row2) {
    if (!mat || row1 >= mat->num_rows || row2 >= mat->num_rows) {
        // Handle invalid input
        fprintf(stderr, "Invalid input for matrix row swap.\n");
        return;
    }

    double *data = (double *)mat->data;
    for (unsigned int i = 0; i < mat->num_cols; i++) {
        double temp = data[row1 * mat->num_cols + i];
        data[row1 * mat->num_cols + i] = data[row2 * mat->num_cols + i];
        data[row2 * mat->num_cols + i] = temp;
    }

}

void matrix_swap_cols(matrix *mat, unsigned int col1, unsigned int col2) {
    if (!mat || col1 >= mat->num_cols || col2 >= mat->num_cols) {
        // Handle invalid input
        fprintf(stderr, "Invalid input for matrix column swap.\n");
        return;
    }

    double *data = (double *)mat->data;
    for (unsigned int i = 0; i < mat->num_rows; i++) {
        double temp = data[i * mat->num_cols + col1];
        data[i * mat->num_cols + col1] = data[i * mat->num_cols + col2];
        data[i * mat->num_cols + col2] = temp;
    }

}



matrix *matrix_add(const matrix *mat1, const matrix *mat2) {
    // Check if both matrices have the same dimensions
    if (!matrix_eqdim(mat1, mat2)) {
        fprintf(stderr, "Matrices dimensions do not match.\n");
        return NULL;
    }

    // Create a new matrix to store the result
    matrix *result = matrix_new(mat1->num_rows, mat1->num_cols, sizeof(double));
    if (!result) {
        return NULL; // Memory allocation failure
    }

    // Perform the addition
    double *data1 = (double *)mat1->data;
    double *data2 = (double *)mat2->data;
    double *result_data = (double *)result->data;

    for (unsigned int i = 0; i < mat1->num_rows; ++i) {
        for (unsigned int j = 0; j < mat1->num_cols; ++j) {
            result_data[i * mat1->num_cols + j] = data1[i * mat1->num_cols + j] + data2[i * mat1->num_cols + j];
        }
    }

    return result;
}

