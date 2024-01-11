#include "matrix.h"
#include <stdio.h>
#include <string.h>

int main() {
    // Allocate 2x3 double matrix
    matrix *mat = matrix_new(2, 3, sizeof(double));
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
    printf("First matrix of doubles 2x3 from 1.0 to 6.0\n");
    matrix_print(mat);


    printf("Printing third column of the matrix above.\n");

    matrix *mat_col3 = matrix_col_get(mat, 2);
    matrix_print(mat_col3);
    matrix_free(mat_col3); 

    printf("Print second row of the matrix above.\n");
    matrix *mat_row2 = matrix_row_get(mat, 1);
    matrix_print(mat_row2);
    matrix_free(mat_row2);

    // Free memory
    matrix_free(mat);

    matrix *mat1 = matrix_rand(3, 4, 0.0, 10.0, sizeof(double));
    printf("\nSecond matrix of random doubles 3x4 with values from 0.0-10.0\n");
    matrix_print(mat1);

    // Allocate 4x4 int square matrix
    matrix *mat2 = matrix_sqr(4, sizeof(double));
    if (!mat2) return 1;

    // Confirm we have an initial matrix of all zeroes
    printf("Check our 4x4 square matrix is initialized with all zeroes\n");
    matrix_print(mat2);

    // Initialize values for int matrix
    double *data_int = (double *)mat2->data;
    for (int i = 0; i < (int)(mat2->num_rows); i++) {
        for (int j = 0; j < (int)(mat2->num_cols); j++) {
            data_int[i * mat2->num_cols + j] = i + j + 1;
        }
    }

    // Print the int matrix
    printf("Confirm the square matrix is now from 1.0 to 16.0\n");
    matrix_print(mat2);

    // Free the matrix memory
    matrix_free(mat2);

    // Allocate 6x6 double identity matrix
    double identity_double = 1.0;
    matrix *mat3 = matrix_eye(6, sizeof(double), &identity_double);
    if (!mat3) return 1;

    printf("Confirm we have a proper 6x6 identity matrix\n");
    // Print the double identity matrix
    matrix_print(mat3);

    // Free the matrix memory
    matrix_free(mat3);

    return 0;
}

