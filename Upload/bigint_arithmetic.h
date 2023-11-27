#include "config.h"
#include "bigint_utils.h"

#ifndef BIGINT_ARITHMETIC_H
#define BIGINT_ARITHMETIC_H

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
 * @brief Multiplies two WORD-sized integers and stores the result in a BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param valX The first WORD-sized integer operand.
 * @param valY The second WORD-sized integer operand.
 * @param pptrZ A pointer to the pointer of the BINT object where the result will be stored.
 * @note This function is utilized for single word multiplication that needs to be stored in a multi-word BINT object.
 */
void mul_xyz(WORD valX, WORD valY, BINT** pptrZ);
/**
 * @brief Core textbook algorithm for multiplying two BINT objects and storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the first BINT operand.
 * @param pptrY A pointer to the pointer of the second BINT operand.
 * @param pptrZ A pointer to the pointer where the multiplication result will be stored.
 * @note This function implements the standard long multiplication algorithm taught in textbooks.
 */
void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Tests the improved textbook multiplication algorithm on two BINT objects, storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the first BINT operand.
 * @param pptrY A pointer to the pointer of the second BINT operand.
 * @param pptrZ A pointer to the pointer where the multiplication result will be stored.
 * @note This function is used for testing optimizations in the textbook multiplication algorithm.
 */
void mul_core_ImpTxtBk_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
/**
 * @brief Core function for the improved textbook multiplication algorithm of two BINT objects, storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the first BINT operand.
 * @param pptrY A pointer to the pointer of the second BINT operand.
 * @param pptrZ A pointer to the pointer where the multiplication result will be stored.
 * @note This function is a more efficient version of the textbook multiplication algorithm, optimized for larger BINT objects.
 */
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);


/**
 * @brief Tests the Karatsuba multiplication algorithm on two BINT objects, storing the result in a third BINT object, with a flag for testing purposes.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the first BINT operand.
 * @param pptrY A pointer to the pointer of the second BINT operand.
 * @param pptrZ A pointer to the pointer where the multiplication result will be stored.
 * @param flag An integer flag used to control test behavior or algorithm variants.
 * @note This function is used to validate the Karatsuba algorithm's correctness and performance.
 */
void mul_core_Krtsb_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
/**
 * @brief Karatsuba multiplication test function with a flag parameter to control the behavior of the multiplication algorithm for two BINT objects.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the first BINT operand.
 * @param pptrY A pointer to the pointer of the second BINT operand.
 * @param pptrZ A pointer to the pointer where the multiplication result will be stored.
 * @param flag An integer to toggle specific behaviors or optimizations during the multiplication process.
 * @note This function allows for controlled experimentation with the Karatsuba algorithm.
 */
void Krtsb_FLAG_Test(BINT** pptrX, BINT** pptrY, BINT** pptrZ, int flag);
/**
 * @brief Core Karatsuba multiplication algorithm for two BINT objects, storing the result in a third BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the first BINT operand.
 * @param pptrY A pointer to the pointer of the second BINT operand.
 * @param pptrZ A pointer to the pointer where the multiplication result will be stored.
 * @note This function implements the Karatsuba algorithm for fast multiplication of large BINT objects.
 */
void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Performs squaring of a WORD-sized integer and stores the result in a BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param valX The WORD-sized integer to be squared.
 * @param pptrZ A pointer to the pointer of the BINT object where the result will be stored.
 * @note This function is optimized for squaring a single WORD-sized integer within a multi-word BINT context.
 */
void squ_core(WORD valX,BINT** pptrZ);
/**
 * @brief Textbook squaring algorithm for a BINT object, storing the result in another BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the BINT object to be squared.
 * @param pptrZ A pointer to the pointer where the squaring result will be stored.
 * @note Implements the standard algorithm for squaring, similar to the textbook multiplication algorithm.
 */
void SQU_Txtbk_xz(BINT** pptrX, BINT** pptrZ);
/**
 * @brief Karatsuba algorithm for squaring a BINT object, storing the result in another BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX A pointer to the pointer of the BINT object to be squared.
 * @param pptrZ A pointer to the pointer where the squaring result will be stored.
 * @note Utilizes the efficient Karatsuba multiplication algorithm adapted for the squaring operation.
 */
void SQU_Krtsb_xz(BINT** pptrX, BINT** pptrZ);

/**
 * @brief Tests the binary long division algorithm using BINT objects for the dividend and divisor, and stores the quotient and remainder.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrDividend A pointer to the pointer of the BINT object representing the dividend.
 * @param pptrDivisor A pointer to the pointer of the BINT object representing the divisor.
 * @param pptrQ A pointer to the pointer where the quotient BINT object will be stored.
 * @param pptrR A pointer to the pointer where the remainder BINT object will be stored.
 * @note This function is intended for testing the binary long division algorithm.
 */
void DIV_Bianry_Long_Test(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);
/**
 * @brief Performs binary long division on BINT objects for the dividend and divisor, and stores the quotient and remainder.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrDividend A pointer to the pointer of the BINT object representing the dividend.
 * @param pptrDivisor A pointer to the pointer of the BINT object representing the divisor.
 * @param pptrQ A pointer to the pointer where the quotient BINT object will be stored.
 * @param pptrR A pointer to the pointer where the remainder BINT object will be stored.
 * @note Implements the binary long division algorithm which is efficient for large BINT objects.
 */
void DIV_Bianry_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);
/**
 * @brief Performs general long division on BINT objects for the dividend and divisor, outputting the quotient and remainder.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrDividend A pointer to the pointer of the BINT object representing the dividend.
 * @param pptrDivisor A pointer to the pointer of the BINT object representing the divisor.
 * @param pptrQ A pointer to the pointer where the quotient BINT object will be stored.
 * @param pptrR A pointer to the pointer where the remainder BINT object will be stored.
 * @note This version of long division is optimized for a general case and can handle different forms of BINT objects.
 */
void DIV_General_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

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
 * @brief Tests the binary long division algorithm using BINT objects for the dividend and divisor, and stores the quotient and remainder.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrDividend A pointer to the pointer of the BINT object representing the dividend.
 * @param pptrDivisor A pointer to the pointer of the BINT object representing the divisor.
 * @param pptrQ A pointer to the pointer where the quotient BINT object will be stored.
 * @param pptrR A pointer to the pointer where the remainder BINT object will be stored.
 * @note This function is intended for testing the binary long division algorithm.
 */
void DIV_Bianry_Long_Test(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

/**
 * @brief Performs binary long division on BINT objects for the dividend and divisor, and stores the quotient and remainder.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrDividend A pointer to the pointer of the BINT object representing the dividend.
 * @param pptrDivisor A pointer to the pointer of the BINT object representing the divisor.
 * @param pptrQ A pointer to the pointer where the quotient BINT object will be stored.
 * @param pptrR A pointer to the pointer where the remainder BINT object will be stored.
 * @note Implements the binary long division algorithm which is efficient for large BINT objects.
 */
void DIV_Bianry_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

/**
 * @brief Performs general long division on BINT objects for the dividend and divisor, outputting the quotient and remainder.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrDividend A pointer to the pointer of the BINT object representing the dividend.
 * @param pptrDivisor A pointer to the pointer of the BINT object representing the divisor.
 * @param pptrQ A pointer to the pointer where the quotient BINT object will be stored.
 * @param pptrR A pointer to the pointer where the remainder BINT object will be stored.
 * @note This version of long division is optimized for a general case and can handle different forms of BINT objects.
 */
void DIV_General_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

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

#endif // BIGINT_ARITHMETIC_H