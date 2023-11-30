#include "config.h"
#include "utils.h"

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

/**
 * @brief Computes the bitwise NOT of a binary integer.
 * @details This function takes a pointer to a pointer to a binary integer (BINT) and computes its bitwise NOT operation, storing the result in the location pointed to by ptrbint_dst.
 * @param ptrbint_dst A pointer to a pointer to a BINT where the result should be stored.
 * @param pptrBint_src A pointer to a pointer to a BINT that is the source for the NOT operation.
 * @pre pptrBint_src must point to a valid BINT object. ptrbint_dst must be initialized and able to store the result.
 * @post The result of the bitwise NOT operation is stored in the location pointed to by ptrbint_dst.
 * @note This function assumes that the BINT data structure is properly defined and that the necessary memory management is handled outside of the function.
 */
void NOT_BINT(BINT** ptrbint_dst, BINT** pptrBint_src);

/**
 * @brief Computes the bitwise AND of two binary integers.
 * @details This function takes pointers to two BINTs and computes the bitwise AND, storing the result in the location pointed to by pptrZ.
 * @param ptrX A pointer to a BINT representing the first operand.
 * @param ptrY A pointer to a BINT representing the second operand.
 * @param pptrZ A pointer to a pointer to a BINT where the result should be stored.
 * @pre ptrX and ptrY must point to valid BINT objects. pptrZ must be initialized to store the result.
 * @post The result of the bitwise AND operation is stored in the location pointed to by pptrZ.
 * @note It is assumed that proper memory allocation for the result has been done before the function is called.
 */
void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);

/**
 * @brief Computes the bitwise OR of two binary integers.
 * @details This function takes pointers to two BINTs and computes the bitwise OR, storing the result in the location pointed to by pptrZ.
 * @param ptrX A pointer to a BINT representing the first operand.
 * @param ptrY A pointer to a BINT representing the second operand.
 * @param pptrZ A pointer to a pointer to a BINT where the result should be stored.
 * @pre ptrX and ptrY must point to valid BINT objects. pptrZ must be initialized and able to store the result.
 * @post The result of the bitwise OR operation is stored in the location pointed to by pptrZ.
 * @note Ensure that the BINT data structure supports the operation and that memory for the result is allocated beforehand.
 */
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);

/**
 * @brief Computes the bitwise XOR of two binary integers.
 * @details This function takes pointers to two BINTs and computes the bitwise XOR, storing the result in the location pointed to by pptrZ.
 * @param ptrX A pointer to a BINT representing the first operand.
 * @param ptrY A pointer to a BINT representing the second operand.
 * @param pptrZ A pointer to a pointer to a BINT where the result should be stored.
 * @pre ptrX and ptrY must point to valid BINT objects. pptrZ must be initialized to store the result.
 * @post The result of the bitwise XOR operation is stored in the location pointed to by pptrZ.
 * @note The function assumes that memory allocation for the result has been managed outside of the function call.
 */
void XOR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);

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

/**
 * @brief Multiplies two WORD values and stores the result in a BINT object.
 * @details Performs multiplication of valX and valY, and stores the result in the BINT object pointed to by pptrZ.
 * @author Your Name
 * @date Today's Date
 * @param valX The first WORD value to be multiplied.
 * @param valY The second WORD value to be multiplied.
 * @param pptrZ Double pointer to the BINT object where the result will be stored.
 * @pre pptrZ must point to a valid BINT object.
 * @post *pptrZ contains the result of valX * valY.
 */
void mul_xyz(WORD valX, WORD valY, BINT** pptrZ);

