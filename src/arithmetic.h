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
 * @brief Adds two WORD-sized integers with a carry.
 * @details Computes the sum of x and y, adds a carry k, and stores the result in ptrQ. The overflow (if any) is stored in ptrR.
 * @param x A WORD-sized integer to be added.
 * @param y A WORD-sized integer to be added.
 * @param k A WORD-sized carry value.
 * @param ptrQ A pointer to a WORD where the sum is to be stored.
 * @param ptrR A pointer to a WORD where the overflow is to be stored.
 * @pre ptrQ and ptrR must be valid non-null pointers. x, y, and k must be within the bounds of the WORD type.
 * @post *ptrQ contains the sum of x, y, and k. *ptrR contains the carry-out value.
 * @note WORD is assumed to be a type alias for an integral type with a fixed size.
 */
void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR);

/**
 * @brief Adds two arbitrary-sized binary integers.
 * @details Sums the binary integers pointed to by pptrX and pptrY and stores the result in the location pointed to by pptrZ.
 * @param pptrX A pointer to a pointer to a BINT representing the first addend.
 * @param pptrY A pointer to a pointer to a BINT representing the second addend.
 * @param pptrZ A pointer to a pointer to a BINT where the result is to be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be initialized to store the result.
 * @post *pptrZ contains the sum of *pptrX and *pptrY.
 * @note It's assumed that the BINT structure and associated functions properly manage memory and handle arithmetic.
 */
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Adds two binary integers.
 * @details Wrapper function that calls the core add function to sum two BINTs pointed to by pptrX and pptrY and stores the result at pptrZ.
 * @param pptrX A pointer to a pointer to a BINT representing the first operand.
 * @param pptrY A pointer to a pointer to a BINT representing the second operand.
 * @param pptrZ A pointer to a pointer to a BINT where the result should be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be properly allocated to store the result.
 * @post The result of addition is stored in the location pointed to by pptrZ.
 * @note This function may call other helper functions to manage BINT arithmetic and memory.
 */
void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Subtracts two WORD-sized integers with borrow handling.
 * @details Computes the difference between x and y, considering an initial borrow, and stores the result in ptrQ. The borrow-out is stored in ptrR.
 * @param x A WORD-sized integer from which y will be subtracted.
 * @param y A WORD-sized integer to be subtracted from x.
 * @param ptrQ A pointer to a WORD to store the result of the subtraction.
 * @param ptrR A pointer to a WORD to store the borrow-out value.
 * @pre ptrQ and ptrR must be valid non-null pointers.
 * @post *ptrQ contains the result of the subtraction. *ptrR contains the borrow-out value.
 * @note WORD is assumed to be a type alias for an integral type with a fixed size.
 */
void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR);

/**
 * @brief Subtracts two arbitrary-sized binary integers.
 * @details Computes the difference between the binary integers pointed to by pptrX and pptrY and stores the result in the location pointed to by pptrZ.
 * @param pptrX A pointer to a pointer to a BINT representing the minuend.
 * @param pptrY A pointer to a pointer to a BINT representing the subtrahend.
 * @param pptrZ A pointer to a pointer to a BINT where the result is to be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be initialized to store the result.
 * @post *pptrZ contains the difference of *pptrX and *pptrY.
 * @note Assumes proper BINT structure and memory management.
 */
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Subtracts two binary integers.
 * @details Wrapper function that calls the core subtract function to compute the difference between two BINTs pointed to by pptrX and pptrY, storing the result at pptrZ.
 * @param pptrX A pointer to a pointer to a BINT representing the first operand.
 * @param pptrY A pointer to a pointer to a BINT representing the second operand.
 * @param pptrZ A pointer to a pointer to a BINT where the result should be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be properly allocated to store the result.
 * @post The result of subtraction is stored in the location pointed to by pptrZ.
 * @note This function may utilize other helper functions for BINT arithmetic and memory management.
 */
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

#endif // _ARITHMETIC_H