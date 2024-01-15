#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include "../include/matrix.h"  // Replace with your actual matrix library header

Test(matrix_operations, eqdim_same_dimensions) {
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

Test(matrix_operations, eqdim_different_dimensions) {
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

Test(matrix_operations, eq_within_tolerance) {
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

Test(matrix_operations, eq_outside_tolerance) {
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

    data2[0] = 1.0;
    data2[1] = 2.0;
    data2[2] = 3.05;
    data2[3] = 4.0;

    double tolerance = 0.02; // Set your desired tolerance value

    // Ensure that mat1 and mat2 are not equal within the specified tolerance
    cr_assert_not(matrix_eq(mat1, mat2, tolerance), "matrix_eq failed for matrices outside tolerance");

    matrix_free(mat1);
    matrix_free(mat2);
}

// Test for matrix_slice function for a single row
Test(matrix_operations, slice_single_row) {
    unsigned int rows = 3, cols = 4;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    // Initialize matrix data...

    Range single_row = {1, 2}; // 2nd row only
    Range all_cols = {-1, -1}; // All columns
    matrix *sliced_row = matrix_slice(mat, single_row, all_cols);

    cr_assert_not_null(sliced_row, "matrix_slice returned NULL for single row");
    cr_assert_eq(sliced_row->num_rows, 1, "Sliced row matrix should have 1 row");
    cr_assert_eq(sliced_row->num_cols, cols, "Sliced row matrix should have all columns");

    matrix_free(mat);
    matrix_free(sliced_row);
}

// Test for matrix_slice function for a single column
Test(matrix_operations, slice_single_column) {
    unsigned int rows = 3, cols = 4;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    // Initialize matrix data...

    Range all_rows = {-1, -1}; // All rows
    Range single_col = {2, 3}; // 3rd column only
    matrix *sliced_col = matrix_slice(mat, all_rows, single_col);

    cr_assert_not_null(sliced_col, "matrix_slice returned NULL for single column");
    cr_assert_eq(sliced_col->num_rows, rows, "Sliced column matrix should have all rows");
    cr_assert_eq(sliced_col->num_cols, 1, "Sliced column matrix should have 1 column");

    matrix_free(mat);
    matrix_free(sliced_col);
}

// Test for matrix_slice function for a range of rows and columns
Test(matrix_operations, slice_row_col_range) {
    unsigned int rows = 4, cols = 5;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    // Initialize matrix data...

    Range row_range = {1, 3}; // Rows 2 to 3
    Range col_range = {2, 4}; // Columns 3 to 4
    matrix *submatrix = matrix_slice(mat, row_range, col_range);

    cr_assert_not_null(submatrix, "matrix_slice returned NULL for row and column range");
    cr_assert_eq(submatrix->num_rows, 2, "Submatrix should have 2 rows");
    cr_assert_eq(submatrix->num_cols, 2, "Submatrix should have 2 columns");

    matrix_free(mat);
    matrix_free(submatrix);
}

Test(matrix_operations, slice_full_matrix) {
    unsigned int rows = 3, cols = 4;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    Range all_rows = {-1, -1};
    Range all_cols = {-1, -1};
    matrix *sliced = matrix_slice(mat, all_rows, all_cols);

    cr_assert_not_null(sliced, "matrix_slice returned NULL for full matrix");
    cr_assert_eq(sliced->num_rows, rows, "Sliced matrix should have all rows");
    cr_assert_eq(sliced->num_cols, cols, "Sliced matrix should have all columns");

    matrix_free(mat);
    matrix_free(sliced);
}


Test(matrix_operations, slice_all_rows_one_column) {
    unsigned int rows = 3, cols = 4, col_num = 1;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    Range all_rows = {-1, -1};
    Range single_col = {col_num, col_num + 1};
    matrix *sliced = matrix_slice(mat, all_rows, single_col);

    cr_assert_not_null(sliced, "matrix_slice returned NULL for all rows and one column");
    cr_assert_eq(sliced->num_rows, rows, "Sliced matrix should have all rows");
    cr_assert_eq(sliced->num_cols, 1, "Sliced matrix should have one column");

    matrix_free(mat);
    matrix_free(sliced);
}


Test(matrix_operations, slice_all_rows_range_of_columns) {
    unsigned int rows = 3, cols = 4;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    Range all_rows = {-1, -1};
    Range col_range = {1, 3};  // Columns 2 to 3
    matrix *sliced = matrix_slice(mat, all_rows, col_range);

    cr_assert_not_null(sliced, "matrix_slice returned NULL for all rows and range of columns");
    cr_assert_eq(sliced->num_rows, rows, "Sliced matrix should have all rows");
    cr_assert_eq(sliced->num_cols, 2, "Sliced matrix should have range of columns");

    matrix_free(mat);
    matrix_free(sliced);
}


Test(matrix_operations, slice_range_of_rows_one_column) {
    unsigned int rows = 4, cols = 5, col_num = 2;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    Range row_range = {1, 3};  // Rows 2 to 3
    Range single_col = {col_num, col_num + 1};
    matrix *sliced = matrix_slice(mat, row_range, single_col);
    cr_assert_not_null(sliced, "matrix_slice returned NULL for range of rows and one column");
    cr_assert_eq(sliced->num_rows, 2, "Sliced matrix should have range of rows");
    cr_assert_eq(sliced->num_cols, 1, "Sliced matrix should have one column");

    matrix_free(mat);
    matrix_free(sliced);
}

Test(matrix_operations, slice_range_of_rows_and_columns) {
    unsigned int rows = 4, cols = 5;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    Range row_range = {1, 3}; // Rows 2 to 3
    Range col_range = {2, 4}; // Columns 3 to 4
    matrix *sliced = matrix_slice(mat, row_range, col_range);

    cr_assert_not_null(sliced, "matrix_slice returned NULL for range of rows and columns");
    cr_assert_eq(sliced->num_rows, 2, "Sliced matrix should have range of rows");
    cr_assert_eq(sliced->num_cols, 2, "Sliced matrix should have range of columns");

    matrix_free(mat);
    matrix_free(sliced);
}

Test(matrix_operations, slice_one_row_one_column) {
    unsigned int rows = 3, cols = 4, row_num = 1, col_num = 2;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    Range single_row = {row_num, row_num + 1};
    Range single_col = {col_num, col_num + 1};
    matrix *sliced = matrix_slice(mat, single_row, single_col);

    cr_assert_not_null(sliced, "matrix_slice returned NULL for one row and one column");
    cr_assert_eq(sliced->num_rows, 1, "Sliced matrix should have one row");
    cr_assert_eq(sliced->num_cols, 1, "Sliced matrix should have one column");

    matrix_free(mat);
    matrix_free(sliced);
}

Test(matrix_operations, slice_one_row_range_of_columns) {
    unsigned int rows = 3, cols = 4, row_num = 1;
    matrix *mat = matrix_new(rows, cols, sizeof(double));

    Range single_row = {row_num, row_num + 1};
    Range col_range = {1, 3}; // Columns 2 to 3
    matrix *sliced = matrix_slice(mat, single_row, col_range);

    cr_assert_not_null(sliced, "matrix_slice returned NULL for one row and range of columns");
    cr_assert_eq(sliced->num_rows, 1, "Sliced matrix should have one row");
    cr_assert_eq(sliced->num_cols, 2, "Sliced matrix should have range of columns");

    matrix_free(mat);
    matrix_free(sliced);
}

Test(matrix_operations, swap_rows_square_matrix) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    memcpy(mat->data, values, 9 * sizeof(double));

    matrix_swap_rows(mat, 0, 2); // Swap first and last rows

    // Verify that rows are swapped
    cr_assert_eq(((double*)mat->data)[0], 7, "First element of first row is incorrect after swap");
    cr_assert_eq(((double*)mat->data)[8], 3, "Last element of last row is incorrect after swap");

    matrix_free(mat);
}

// Test case for swapping two rows in a rectangular matrix
Test(matrix_operations, swap_rows_rectangular_matrix) {
    matrix *mat = matrix_new(4, 2, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double values[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    memcpy(mat->data, values, 8 * sizeof(double));

    matrix_swap_rows(mat, 1, 3); // Swap second and fourth rows

    // Verify that rows are swapped
    cr_assert_eq(((double*)mat->data)[2], 7, "First elem of second row is incorrect after swap");
    cr_assert_eq(((double*)mat->data)[7], 4, "Last elem of fourth row is incorrect after swap");

    matrix_free(mat);
}

// Test case for swapping two columns in a square matrix
Test(matrix_operations, swap_cols_square_matrix) {
    matrix *mat = matrix_new(3, 3, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    memcpy(mat->data, values, 9 * sizeof(double));

    matrix_swap_cols(mat, 0, 2); // Swap first and last columns

    // Verify that columns are swapped
    cr_assert_eq(((double*)mat->data)[0], 3, "First elem of first column is incorrect after swap");
    cr_assert_eq(((double*)mat->data)[8], 7, "Last elem of last column is incorrect after swap");

    matrix_free(mat);

}

// Test case for swapping two columns in a rectangular matrix
Test(matrix_operations, swap_cols_rectangular_matrix) {
    matrix *mat = matrix_new(2, 4, sizeof(double));
    cr_assert_not_null(mat, "Matrix allocation returned NULL");

    // Initialize matrix with some values
    double values[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    memcpy(mat->data, values, 8 * sizeof(double));

    matrix_swap_cols(mat, 1, 3); // Swap second and fourth columns

    // Verify that columns are swapped
    cr_assert_eq(((double*)mat->data)[1], 4, "Second elem of first row is incorrect after swap");
    cr_assert_eq(((double*)mat->data)[7], 6, "Last elem of second row is incorrect after swap");

    matrix_free(mat);

}