/**
 * @brief Core multiplication function using the textbook algorithm.
 * @details Multiplies the values in BINT objects pointed to by pptrX and pptrY, stores the result in pptrZ using the textbook multiplication algorithm.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the first BINT operand.
 * @param pptrY Double pointer to the second BINT operand.
 * @param pptrZ Double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Test function for improved textbook multiplication.
 * @details Similar to mul_core_TxtBk_xyz, but for testing an improved textbook algorithm on BINT objects.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the first BINT operand.
 * @param pptrY Double pointer to the second BINT operand.
 * @param pptrZ Double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void mul_core_ImpTxtBk_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Core multiplication function using the improved textbook algorithm.
 * @details Multiplies BINT objects pointed to by pptrX and pptrY, stores the result in pptrZ using an improved textbook algorithm for efficiency.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the first BINT operand.
 * @param pptrY Double pointer to the second BINT operand.
 * @param pptrZ Double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Test function for Karatsuba multiplication.
 * @details Tests the Karatsuba multiplication algorithm with BINT objects, similar in purpose to mul_core_ImpTxtBk_test but for Karatsuba algorithm.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the first BINT operand.
 * @param pptrY Double pointer to the second BINT operand.
 * @param pptrZ Double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void mul_core_Krtsb_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Karatsuba multiplication test function with a flag parameter.
 * @details Performs Karatsuba multiplication on BINT objects with an additional parameter to toggle specific algorithmic behavior based on the flag.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the first BINT operand.
 * @param pptrY Double pointer to the second BINT operand.
 * @param pptrZ Double pointer to the BINT object to store the result.
 * @param flag Integer flag to modify algorithmic behavior.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void Krtsb_FLAG_Test(BINT** pptrX, BINT** pptrY, BINT** pptrZ, int flag);

/**
 * @brief Core multiplication function using the Karatsuba algorithm.
 * @details Multiplies BINT objects pointed to by pptrX and pptrY, stores the result in pptrZ using the Karatsuba multiplication algorithm for efficiency.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the first BINT operand.
 * @param pptrY Double pointer to the second BINT operand.
 * @param pptrZ Double pointer to the BINT object to store the result.
 * @pre pptrX and pptrY must point to valid BINT objects; pptrZ must be initialized.
 * @post *pptrZ contains the result of the multiplication.
 */
void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Squares a WORD value and stores the result in a BINT object.
 * @details Performs the squaring of valX and stores the result in the BINT object pointed to by pptrZ.
 * @author Your Name
 * @date Today's Date
 * @param valX The WORD value to be squared.
 * @param pptrZ Double pointer to the BINT object where the result will be stored.
 * @pre pptrZ must point to a valid BINT object.
 * @post *pptrZ contains the result of valX squared.
 */
void squ_core(WORD valX,BINT** pptrZ);

/**
 * @brief Squares a BINT object using the textbook algorithm.
 * @details Squares the value in the BINT object pointed to by pptrX and stores the result in pptrZ using the textbook squaring algorithm.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the BINT object to be squared.
 * @param pptrZ Double pointer to the BINT object where the result will be stored.
 * @pre pptrX must point to a valid BINT object; pptrZ must be initialized.
 * @post *pptrZ contains the result of squaring *pptrX.
 */
void SQU_Txtbk_xz(BINT** pptrX, BINT** pptrZ);

/**
 * @brief Squares a BINT object using the Karatsuba algorithm.
 * @details Squares the value in the BINT object pointed to by pptrX and stores the result in pptrZ using the Karatsuba squaring algorithm for efficiency.
 * @author Your Name
 * @date Today's Date
 * @param pptrX Double pointer to the BINT object to be squared.
 * @param pptrZ Double pointer to the BINT object where the result will be stored.
 * @pre pptrX must point to a valid BINT object; pptrZ must be initialized.
 * @post *pptrZ contains the result of squaring *pptrX.
 */
void SQU_Krtsb_xz(BINT** pptrX, BINT** pptrZ);

/**
 * @brief Tests the binary long division algorithm.
 * @details Performs division of the BINT objects pointed to by pptrDividend and pptrDivisor and stores the quotient and remainder in pptrQ and pptrR, respectively, as a test for the binary long division algorithm.
 * @author Your Name
 * @date Today's Date
 * @param pptrDividend Double pointer to the BINT dividend.
 * @param pptrDivisor Double pointer to the BINT divisor.
 * @param pptrQ Double pointer to the BINT object to store the quotient.
 * @param pptrR Double pointer to the BINT object to store the remainder.
 * @pre pptrDividend and pptrDivisor must point to valid BINT objects; pptrQ and pptrR must be initialized.
 * @post *pptrQ and *pptrR contain the quotient and remainder of the division, respectively.
 */
