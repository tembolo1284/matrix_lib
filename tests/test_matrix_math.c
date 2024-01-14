#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "../include/matrix.h"
#include <stdio.h>

// Test case for multiplying a row by a scalar
Test(matrix_math, row_mult_r) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double *data = (double *) mat->data;
    for (unsigned int i = 0; i < 9; i++) {
        data[i] = i + 1; // 1 to 9
    }

    double scalar = 2.0;
    matrix_row_mult_r(mat, 1, scalar); // Multiply second row by scalar

    // Verify that the second row is multiplied by scalar
    for (unsigned int i = 0; i < 3; i++) {
        cr_assert_eq(data[3 + i], (i + 4) * scalar, "Element at [1][%u] is not correctly multiplied", i);
    }

    matrix_free(mat);
}

// Test case for multiplying a column by a scalar
Test(matrix_math, col_mult_r) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double *data = (double *) mat->data;
    for (unsigned int i = 0; i < 9; i++) {
        data[i] = i + 1; // 1 to 9
    }

    double scalar = 3.0;
    matrix_col_mult_r(mat, 2, scalar); // Multiply third column by scalar

    // Verify that the third column is multiplied by scalar
    for (unsigned int i = 0; i < 3; i++) {
        cr_assert_eq(data[i * 3 + 2], (i * 3 + 3) * scalar, "Element at [%u][2] is not correctly multiplied", i);
    }

    matrix_free(mat);
}

// Test case for multiplying the entire matrix by a scalar
Test(matrix_math, matrix_mult_r) {
    matrix *mat = matrix_new(2, 2, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double *data = (double *) mat->data;
    data[0] = 1.0; 
    data[1] = 2.0;
    data[2] = 3.0; 
    data[3] = 4.0;

    double scalar = 6.0;
    matrix_mult_r(mat, scalar); // Multiply entire matrix by scalar

    // Verify that each element is multiplied by scalar
    for (unsigned int i = 0; i < 4; i++) {
        cr_assert_eq(data[i], (i + 1) * scalar, "Element [%u] is not correctly multiplied", i);
    }

    matrix_free(mat);
}

