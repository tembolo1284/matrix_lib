

The representation data[i][j] <=> array[i * m + j] is typical of the C programming language approach for handling two-dimensional arrays (matrices) in a one-dimensional array format. In this representation:

data[i][j] is how you would access the element at row i and column j in a two-dimensional array (matrix).
array[i * m + j] represents the equivalent access but in a one-dimensional array. Here, m is the number of columns in the matrix, i is the current row, and j is the current column.

This method of storing matrix data in a linear array and accessing it as if it were a two-dimensional array is common in C because C does not natively support true multi-dimensional arrays, only arrays of arrays. This approach also tends to be more cache-friendly, as it stores the matrix elements in contiguous memory locations, which can improve performance due to better utilization of the CPU cache. This is how the matrix library has been implemented here.


# Matrix Library

This matrix library provides a set of functions for creating, manipulating, and comparing matrices in C. Below is the documentation for the currently implemented functions.

## Functions

### `matrix_new`
- **Description**: Allocates and returns a new matrix with the specified number of rows and columns. Each element's size is determined by `element_size`.
- **Parameters**:
  - `num_rows`: Number of rows in the matrix.
  - `num_cols`: Number of columns in the matrix.
  - `element_size`: Size of each element in the matrix.
- **Returns**: Pointer to the newly created matrix.

### `matrix_rand`
- **Description**: Creates a matrix with random elements between a specified range.
- **Parameters**:
  - `num_rows`: Number of rows in the matrix.
  - `num_cols`: Number of columns in the matrix.
  - `min`: Minimum value for random elements.
  - `max`: Maximum value for random elements.
  - `element_size`: Size of each element in the matrix.
- **Returns**: Pointer to the matrix filled with random elements.

### `matrix_sqr`
- **Description**: Allocates and returns a new square matrix of the given size.
- **Parameters**:
  - `size`: Number of rows and columns in the square matrix.
  - `element_size`: Size of each element in the matrix.
- **Returns**: Pointer to the new square matrix.

### `matrix_eye`
- **Description**: Creates an identity matrix of a specified size.
- **Parameters**:
  - `size`: Size of the identity matrix (number of rows and columns).
  - `element_size`: Size of each element in the matrix.
  - `identity_element`: Pointer to the value to be used as the identity element.
- **Returns**: Pointer to the identity matrix.

### `matrix_free`
- **Description**: Frees the memory allocated for a matrix.
- **Parameters**:
  - `mat`: Pointer to the matrix to be freed.

### `matrix_print`
- **Description**: Prints the matrix to the standard output.
- **Parameters**:
  - `matrix`: Pointer to the matrix to be printed.

### `matrix_printf`
- **Description**: Prints the matrix to the standard output using a specified format for elements.
- **Parameters**:
  - `matrix`: Pointer to the matrix to be printed.
  - `d_fmt`: Format string for printing each element.

### `matrix_eqdim`
- **Description**: Checks if two matrices have the same dimensions.
- **Parameters**:
  - `m1`: Pointer to the first matrix.
  - `m2`: Pointer to the second matrix.
- **Returns**: 1 if dimensions are equal, 0 otherwise.

### `matrix_eq`
- **Description**: Compares two matrices for equality within a specified tolerance.
- **Parameters**:
  - `m1`: Pointer to the first matrix.
  - `m2`: Pointer to the second matrix.
  - `tolerance`: The tolerance level for element-wise comparison.
- **Returns**: 1 if matrices are equal within the given tolerance, 0 otherwise.

## Usage

To use this library, include `matrix.h` in your C program and link against the implementation of these functions.

