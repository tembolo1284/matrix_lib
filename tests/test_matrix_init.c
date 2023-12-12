#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "../include/matrix.h"  // Replace with your actual matrix library header

// Test case for matrix allocation
Test(matrix, new_3by4) {
    int rows = 3, cols = 4;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    cr_assert_not_null(mat, "Matrix allocation returned NULL");
    cr_assert_eq(mat->num_rows, rows, "Number of rows is not correct");
    cr_assert_eq(mat->num_cols, cols, "Number of cols is not correct");

    // Further assertions can be added here as needed

    matrix_free(mat);  // Assuming this is your deallocation function
}

Test(matrix, square_matrix) {
    unsigned int size = 4;
    matrix *mat = matrix_sqr(size, sizeof(double));

    cr_assert_not_null(mat, "matrix_sqr returned NULL");
    cr_assert_eq(mat->num_rows, size, "Square matrix does not have the correct number of rows");
    cr_assert_eq(mat->num_cols, size, "Square matrix does not have the correct number of columns");
    cr_assert(mat->is_square, "Matrix is not square when it should be");

    matrix_free(mat);
}

Test(matrix, identity_matrix) {
    unsigned int size = 4;
    double identity_element = 1.0;
    matrix *mat = matrix_eye(size, sizeof(double), &identity_element);

    cr_assert_not_null(mat, "matrix_eye returned NULL");
    cr_assert_eq(mat->num_rows, size, "Identity matrix does not have the correct number of rows");
    cr_assert_eq(mat->num_cols, size, "Identity matrix does not have the correct number of columns");
    cr_assert(mat->is_square, "Matrix is not square when it should be");

    // Check if the diagonal elements are 1 and others are 0
    double *data = (double *)mat->data;
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < size; j++) {
            double expected_value = (i == j) ? identity_element : 0.0;
            cr_assert_eq(data[i * size + j], expected_value, "Element at [%u][%u] is not correct", i, j);
        }
    }

    matrix_free(mat);
}

Test(matrix, print) {
    matrix *mat = matrix_new(2, 2, sizeof(double));
    if (!mat) {
      cr_assert_fail("Matrix allocation failed");
    }

    // Cast the void* data to double* for easy manipulation
    double *data = (double *) mat->data;

    // Initialize matrix with some values
    data[0] = 1.0;  // element at row 0, col 0
    data[1] = 2.0;  // element at row 0, col 1
    data[2] = 3.0;  // element at row 1, col 0
    data[3] = 4.0;  // element at row 1, col 1

    // Printing the matrix
    cr_log_info("Printing matrix:\n");
    matrix_print(mat);

    matrix_free(mat);
}


