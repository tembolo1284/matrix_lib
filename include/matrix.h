#ifndef MATRIX_H
#define MATRIX_H

typedef struct nml_mat_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double *data; 
  int is_square;
} nml_mat;

nml_mat* nml_allocate_mat(unsigned int num_rows, unsigned int num_cols);

nml_mat* nml_mat_sqr(unsigned int size);

void nml_free_mat(nml_mat *mat);

void nml_mat_print(nml_mat *matrix);
void nml_mat_printf(nml_mat *matrix, const char *d_fmt);

#endif //MATRIX_H
