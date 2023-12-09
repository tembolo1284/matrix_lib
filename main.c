#include "matrix.h"  
#include <stdio.h>

int main() {

  // Allocate 3x3 matrix 
  nml_mat* mat = nml_allocate_mat(3, 3); 
  
  // Set values
  mat->data[0] = 1.0; 
  mat->data[1] = 2.0;
  mat->data[2] = 3.0;

  mat->data[3] = 4.0;
  mat->data[4] = 5.0;
  mat->data[5] = 6.0;

  mat->data[6] = 7.0; 
  mat->data[7] = 8.0;
  mat->data[8] = 9.0;

  // Print the matrix
  nml_mat_print(mat);
  
  // Free memory 
  nml_free_mat(mat);

  nml_mat* mat2 = nml_mat_sqr(4);
  for (int i = 0; i < (int)(mat2->num_rows); i++) {
    for (int j = 0; j < (int)(mat2->num_cols); j++) {
      mat2->data[i * mat2->num_cols + j] = (double)(i + j + 1);  
    }
  }

  // Print the matrix to verify
  nml_mat_print(mat2);

  // Free the matrix memory
  nml_free_mat(mat2);  

  return 0;

}
