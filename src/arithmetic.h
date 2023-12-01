/**
 * @mainpage Big Integer Library Documentation
 *
 * @section intro Introduction
 * The Big Integer Library provides an implementation for handling very large integers 
 * that are beyond the capacity of standard C integer types. This library is useful in 
 * scenarios such as cryptographic computations, high precision calculations, and 
 * anywhere standard integer types are insufficient due to their size limitations.
 *
 * This documentation covers the usage, functionalities, and implementation details of 
 * the Big Integer Library. It is intended for developers who need to integrate large 
 * integer operations into their applications.
 *
 * @section usage Usage
 * To use this library, include 'bigint.h' in your C source files. The library 
 * provides a struct `BINT` to represent large integers and a set of functions 
 * for operations on these integers.
 *
 * Example of initializing a big integer:
 * @code
 * BINT myBigInt;
 * bigIntInit(&myBigInt, 0); // Initialize the big integer to zero
 * @endcode
 *
 * Example of arithmetic operation:
 * @code
 * BINT a, b, sum;
 * bigIntInit(&a, 123456789);
 * bigIntInit(&b, 987654321);
 * bigIntAdd(&a, &b, &sum); // Perform addition: sum = a + b
 * @endcode
 *
 * @section notes Implementation Notes
 * - The library uses dynamic memory allocation for managing the array of words in `BINT`.
 * - Care should be taken to free the allocated memory using the appropriate function 
 *   provided by the library.
 * - The library handles both positive and negative numbers, represented by the `sign` 
 *   field in the `BINT` struct.
 * - Error handling is implemented using specific return codes from functions.
 *
 * @section development Development
 * The source code and development resources for the Big Integer Library can be found on GitHub:
 * [Big Integer Library on GitHub](https://github.com/yourusername/bigintlibrary)
 *
 * @author Your Name
 * @date YYYY-MM-DD
 */

#include "utils.h"

#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H

/**
 * @brief Adds two Single WORD-sized binary integers.
 * @details Sums the binary int egers pointed to by pptrX and pptrY and stores the result in the location pointed to by pptrZ.
 * @param pptrX A pointer to a pointer to a BINT representing the first addend.
 * @param pptrY A pointer to a pointer to a BINT representing the second addend.
 * @param pptrZ A pointer to a pointer to a BINT where the result is to be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be initialized to store the result.
 * @post *pptrZ contains the sum of *pptrX and *pptrY.
 * @note It's assumed that the BINT structure and associated functions properly manage memory and handle arithmetic.
 */
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

#endif // _ARITHMETIC_H