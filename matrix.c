#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

nml_mat* nml_allocate_mat(unsigned int num_rows, unsigned int num_cols) {

  nml_mat *mat = (nml_mat*)malloc(sizeof(nml_mat));
  mat->num_rows = num_rows;
  mat->num_cols = num_cols;
  mat->is_square = (num_rows == num_cols);

  mat->data = (double*)malloc(num_rows * num_cols * sizeof(double));

  return mat;
}

void nml_free_mat(nml_mat *mat) {
  free(mat->data); 
  free(mat);
}

void nml_mat_print(nml_mat *matrix) {
  nml_mat_printf(matrix, "%lf\t"); 
}

void nml_mat_printf(nml_mat *matrix, const char *d_fmt) {

  int i, j;
  fprintf(stdout, "\n");
  
  for(i = 0; i < matrix->num_rows; ++i) {
    for(j = 0; j < matrix->num_cols; ++j) {
      fprintf(stdout, d_fmt, 
        matrix->data[i*matrix->num_cols + j]); 
    }
    fprintf(stdout, "\n");
  }
  
  fprintf(stdout, "\n");
}
