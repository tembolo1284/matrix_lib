#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "../include/matrix.h"  // Replace with your actual matrix library header

Test(matrix, eqdim_same_dimensions) {
    unsigned int rows = 3, cols = 4;
    matrix *mat1 = matrix_new(rows, cols, sizeof(double));
    matrix *mat2 = matrix_new(rows, cols, sizeof(double));

    cr_assert_not_null(mat1, "Matrix allocation for mat1 returned NULL");
    cr_assert_not_null(mat2, "Matrix allocation for mat2 returned NULL");

    // Ensure that mat1 and mat2 have the same dimensions
    cr_assert(matrix_eqdim(mat1, mat2), "matrix_eqdim failed for matrices with the same dimensions");

    matrix_free(mat1);
    matrix_free(mat2);
}

Test(matrix, eqdim_different_dimensions) {
    unsigned int rows1 = 3, cols1 = 4;
    unsigned int rows2 = 4, cols2 = 3;
    matrix *mat1 = matrix_new(rows1, cols1, sizeof(double));
    matrix *mat2 = matrix_new(rows2, cols2, sizeof(double));

    cr_assert_not_null(mat1, "Matrix allocation for mat1 returned NULL");
    cr_assert_not_null(mat2, "Matrix allocation for mat2 returned NULL");

    // Ensure that mat1 and mat2 have different dimensions
    cr_assert_not(matrix_eqdim(mat1, mat2), "matrix_eqdim failed for matrices with different dimensions");

    matrix_free(mat1);
    matrix_free(mat2);
}

Test(matrix, eq_within_tolerance) {
    unsigned int rows = 2, cols = 2;
    matrix *mat1 = matrix_new(rows, cols, sizeof(double));
    matrix *mat2 = matrix_new(rows, cols, sizeof(double));

    cr_assert_not_null(mat1, "Matrix allocation for mat1 returned NULL");
    cr_assert_not_null(mat2, "Matrix allocation for mat2 returned NULL");

    // Initialize mat1 and mat2 with values close to each other within tolerance
    double *data1 = (double *)mat1->data;
    double *data2 = (double *)mat2->data;
    data1[0] = 1.0;
    data1[1] = 2.0;
    data1[2] = 3.0;
    data1[3] = 4.0;

    data2[0] = 1.01; // Within tolerance of 0.01
    data2[1] = 2.01; // Within tolerance of 0.01
    data2[2] = 3.0;  // Exact match
    data2[3] = 4.0;  // Exact match

    double tolerance = 0.02; // Set your desired tolerance value

    // Ensure that mat1 and mat2 are equal within the specified tolerance
    cr_assert(matrix_eq(mat1, mat2, tolerance), "matrix_eq failed for matrices within tolerance");

    matrix_free(mat1);
    matrix_free(mat2);
}

Test(matrix, eq_outside_tolerance) {
    unsigned int rows = 2, cols = 2;
    matrix *mat1 = matrix_new(rows, cols, sizeof(double));
    matrix *mat2 = matrix_new(rows, cols, sizeof(double));

    cr_assert_not_null(mat1, "Matrix allocation for mat1 returned NULL");
    cr_assert_not_null(mat2, "Matrix allocation for mat2 returned NULL");

    // Initialize mat1 and mat2 with values outside the specified tolerance
    double *data1 = (double *)mat1->data;
    double *data2 = (double *)mat2->data;
    data1[0] = 1.0;
    data1[1] = 2.0;
    data1[2] = 3.0;
    data1[3] = 4.0;

    data2[0] = 1.03; // Outside tolerance of 0.02
    data2[1] = 2.01; // Within tolerance of 0.02
    data2[2] = 3.0;  // Exact match
    data2[3] = 4.0;  // Exact match

    double tolerance = 0.02; // Set your desired tolerance value

    // Ensure that mat1 and mat2 are not equal within the specified tolerance
    cr_assert_not(matrix_eq(mat1, mat2, tolerance), "matrix_eq failed for matrices outside tolerance");

    matrix_free(mat1);
    matrix_free(mat2);
}

