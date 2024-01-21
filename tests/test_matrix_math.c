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
Test(matrix_math, add_rows_square_matrix) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    double values[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    memcpy(mat->data, values, 9 * sizeof(double));

    matrix_row_addrow(mat, 0, 1, 2.0); // Add rows 0 and 1, store result in row 1

    // Check if the sum is correct in row 1
    cr_assert_eq(((double*)mat->data)[3], 6.0, "Row addition result is incorrect");
    cr_assert_eq(((double*)mat->data)[4], 9.0, "Row addition result is incorrect");
    cr_assert_eq(((double*)mat->data)[5], 12.0, "Row addition result is incorrect");

    matrix_free(mat);
}

Test(matrix_math, add_rows_rectangular_matrix) {
    matrix *mat = matrix_new(4, 2, sizeof(double));
    double values[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    memcpy(mat->data, values, 8 * sizeof(double));

    matrix_row_addrow(mat, 1, 2, 3.0); // Add rows 1 and 2, store result in row 2

    // Check if the sum is correct in row 1
    cr_assert_eq(((double*)mat->data)[4], 14.0, "Row addition result is incorrect");
    cr_assert_eq(((double*)mat->data)[5], 18.0, "Row addition result is incorrect");

    matrix_free(mat);
}

Test(matrix_math, add_rows_invalid_indices) {
    matrix *mat = matrix_new(2, 2, sizeof(double));
    double values[4] = {1.0, 2.0, 3.0, 4.0};
    memcpy(mat->data, values, 4 * sizeof(double));

    // Call with invalid indices and check if function handles it gracefully
    matrix_row_addrow(mat, 2, 3, 1.0); // Invalid indices

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


Test(matrix_math, subtract_two_square_matrices) {
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
    matrix *result = matrix_subtract(mat1, mat2);

    // Check if the result matrix is correct
    double *result_data = (double *) result->data;
    double expected_values[9] = {-8, -6, -4, -2, 0, 2, 4, 6, 8};
    for (int i = 0; i < 9; i++) {
        cr_assert_eq(result_data[i], expected_values[i], "Element at index %d is not correct", i);
    }

    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}


// Test case for matrix multiplication with square matrices
Test(matrix_math, matrix_mult_square_matrices) {
    matrix *mat1 = matrix_new(2, 2, sizeof(double));
    matrix *mat2 = matrix_new(2, 2, sizeof(double));

    double values1[4] = {1.0, 2.0, 3.0, 4.0};
    double values2[4] = {5.0, 6.0, 7.0, 8.0};

    memcpy(mat1->data, values1, 4 * sizeof(double));
    memcpy(mat2->data, values2, 4 * sizeof(double));

    matrix *result = matrix_mult(mat1, mat2);

    double *result_data = (double *)result->data;
    double expected_values[4] = {19.0, 22.0, 43.0, 50.0};

    for (int i = 0; i < 4; i++) {
        cr_assert_eq(result_data[i], expected_values[i], "Element at index %d is not correct", i);
    }

    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}

// Test case for matrix multiplication with rectangular matrices
Test(matrix_math, matrix_mult_rectangular_matrices) {
    matrix *mat1 = matrix_new(2, 3, sizeof(double));
    matrix *mat2 = matrix_new(3, 2, sizeof(double));

    double values1[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    double values2[6] = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0};

    memcpy(mat1->data, values1, 6 * sizeof(double));
    memcpy(mat2->data, values2, 6 * sizeof(double));

    matrix *result = matrix_mult(mat1, mat2);

    double *result_data = (double *)result->data;
    double expected_values[4] = {58.0, 64.0, 139.0, 154.0};

    for (int i = 0; i < 4; i++) {
        cr_assert_eq(result_data[i], expected_values[i], "Element at index %d is not correct", i);
    }

    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}

// Test case for pivotidx function
Test(matrix_math, pivotidx_test) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    double values[9] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    memcpy(mat->data, values, 9 * sizeof(double));

    unsigned int pivot = matrix_pivotidx(mat, 0, 0);

    cr_assert_eq(pivot, 2, "Pivot index is incorrect");

    matrix_free(mat);
}

// Test case for matrix_ref function
Test(matrix_math, matrix_ref_test) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    double values[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    memcpy(mat->data, values, 9 * sizeof(double));

    matrix *ref = matrix_ref(mat);

    // Check if the dimensions of the reference matrix are correct
    cr_assert_eq(ref->num_rows, 3, "Reference matrix has incorrect number of rows");
    cr_assert_eq(ref->num_cols, 3, "Reference matrix has incorrect number of columns");

    // Check if the values in the reference matrix match the expected values
    double *ref_data = (double *)ref->data;
    double expected_values[9] = {1.0, 1.142857, 1.285714, 0.0, 1.0, 2.0, 0.0, 0.0, 0.0};
    matrix_print(ref);
    for (int i = 0; i < 9; i++) {
        cr_assert_float_eq(ref_data[i], expected_values[i], 1e-6, "Element at index %d in reference matrix is incorrect", i);
    }

    matrix_free(mat);
    matrix_free(ref);
}

