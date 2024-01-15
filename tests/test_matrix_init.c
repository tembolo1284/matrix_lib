#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "../include/matrix.h"  

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

Test(matrix_operations, transpose_square_matrix) {
    matrix *mat = matrix_new(2, 2, sizeof(double));
    double values[4] = {1.0, 2.0, 3.0, 4.0};
    memcpy(mat->data, values, 4 * sizeof(double));

    matrix_transpose(mat);

    cr_assert_eq(((double*)mat->data)[0], 1.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[1], 3.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[2], 2.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[3], 4.0, "Incorrect value in transposed matrix");

    matrix_free(mat);
}

Test(matrix_operations, transpose_rectangular_matrix) {
    matrix *mat = matrix_new(2, 4, sizeof(double));
    double values[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    memcpy(mat->data, values, 8 * sizeof(double));

    matrix_transpose(mat);

    cr_assert_eq(mat->num_rows, 4, "Number of rows should be 3 after transpose");
    cr_assert_eq(mat->num_cols, 2, "Number of columns should be 2 after transpose");
    cr_assert_eq(((double*)mat->data)[0], 1.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[1], 5.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[2], 2.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[3], 6.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[4], 3.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[5], 7.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[6], 4.0, "Incorrect value in transposed matrix");
    cr_assert_eq(((double*)mat->data)[7], 8.0, "Incorrect value in transposed matrix");

    matrix_free(mat);
}

Test(matrix_operations, stackv_square_matrices) {
    matrix *mat1 = matrix_new(2, 2, sizeof(double));
    matrix *mat2 = matrix_new(2, 2, sizeof(double));
    double values1[4] = {1.0, 2.0, 3.0, 4.0};
    double values2[4] = {5.0, 6.0, 7.0, 8.0};
    memcpy(mat1->data, values1, 4 * sizeof(double));
    memcpy(mat2->data, values2, 4 * sizeof(double));

    matrix *result = matrix_stackv(mat1, mat2);

    cr_assert_not_null(result, "matrix_stackv returned NULL");
    cr_assert_eq(result->num_rows, 4, "Stacked matrix should have 4 rows");
    cr_assert_eq(result->num_cols, 2, "Stacked matrix should have 2 columns");

    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}


Test(matrix_operations, stackv_rectangular_matrices) {
    matrix *mat1 = matrix_new(2, 3, sizeof(double));
    matrix *mat2 = matrix_new(2, 3, sizeof(double));
    // Initialize matrices mat1 and mat2 with some values

    matrix *result = matrix_stackv(mat1, mat2);

    cr_assert_not_null(result, "matrix_stackv returned NULL");
    cr_assert_eq(result->num_rows, 4, "Stacked matrix should have 4 rows");
    cr_assert_eq(result->num_cols, 3, "Stacked matrix should have 3 columns");

    // Optionally, add assertions to check the values in the stacked matrix
   
    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);

}


Test(matrix_operations, stackh_square_matrices) {
    matrix *mat1 = matrix_new(2, 2, sizeof(double));
    matrix *mat2 = matrix_new(2, 2, sizeof(double));
    double values1[4] = {1.0, 2.0, 3.0, 4.0};
    double values2[4] = {5.0, 6.0, 7.0, 8.0};
    memcpy(mat1->data, values1, 4 * sizeof(double));
    memcpy(mat2->data, values2, 4 * sizeof(double));

    matrix *result = matrix_stackh(mat1, mat2);

    cr_assert_not_null(result, "matrix_stackh returned NULL");
    cr_assert_eq(result->num_rows, 2, "Stacked matrix should have 2 rows");
    cr_assert_eq(result->num_cols, 4, "Stacked matrix should have 4 columns");

    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}

Test(matrix_operations, stackh_rectangular_matrices) {
    matrix *mat1 = matrix_new(3, 2, sizeof(double));
    matrix *mat2 = matrix_new(3, 2, sizeof(double));
    // Initialize matrices mat1 and mat2 with some values

    matrix *result = matrix_stackh(mat1, mat2);

    cr_assert_not_null(result, "matrix_stackh returned NULL");
    cr_assert_eq(result->num_rows, 3, "Stacked matrix should have 3 rows");
    cr_assert_eq(result->num_cols, 4, "Stacked matrix should have 4 columns");

    // Optionally, add assertions to check the values in the stacked matrix
    
    matrix_free(mat1);
    matrix_free(mat2);
    matrix_free(result);
}


