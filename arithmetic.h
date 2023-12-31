/**
 * @file arithmetic.h
 * @brief Header file for large integer arithmetic operations.
 *
 * This file declares the functions for large integer arithmetic operations,
 * leveraging the basic functionality provided by utils.h and utils.c. It
 * includes more complex operations such as multiplication, division, etc.
 */

#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H

#include "utils.h"

/**
 * @brief Computes the bitwise OR of two big integers.
 * @details This function takes pointers to two BINTs and computes the bitwise OR, storing the result in the location pointed to by pptrZ.
 * @param pptrX A double pointer to a BINT representing the first operand.
 * @param pptrY A doule pointer to a BINT representing the second operand.
 * @param pptrZ A double pointer to a BINT where the result should be stored.
 * @pre pptrX and pptrY must point to point to valid BINT objects. pptrZ must be initialized and able to store the result.
 * @post The result of the bitwise OR operation is stored in the location pointed to by pptrZ.
 * @note Ensure that the BINT data structure supports the operation and that memory for the result is allocated beforehand.
 */
void OR_BINT(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

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
 * @param pptrX A double pointer to a BINT representing the first addend.
 * @param pptrY A double pointer to a BINT representing the second addend.
 * @param pptrZ A double pointer to a BINT where the result is to be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be initialized to store the result.
 * @post *pptrZ contains the sum of *pptrX and *pptrY.
 * @note It's assumed that the BINT structure and associated functions properly manage memory and handle arithmetic.
 */
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Adds two binary integers.
 * @details Wrapper function that calls the core add function to sum two BINTs pointed to by pptrX and pptrY and stores the result at pptrZ.
 * @param pptrX A double pointer to a BINT representing the first operand.
 * @param pptrY A double pointer to a BINT representing the second operand.
 * @param pptrZ A double pointer to a BINT where the result should be stored.
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
 * @param ptrQ A pointer to a WORD to store the borrow-out value.
 * @param ptrR A pointer to a WORD to store the result of the subtraction.
 * @pre ptrQ and ptrR must be valid non-null pointers.
 * @post *ptrQ contains the result of the subtraction. *ptrR contains the borrow-out value.
 * @note WORD is assumed to be a type alias for an integral type with a fixed size.
 */
void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR);

/**
 * @brief Subtracts two arbitrary-sized binary integers.
 * @details Computes the difference between the binary integers pointed to by pptrX and pptrY and stores the result in the location pointed to by pptrZ.
 * @param pptrX A double pointer to a BINT representing the minuend.
 * @param pptrY A double pointer to a BINT representing the subtrahend.
 * @param pptrZ A double pointer to a BINT where the result is to be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be initialized to store the result.
 * @post *pptrZ contains the difference of *pptrX and *pptrY.
 * @note Assumes proper BINT structure and memory management.
 */
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Subtracts two binary integers.
 * @details Wrapper function that calls the core subtract function to compute the difference between two BINTs pointed to by pptrX and pptrY, storing the result at pptrZ.
 * @param pptrX A double pointer to a BINT representing the first operand.
 * @param pptrY A double pointer to a BINT representing the second operand.
 * @param pptrZ A double pointer to a BINT where the result should be stored.
 * @pre pptrX and pptrY must point to valid BINT objects. pptrZ must be properly allocated to store the result.
 * @post The result of subtraction is stored in the location pointed to by pptrZ.
 * @note This function may utilize other helper functions for BINT arithmetic and memory management.
 */
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Multiplies two WORD values and stores the result in a BINT object.
 * @details Performs multiplication of valX and valY, and stores the result in the BINT object pointed to by pptrZ.
 * @param valX The first WORD value to be multiplied.
 * @param valY The second WORD value to be multiplied.
 * @param pptrZ A double pointer to the BINT object where the result will be stored.
 * @pre pptrZ must point to a valid BINT object.
 * @post *pptrZ contains the result of valX * valY.
 */
void mul_xyz(WORD valX, WORD valY, BINT** pptrZ);

/**
 * @brief Core multiplication function using the textbook algorithm.
 * @details Multiplies the values in BINT objects pointed to by pptrX and pptrY, stores the result in pptrZ using the textbook multiplication algorithm.
 * @param pptrX A double pointer to the first BINT operand.
 * @param pptrY A double pointer to the second BINT operand.
 * @param pptrZ A double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Core multiplication function using the improved textbook algorithm.
 * @details Multiplies BINT objects pointed to by pptrX and pptrY, stores the result in pptrZ using an improved textbook algorithm for efficiency.
 * @param pptrX A double pointer to the first BINT operand.
 * @param pptrY A double pointer to the second BINT operand.
 * @param pptrZ A double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Core multiplication function using the Karatsuba algorithm.
 * @details Multiplies BINT objects pointed to by pptrX and pptrY, stores the result in pptrZ using the Karatsuba multiplication algorithm for efficiency.
 * @param pptrX A double pointer to the first BINT operand.
 * @param pptrY A double pointer to the second BINT operand.
 * @param pptrZ A double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Squares a WORD value and stores the result in a BINT object.
 * @details Performs the squaring of valX and stores the result in the BINT object pointed to by pptrZ.
 * @param valX The WORD value to be squared.
 * @param pptrZ A double pointer to the BINT object where the result will be stored.
 * @pre pptrZ must point to a valid BINT object.
 * @post *pptrZ contains the result of valX squared.
 */
