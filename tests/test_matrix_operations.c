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
// Test for matrix_col_get function
Test(matrix, col_get) {
    unsigned int rows = 3, cols = 4, col_num = 2;
    matrix *mat = matrix_new(rows, cols, sizeof(double));
    matrix *col = matrix_col_get(mat, col_num);
    cr_assert_not_null(col, "matrix_col_get returned NULL");
    cr_assert_eq(col->num_rows, mat->num_rows, "Incorrect number of rows in column matrix");
    cr_assert_eq(col->num_cols, 1, "Column matrix should have only 1 column");
    matrix_free(mat);
    matrix_free(col);
}

// Test for matrix_row_get function
Test(matrix, row_get) {
    unsigned int rows = 3, cols = 4, row_num = 1;
    matrix *mat = matrix_new(rows, cols, sizeof(double));
    matrix *row = matrix_row_get(mat, row_num);
    cr_assert_not_null(row, "matrix_row_get returned NULL");
    cr_assert_eq(row->num_cols, mat->num_cols, "Incorrect number of columns in row matrix");
    cr_assert_eq(row->num_rows, 1, "Row matrix should have only 1 row");
    matrix_free(mat);
    matrix_free(row);
}

// Test for matrix_slice function for a single row
Test(matrix, slice_single_row) {
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
Test(matrix, slice_single_column) {
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
Test(matrix, slice_row_col_range) {
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