// Test case for removing a row
Test(matrix_math, row_remove_first) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double *data = (double *)mat->data;
    for (unsigned int i = 0; i < 9; i++) {
        data[i] = i + 1; // 1 to 9
    }

    // Remove the first row
    matrix *new_mat = matrix_row_rem(mat, 0);

    // Verify the new matrix dimensions and contents
    cr_assert_not_null(new_mat, "New matrix after row removal is NULL");
    cr_assert_eq(new_mat->num_rows, 2, "New matrix should have 2 rows");
    cr_assert_eq(new_mat->num_cols, 3, "Number of columns should remain unchanged");
    double expected_values[6] = {4, 5, 6, 7, 8, 9};
    for (unsigned int i = 0; i < 6; i++) {
        cr_assert_eq(((double *)new_mat->data)[i], expected_values[i], "Element [%u] is incorrect after row removal", i);
    }

    matrix_free(new_mat);
}

Test(matrix_math, row_remove_last) {

    matrix *mat = matrix_new(3, 4, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double *data = (double *)mat->data;
    for (unsigned int i = 0; i < 12; i++) {
        data[i] = i + 1; // 1 to 12
    }

    // Remove the last row
    matrix *new_mat = matrix_row_rem(mat, 2);

    // Verify the new matrix dimensions and contents
    cr_assert_not_null(new_mat, "New matrix after row removal is NULL");
    cr_assert_eq(new_mat->num_rows, 2, "New matrix should have 2 rows");
    cr_assert_eq(new_mat->num_cols, 4, "Number of columns should remain unchanged");
    double expected_values[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    for (unsigned int i = 0; i < 8; i++) {
        cr_assert_eq(((double *)new_mat->data)[i], expected_values[i], "Element [%u] is incorrect after row removal", i);
    }

    matrix_free(new_mat);
}

Test(matrix_math, col_remove_first) {

    matrix *mat = matrix_new(3, 4, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double *data = (double *)mat->data;
    for (unsigned int i = 0; i < 12; i++) {
        data[i] = i + 1; // 1 to 12
    }

    // Remove the last row
    matrix *new_mat = matrix_col_rem(mat, 0);

    // Verify the new matrix dimensions and contents
    cr_assert_not_null(new_mat, "New matrix after row removal is NULL");
    cr_assert_eq(new_mat->num_rows, 3, "New matrix should have 3 rows");
    cr_assert_eq(new_mat->num_cols, 3, "New matrix should have 3 colsumns");
    double expected_values[9] = {2, 3, 4, 6, 7, 8, 10, 11, 12};
    for (unsigned int i = 0; i < 9; i++) {
        cr_assert_eq(((double *)new_mat->data)[i], expected_values[i], "Element [%u] is incorrect after column removal", i);
    }

    matrix_free(new_mat);
}


Test(matrix_math, col_remove_last) {

    matrix *mat = matrix_new(3, 4, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double *data = (double *)mat->data;
    for (unsigned int i = 0; i < 12; i++) {
        data[i] = i + 1; // 1 to 12
    }

    // Remove the last row
    matrix *new_mat = matrix_col_rem(mat, 3);

    // Verify the new matrix dimensions and contents
    cr_assert_not_null(new_mat, "New matrix after row removal is NULL");
    cr_assert_eq(new_mat->num_rows, 3, "New matrix should have 3 rows");
    cr_assert_eq(new_mat->num_cols, 3, "New matrix should have 3 columns");
    double expected_values[9] = {1, 2, 3, 5, 6, 7, 9, 10, 11};
    for (unsigned int i = 0; i < 9; i++) {
        cr_assert_eq(((double *)new_mat->data)[i], expected_values[i], "Element [%u] is incorrect after column removal: (double*)new_mat->data[i] = %lf and expected_value[i] = %lf", i, ((double*)new_mat->data)[i], expected_values[i]);
    }

    matrix_free(new_mat);
}
