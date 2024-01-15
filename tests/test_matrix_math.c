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
    printf("Matrix `mat` before scalar mult of second row\n");
    matrix_print(mat);

    double scalar = 2.0;
    matrix_row_mult_r(mat, 1, scalar); // Multiply second row by scalar

    // Verify that the second row is multiplied by scalar
    printf("Matrix `mat` after scalar mult of second row by 2.\n");
    matrix_print(mat);

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

Test(matrix_math, add_rows_square_matrix) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    double values[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    memcpy(mat->data, values, 9 * sizeof(double));

    matrix_row_addrow(mat, 0, 1, 1); // Add rows 0 and 1, store result in row 2

    // Check if the sum is correct in row 2
    cr_assert_eq(((double*)mat->data)[3], 5.0, "Row addition result is incorrect");
    cr_assert_eq(((double*)mat->data)[4], 7.0, "Row addition result is incorrect");
    cr_assert_eq(((double*)mat->data)[5], 9.0, "Row addition result is incorrect");

    matrix_free(mat);
}

Test(matrix_math, add_rows_rectangular_matrix) {
    matrix *mat = matrix_new(4, 2, sizeof(double));
    double values[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    memcpy(mat->data, values, 8 * sizeof(double));

    matrix_row_addrow(mat, 1, 2, 3); // Add rows 1 and 2, store result in row 3

    // Check if the sum is correct in row 3
    cr_assert_eq(((double*)mat->data)[6], 8.0, "Row addition result is incorrect");
    cr_assert_eq(((double*)mat->data)[7], 10.0, "Row addition result is incorrect");

    matrix_free(mat);
}


Test(matrix_math, add_rows_invalid_indices) {
    matrix *mat = matrix_new(2, 2, sizeof(double));
    double values[4] = {1.0, 2.0, 3.0, 4.0};
    memcpy(mat->data, values, 4 * sizeof(double));

    // Call with invalid indices and check if function handles it gracefully
    matrix_row_addrow(mat, 2, 3, 1); // Invalid indices

    // Check if the matrix is unchanged
    cr_assert_eq(((double*)mat->data)[0], 1.0, "Matrix should be unchanged");
    cr_assert_eq(((double*)mat->data)[1], 2.0, "Matrix should be unchanged");
    cr_assert_eq(((double*)mat->data)[2], 3.0, "Matrix should be unchanged");
    cr_assert_eq(((double*)mat->data)[3], 4.0, "Matrix should be unchanged");

    matrix_free(mat);
}


Test(matrix_math, add_rows_result_invalid_indices) {
    matrix *mat = matrix_new(2, 2, sizeof(double));
    double values[4] = {1.0, 2.0, 3.0, 4.0};
    memcpy(mat->data, values, 4 * sizeof(double));

    // Call with invalid indices and check if function handles it gracefully
    matrix_row_addrow(mat, 0, 1, 4); // Invalid indices

    // Check if the matrix is unchanged
    cr_assert_eq(((double*)mat->data)[0], 1.0, "Matrix should be unchanged");
    cr_assert_eq(((double*)mat->data)[1], 2.0, "Matrix should be unchanged");
    cr_assert_eq(((double*)mat->data)[2], 3.0, "Matrix should be unchanged");
    cr_assert_eq(((double*)mat->data)[3], 4.0, "Matrix should be unchanged");

    matrix_free(mat);
}

Test(matrix_math, add_two_square_matrices) {
    // Create two 3x3 matrices
    matrix *mat1 = matrix_new(3, 3, sizeof(double));
    matrix *mat2 = matrix_new(3, 3, sizeof(double));
    cr_assert_not_null(mat1, "Matrix 1 allocation returned NULL");
    cr_assert_not_null(mat2, "Matrix 2 allocation returned NULL");

    // Initialize matrices with values
    double values1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double values2[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    memcpy(mat1->data, values1, 9 * sizeof(double));
    memcpy(mat2->data, values2, 9 * sizeof(double));

    // Perform matrix addition
    matrix *result = matrix_add(mat1, mat2);

    // Check if the result matrix is correct
    double *result_data = (double *) result->data;
    for (int i = 0; i < 9; i++) {
        cr_assert_eq(result_data[i], 10, "Element at index %d is not correct", i);
    }

    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}


Test(matrix_math, add_two_rectangular_matrices) {
    // Create two 2x4 matrices
    matrix *mat1 = matrix_new(2, 4, sizeof(double));
    matrix *mat2 = matrix_new(2, 4, sizeof(double));
    cr_assert_not_null(mat1, "Matrix 1 allocation returned NULL");
    cr_assert_not_null(mat2, "Matrix 2 allocation returned NULL");

    // Initialize matrices with values
    double values1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    double values2[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    memcpy(mat1->data, values1, 8 * sizeof(double));
    memcpy(mat2->data, values2, 8 * sizeof(double));

    // Perform matrix addition
    matrix *result = matrix_add(mat1, mat2);

    // Check if the result matrix is correct
    double *result_data = (double *) result->data;
    double expected_values[8] = {9, 9, 9, 9, 9, 9, 9, 9};
    for (int i = 0; i < 8; i++) {
        cr_assert_eq(result_data[i], expected_values[i], "Element at index %d is not correct", i);
    }

    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}


