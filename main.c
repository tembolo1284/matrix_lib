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
  
  return 0;

}