void squ_core(WORD valX, BINT** pptrZ);

/**
 * @brief Squares a BINT object using the textbook algorithm.
 * @details Squares the value in the BINT object pointed to by pptrX and stores the result in pptrZ using the textbook squaring algorithm.
 * @param pptrX A double pointer to the BINT object to be squared.
 * @param pptrZ A double pointer to the BINT object where the result will be stored.
 * @pre pptrX must point to a valid BINT object; pptrZ must be initialized.
 * @post *pptrZ contains the result of squaring *pptrX.
 */
void SQU_TxtBk_xz(BINT** pptrX, BINT** pptrZ);

/**
 * @brief Squares a BINT object using the Karatsuba algorithm.
 * @details Squares the value in the BINT object pointed to by pptrX and stores the result in pptrZ using the Karatsuba squaring algorithm for efficiency.
 * @param pptrX A double pointer to the BINT object to be squared.
 * @param pptrZ A double pointer to the BINT object where the result will be stored.
 * @pre pptrX must point to a valid BINT object; pptrZ must be initialized.
 * @post *pptrZ contains the result of squaring *pptrX.
 */
void SQU_Krtsb_xz(BINT** pptrX, BINT** pptrZ);

/**
 * @brief Performs binary long division.
 * @details Divides the BINT objects pointed to by pptrDividend and pptrDivisor and stores the quotient and remainder in pptrQ and pptrR, respectively, using the binary long division algorithm.
 * @param pptrDividend A double pointer to the BINT dividend.
 * @param pptrDivisor A double pointer to the BINT divisor.
 * @param pptrQ A double pointer to the BINT object to store the quotient.
 * @param pptrR A double pointer to the BINT object to store the remainder.
 * @pre pptrDividend and pptrDivisor must point to valid BINT objects; pptrQ and pptrR must be initialized.
 * @post *pptrQ and *pptrR contain the quotient and remainder of the division, respectively.
 */
void DIV_Binary_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

WORD quotient(WORD dividend1, WORD dividend0, WORD divisor);

/**
 * @brief Performs long division.
 * @details Divides the BINT objects pointed to by pptrDividend and pptrDivisor and stores the quotient and remainder in pptrQ and pptrR, respectively, using the binary long division algorithm.
 * @param pptrDividend A double pointer to the BINT dividend.
 * @param pptrDivisor A double pointer to the BINT divisor.
 * @param pptrQ A double pointer to the BINT object to store the quotient.
 * @param pptrR A double pointer to the BINT object to store the remainder.
 * @pre pptrDividend and pptrDivisor must point to valid BINT objects; pptrQ and pptrR must be initialized. 
 * @post *pptrQ and *pptrR contain the quotient and remainder of the division, respectively.
 * @note (*pptrDividend)->wordlen = (*pptrDivisor) + 1;
 */
void DIV_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

/**
 * @brief Performs left-to-right modular exponentiation on three BINT objects and stores the result in a fourth BINT object.
 * @details This function implements the left-to-right binary method for modular exponentiation. It iteratively computes the result 
 *          of raising the base (pointed to by pptrX) to the power of the exponent (pointed to by pptrY), modulo the modulus 
 *          (pointed to by ptrMod). The result is stored in the BINT object pointed to by pptrZ. This method iterates through the 
 *          bits of the exponent starting from the most significant bit towards the least significant bit.
 * @param pptrX A double pointer of the base BINT operand.
 * @param pptrY A double pointer of the exponent BINT operand.
 * @param pptrZ A double pointer where the modular exponentiation result will be stored.
 * @param ptrMod A double pointer of the modulus BINT operand.
 * @note This function is suitable for high-precision arithmetic and is often used in cryptographic applications involving large numbers.
 */
void EXP_MOD_L2R(BINT** pptrX, BINT** pptrY, BINT** pptrZ, BINT* ptrMod);

/**
 * @brief Performs right-to-left modular exponentiation on three BINT objects and stores the result in a fourth BINT object.
 * @details This function implements the right-to-left binary method for modular exponentiation. It computes the result of raising 
 *          the base (pointed to by pptrX) to the power of the exponent (pointed to by pptrY), modulo the modulus (pointed to by 
 *          ptrMod), and stores the result in the BINT object pointed to by pptrZ. This method processes the bits of the exponent 
 *          starting from the least significant bit towards the most significant bit, which can be more efficient in certain scenarios.
 * @param pptrX A double pointer of the base BINT operand.
 * @param pptrY A double pointer of the exponent BINT operand.
 * @param pptrZ A double pointer where the modular exponentiation result will be stored.
 * @param ptrMod A pointer of the modulus BINT operand.
 * @note This function is also suitable for high-precision arithmetic, including cryptographic applications that require large number operations.
 */
