#include "matrix.h"
#include <stdio.h>
#include <string.h>

int main() {
    // Allocate 2x3 double matrix
    matrix *mat = matrix_allocate(2, 3, sizeof(double));
    if (!mat) return 1;

    // Initialize values for double matrix
    double *data_double = (double *)mat->data;
    data_double[0] = 1.0;
    data_double[1] = 2.0;
    data_double[2] = 3.0;
    data_double[3] = 4.0;
    data_double[4] = 5.0;
    data_double[5] = 6.0;

    // Print the double matrix
    matrix_print(mat);

    // Free memory
    matrix_free(mat);

    // Allocate 4x4 int square matrix
    matrix *mat2 = matrix_sqr(4, sizeof(int));
    if (!mat2) return 1;

    // Initialize values for int matrix
    int *data_int = (int *)mat2->data;
    for (int i = 0; i < (int)(mat2->num_rows); i++) {
        for (int j = 0; j < (int)(mat2->num_cols); j++) {
            data_int[i * mat2->num_cols + j] = i + j + 1;
        }
    }

    // Print the int matrix
    matrix_print(mat2);

    // Free the matrix memory
    matrix_free(mat2);

    // Allocate 6x6 double identity matrix
    double identity_double = 1.0;
    matrix *mat3 = matrix_eye(6, sizeof(double), &identity_double);
    if (!mat3) return 1;

    // Print the double identity matrix
    matrix_print(mat3);

    // Free the matrix memory
    matrix_free(mat3);

    return 0;
}

