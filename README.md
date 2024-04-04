

The representation data[i][j] <=> array[i * m + j] is typical of the C programming language approach for handling two-dimensional arrays (matrices) in a one-dimensional array format. In this representation:

data[i][j] is how you would access the element at row i and column j in a two-dimensional array (matrix).
array[i * m + j] represents the equivalent access but in a one-dimensional array. Here, m is the number of columns in the matrix, i is the current row, and j is the current column.

This method of storing matrix data in a linear array and accessing it as if it were a two-dimensional array is common in C because C does not natively support true multi-dimensional arrays, only arrays of arrays. This approach also tends to be more cache-friendly, as it stores the matrix elements in contiguous memory locations, which can improve performance due to better utilization of the CPU cache. This is how the matrix library has been implemented here.

# Matrix Library

This matrix library in C is designed for efficient handling and operations of matrices, storing them in a one-dimensional array while giving the illusion of a two-dimensional matrix. This approach, `data[i][j] <=> array[i * m + j]`, is typical in C, promoting cache-friendliness and performance. 


## Installation

- Clone/download the repository.
- Include the `matrix.h` in your C program.
- Compile your program with `matrix.c`.  For example `gcc -o your_program your_program.c src/matrix.c`.

## Features
- Matrix creation, manipulation, and comparison.
- Support for square, identity, and random matrices.
- Matrix arithmetic operations including addition, multiplication, and transposition.
- Functions for checking matrix dimensions and equality with tolerance.

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

### `matrix_transpose`
- **Description**: Transposes the given matrix, swapping its rows and columns.
- **Parameters**:
  - `mat`: Pointer to the matrix to be transposed.
- **Returns**: Nothing.  The function transposes the matrix in place, modifying the original matrix.


### `matrix_stackv`
- **Description**: Stacks two matrices vertically, assuming they have the same number of columns.
- **Parameters**:
  - `mat1`: Pointer to the first matrix.
  - `mat2`: Pointer to the second matrix.
- **Returns**: A new matrix that is the result of stacking `mat1` over `mat2`.


### `matrix_stackh`
- **Description**: Stacks two matrices horizontally, assuming they have the same number of rows.
- **Parameters**:
  - `mat1`: Pointer to the first matrix.
  - `mat2`: Pointer to the second matrix.
- **Returns**: A new matrix that is the result of stacking `mat1` next to `mat2` on the right side.

### Example

```
#include "matrix.h"

int main() {
  matrix *mat = matrix_new(3, 3, sizeof(double));
  // Perform operations...
  matrix_free(mat);
  return 0;
}

```
### Memory Leak Check

Feel free to run the below valgrind command to check for any memory leaks

```
valgrind --leak-check=full ./main

```

# Pydantic Models Documentation

## Overview

This document outlines the usage of Pydantic models within the application to ensure data integrity and structured data flow. These models, namely `EventModel`, `OwnerModel`, and funding request-related classes, are essential for the application's operations, handling different types of data from various API calls.

## EventModel

The `EventModel` encapsulates data related to system events, reflecting the structure of API response bodies. It contains information on transactions, facilities, and ownership details.

### Key Fields:

- `success`: A boolean indicating the outcome of the API call.
- `result`: An optional field containing event details; it's optional to handle scenarios where an event might not return data.

### Submodels of EventModel:

- `TransactionEvent`: Details specific to a transaction.
- `Owner`: Financial information regarding the owner, vital for computations.
- `Facility`: Information on the credit facility involved in the event.

## OwnerModel

The `OwnerModel` provides a structured format for the API response concerning owner information. It includes critical financial data necessary for assessing an entity's fiscal health.

### Key Fields:

- `success`: Indicates the success of the API call.
- `owner`: Optionally contains the owner's detailed financial information.

### Submodels of OwnerModel:

- `AccruedInterest`: The accumulated interest over time.
- `AvailableCredit`: The current available credit for the owner.

### Rationale for Optional Fields:

Fields are marked optional to handle cases where the API response may not include all the data, providing robustness in data handling.

## FundingRequest Related Classes

Introduced are classes related to funding requests to handle automated booking of funding trades.

### Enums:

- `AmortizationTypeEnum`: Defines the types of loan amortization schedules.
- `CurrencyEnum`: Enumerates the supported currencies, primarily `USD`.
- `RollConventionEnum`: Specifies conventions for repayment dates, such as `FOLLOWING` and `MODIFIED_FOLLOWING`.
- `BusinessDayConventionEnum`: Defines the business day adjustment logic.
- `ActionEnum`: Indicates the type of action associated with a funding request, e.g., `CREATE`, `MODIFY`, `TERMINATE`.

### Models:

- `PaymentSchedule`: Outlines the repayment schedule details.
- `Terms`: Details the terms of the loan, including the payment schedule.
- `FundingRequest`: Represents an individual funding request, containing terms and unique identifiers.
- `FundingResponse`: A wrapper for a list of `FundingRequest` objects, representing multiple requests or the batch process.

### Rationale for Funding Request Fields:

The fields within `FundingRequest` are designed to precisely reflect the structure required by the API. Optional fields and strict typing via enums promote data validation and prevent the processing of invalid or unexpected data.

## Conclusion

The `EventModel`, `OwnerModel`, and funding request-related classes maintain the application's data processing integrity. They ensure that our system can reliably handle complex financial data, perform necessary computations, and make informed decisions based on the structured data provided by various API endpoints.

