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

#endif //MATRIX_H