void DIV_Bianry_Long_Test(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

/**
 * @brief Performs binary long division.
 * @details Divides the BINT objects pointed to by pptrDividend and pptrDivisor and stores the quotient and remainder in pptrQ and pptrR, respectively, using the binary long division algorithm.
 * @author Your Name
 * @date Today's Date
 * @param pptrDividend Double pointer to the BINT dividend.
 * @param pptrDivisor Double pointer to the BINT divisor.
 * @param pptrQ Double pointer to the BINT object to store the quotient.
 * @param pptrR Double pointer to the BINT object to store the remainder.
 * @pre pptrDividend and pptrDivisor must point to valid BINT objects; pptrQ and pptrR must be initialized.
 * @post *pptrQ and *pptrR contain the quotient and remainder of the division, respectively.
 */
void DIV_Bianry_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

/**
 * @brief Multiplies two BINT objects using a left-to-right binary method, storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the pointer of the first BINT operand.
 * @param ptrY A pointer to the pointer of the second BINT operand.
 * @param ptrZ A pointer to the pointer where the multiplication result will be stored.
 * @note This method multiplies from the most significant bit to the least significant bit of the multiplier.
 */

void mul_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
/**
 * @brief Adds two BINT objects using a left-to-right binary method, storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the pointer of the first BINT addend.
 * @param ptrY A pointer to the pointer of the second BINT addend.
 * @param ptrZ A pointer to the pointer where the addition result will be stored.
 * @note This method adds the BINT objects starting from the most significant bit and moving to the least significant bit.
 */

void add_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
/**
 * @brief Multiplies two BINT objects using a right-to-left binary method, storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the pointer of the first BINT operand.
 * @param ptrY A pointer to the pointer of the second BINT operand.
 * @param ptrZ A pointer to the pointer where the multiplication result will be stored.
 * @note This method multiplies from the least significant bit to the most significant bit of the multiplier.
 */

void mul_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ);

/**
 * @brief Adds two BINT objects using a right-to-left binary method, storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the pointer of the first BINT addend.
 * @param ptrY A pointer to the pointer of the second BINT addend.
 * @param ptrZ A pointer to the pointer where the addition result will be stored.
 * @note This method adds the BINT objects starting from the least significant bit and moving to the most significant bit.
 */
void add_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ);

/**
 * @brief Calculates the Montgomery exponentiation of two BINT objects and stores the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the pointer of the base BINT operand.
 * @param ptrY A pointer to the pointer of the exponent BINT operand.
 * @param ptrZ A pointer to the pointer where the exponentiation result will be stored.
 * @note This method is used for efficient modular exponentiation, especially in cryptographic applications.
 */
void exp_Mongomery(BINT** ptrX, BINT** ptrY,BINT** ptrZ);

/**
 * @brief Performs modular exponentiation using the Montgomery method on three BINT objects and stores the result in a fourth BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the pointer of the base BINT operand.
 * @param ptrY A pointer to the pointer of the exponent BINT operand.
 * @param ptrM A pointer to the pointer of the modulus BINT operand.
 * @param ptrZ A pointer to the pointer where the modular exponentiation result will be stored.
 * @note This function is suitable for high-precision arithmetic, such as cryptographic operations involving large numbers.
 */
void Mod_Exp_Mongo(BINT** ptrX,BINT** ptrY,BINT** ptrM,BINT** ptrZ);

/**
 * @brief Performs the Barrett reduction on three BINT objects and stores the result in a fourth BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the pointer of the BINT object representing the numerator in the reduction.
 * @param ptrY A pointer to the pointer of the BINT object representing the modulus in the reduction.
 * @param ptrZ A pointer to the pointer where the Barrett reduction result will be stored.
 * @note This function uses the Barrett reduction algorithm to compute the reduction of large integers, which is more efficient than traditional methods.
 */
void Barrett_Reduction(BINT** ptrX,BINT** ptrY,BINT** ptrZ);

#endif // ARITHMETIC_H