void EXP_MOD_R2L(BINT** pptrX, BINT** pptrY, BINT** pptrZ, BINT* ptrMod);

/**
 * @brief Performs modular exponentiation using the Montgomery method on three BINT objects and stores the result in a fourth BINT object.
 * @param pptrX A double pointer of the base BINT operand.
 * @param pptrY A double pointer of the exponent BINT operand.
 * @param pptrZ A double pointer where the modular exponentiation result will be stored.
 * @param ptrMod A pointer of the modulus BINT operand.
 * @note This function is suitable for high-precision arithmetic, such as cryptographic operations involving large numbers.
 */
void EXP_MOD_Montgomery(BINT** pptrX, BINT** pptrY, BINT** pptrZ, BINT* ptrMod);

/**
 * @brief Test Function for Barrett Reduction (Not pre-computation)
 * @details This function is designed to test the correctness and functionality of the Barrett Reduction algorithm. 
 *          It takes pointers to BINT (Binary Integer) objects and verifies the accuracy of the Barrett Reduction process 
 *          by comparing the algorithm's output against expected results.
 * @param pptrX A double pointer to the BINT object representing the dividend.
 * @param pptrN A double pointer to the BINT object representing the modulus.
 * @param pptrR A double pointer to the BINT object where the result of the Barrett Reduction will be stored.
 * @pre pptrX, pptrN, and pptrR must point to initialized and valid BINT objects. The Barrett Reduction algorithm must be implemented correctly.
 * @post The function will validate the correctness of the Barrett Reduction algorithm by storing the result in pptrR and comparing it against expected outcomes.
 * @note This test function is crucial for ensuring the reliability of the Barrett Reduction algorithm, especially in cryptographic applications where accuracy is paramount.
 * @warning Incorrect initialization of BINT objects or errors in the Barrett Reduction implementation can lead to misleading test results.
 */
void Barrett_Reduction_TEST(BINT** pptrX, BINT** pptrN, BINT** pptrR);

/**
 * @brief Implements Barrett Reduction (Pre-computation)
 * @details This function performs the Barrett Reduction algorithm, an efficient method for modular reduction of large numbers. 
 *          It takes pointers to BINT objects representing the dividend, modulus, and a precomputed value, and calculates the result of the reduction.
 * @param pptrX A double pointer to the BINT object representing the dividend.
 * @param pptrN A double pointer to the BINT object representing the modulus.
 * @param pptrR A double pointer to the BINT object where the result will be stored.
 * @param pptrPreT A double pointer to the BINT object representing the precomputed value used in Barrett Reduction.
 * @pre pptrX, pptrN, pptrR, and pptrPreT must point to initialized and valid BINT objects. The precomputed value in pptrPreT should be correctly calculated for the given modulus.
 * @post The result of the Barrett Reduction will be stored in pptrR.
 * @note Barrett Reduction is particularly useful in contexts requiring frequent modular reductions, such as in cryptographic algorithms.
 * @warning The accuracy of the result is highly dependent on the correct initialization and computation of the precomputed value in pptrPreT.
 */
void Barrett_Reduction(BINT** pptrX, BINT** pptrN, BINT** pptrR, BINT** pptrPreT);


/**
 * @brief Executes the Extended Euclidean Algorithm.
 * @details Implements the Extended Euclidean Algorithm to compute the coefficients of Bézout's identity and the greatest common divisor (GCD) of two numbers.
 *          This algorithm finds integers x and y (Bézout coefficients) such that ax + by = gcd(a, b), where a and b are the input numbers represented as hexadecimal integers (BINTs).
 * @param pptrX A double pointer to the first BINT, representing 'a'.
 * @param pptrY A double pointer to the second BINT, representing 'b'.
 * @param pptrS A double pointer to store the Bézout coefficient corresponding to 'a'.
 * @param pptrT A double pointer to store the Bézout coefficient corresponding to 'b'.
 * @param pptrGCD A double pointer to store the calculated GCD of 'a' and 'b'.
 * @pre `pptrX` and `pptrY` must be pointers to pointer to initialized BINTs containing the input values. `pptrS`, `pptrT`, and `pptrGCD` should be pointers to point to BINT, ready to store the results.
 * @post Upon completion, `pptrS` and `pptrT` will hold the Bézout coefficients, and `pptrGCD` will contain the GCD of the inputs. The input BINTs (`pptrX` and `pptrY`) remain unaltered.
 * @note Assumes the existence of a well-defined BINT structure and correctly implemented binary arithmetic operations (e.g., addition, subtraction, multiplication).
 * @warning Proper initialization of all BINT pointers is crucial to prevent undefined behavior or memory access violations. Ensure `pptrX`, `pptrY`, `pptrS`, `pptrT`, and `pptrGCD` are correctly allocated and initialized before invocation.
 */
void EEA(BINT** pptrX, BINT** pptrY, BINT** pptrS, BINT** pptrT, BINT** pptrGCD);

#endif // _ARITHMETIC_H