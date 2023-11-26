#include "config.h"
#include "bigint_utils.h"

#ifndef BIGINT_ARITHMETIC_H
#define BIGINT_ARITHMETIC_H

/**
 * @brief Performs a bitwise NOT operation on a BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrbint_dst A pointer to the BINT object pointer where the result will be stored.
 * @param pptrBint_src A pointer to the BINT object pointer to be negated.
 * @note The result is stored in ptrbint_dst, and the original BINT object is not modified.
 */
void NOT_BINT(BINT** ptrbint_dst, BINT** pptrBint_src);
/**
 * @brief Performs a bitwise AND operation between two BINT objects.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the first BINT object operand.
 * @param ptrY A pointer to the second BINT object operand.
 * @param pptrZ A pointer to the BINT object pointer where the result will be stored.
 * @note The function will allocate memory for the result if necessary.
 */
void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
/**
 * @brief Performs a bitwise OR operation between two BINT objects.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the first BINT object operand.
 * @param ptrY A pointer to the second BINT object operand.
 * @param pptrZ A pointer to the BINT object pointer where the result will be stored.
 * @note The function will allocate memory for the result if necessary.
 */
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
/**
 * @brief Performs a bitwise XOR operation between two BINT objects.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrX A pointer to the first BINT object operand.
 * @param ptrY A pointer to the second BINT object operand.
 * @param pptrZ A pointer to the BINT object pointer where the result will be stored.
 * @note The function will allocate memory for the result if necessary.
 */
void XOR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);

/**
 * @brief Performs addition with carry of two WORDs and stores the result and carry.
 * @author Your Name
 * @date Date of creation or last update
 * @param x The first WORD operand.
 * @param y The second WORD operand.
 * @param k The carry-in value.
 * @param ptrQ A pointer to the WORD where the sum will be stored.
 * @param ptrR A pointer to the WORD where the carry-out will be stored.
 * @note This is a low-level operation used in multi-word arithmetic.
 */
void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR);
/**
 * @brief Adds two multi-word integers (BINTs) and stores the result in a third BINT, managing memory as needed.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX Pointer to the pointer of the first BINT operand.
 * @param pptrY Pointer to the pointer of the second BINT operand.
 * @param pptrZ Pointer to the pointer where the resulting BINT will be stored after the addition.
 * @note This function handles the intricacies of BINT addition, including memory reallocation for the result, if necessary.
 */
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
/**
 * @brief Public API function for adding two BINTs, wrapping the core addition functionality with user-friendly access.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX Pointer to the pointer of the first BINT operand.
 * @param pptrY Pointer to the pointer of the second BINT operand.
 * @param pptrZ Pointer to the pointer where the addition result will be stored.
 * @note Utilizes add_core_xyz internally; users should call this function for adding BINTs.
 */
void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * @brief Subtracts two WORD-sized integers, taking into account a borrow, and outputs both the difference and the resulting borrow.
 * @author Your Name
 * @date Date of creation or last update
 * @param x The WORD from which y is to be subtracted.
 * @param y The WORD to be subtracted from x.
 * @param ptrQ Pointer to the variable where the difference will be stored.
 * @param ptrR Pointer to the variable where the resulting borrow will be stored; it will be non-zero if the subtraction underflows.
 * @note Essential for multi-word subtraction, this function ensures that borrows are correctly propagated through the operation.
 */
void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR); 
/**
 * @brief Subtracts two multi-word integers (BINTs) and stores the result in a third BINT, managing memory as needed.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX Pointer to the pointer of the first BINT operand.
 * @param pptrY Pointer to the pointer of the second BINT operand.
 * @param pptrZ Pointer to the pointer where the resulting BINT will be stored after the subtraction.
 * @note This function handles the complexities of BINT subtraction, including memory reallocation for the result, if necessary.
 */
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
/**
 * @brief Public API function for subtracting two BINTs, encapsulating the core subtraction functionality for ease of use.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrX Pointer to the pointer of the first BINT operand.
 * @param pptrY Pointer to the pointer of the second BINT operand.
 * @param pptrZ Pointer to the pointer where the subtraction result will be stored.
 * @note Utilizes sub_core_xyz internally; intended to be the function users call for subtracting BINTs.
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

#endif // BIGINT_ARITHMETIC_H