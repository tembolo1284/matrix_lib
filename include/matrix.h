#ifndef MATRIX_H
#define MATRIX_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct matrix_s {
  unsigned int num_rows;
  unsigned int num_cols;
  void *data;
  bool is_square;
} matrix;

typedef struct Range_s {
  int start;
  int end;
} Range;

matrix *matrix_new(unsigned int num_rows, unsigned int num_cols, size_t element_size);

matrix *matrix_rand(unsigned int num_rows, 
                    unsigned int num_cols, 
                    double min, 
                    double max, 
                    size_t element_size);

matrix *matrix_sqr(unsigned int size, size_t element_size);
matrix *matrix_eye(unsigned int size, size_t element_size, const void *identity_element);

void matrix_free(matrix *mat);
void matrix_print(const matrix *matrix);
void matrix_printf(const matrix *matrix, const char *d_fmt);

int matrix_eqdim(const matrix *m1, const matrix *m2);
int matrix_eq(const matrix *m1, const matrix *m2, double tolerance);

matrix *matrix_col_get(const matrix *mat, unsigned int col_num);
matrix *matrix_row_get(const matrix *mat, unsigned int row_num);

double matrix_at(const matrix *mat, unsigned int i, unsigned int j);
matrix *matrix_slice(matrix *mat, Range row_range, Range col_range);
matrix *matrix_submatrix(const matrix *mat, Range row_range, Range col_range);
matrix *matrix_copy(const matrix *src);

void matrix_all_set(matrix *mat, const void *value, size_t value_size);
void matrix_diag_set(matrix *mat, const void *value, size_t value_size);

/*Matrix math operations*/
//multiply a row with a scalar
void matrix_row_mult_r(matrix *mat, unsigned int row, double value);

//multiply a column with a scalar
void matrix_col_mult_r(matrix *mat, unsigned int col, double value);

//multiply a matrix with a scalar (all rows and columns)
void matrix_mult_r(matrix *mat, double value);



#endif //MATRIX_H
