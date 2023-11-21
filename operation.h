#include "setup.h"
#include "BigInteger.h"

#ifndef OPERATION_H
#define OPERATION_H

/**
 * Computes the bitwise NOT of the given BINT and stores the result in another BINT.
 * @param pptrbint_dst  Double Pointer to the destination BINT pointer where the result will be stored.
 * @param pptrbint_src  Dobule Pointer to the source BINT 
 */
void NOT_BINT(BINT** ptrbint_dst, BINT** pptrBint_src);
/**
 * Computes the bitwise AND of two given BINTs and stores the result in another BINT.
 * @param ptrX   Pointer to the first source BINT.
 * @param ptrY   Pointer to the second source BINT.
 * @param pptrZ  Pointer to the destination BINT pointer where the result will be stored.
 */
void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
/**
 * Computes the bitwise OR of two given BINTs and stores the result in another BINT.
 * @param ptrX   Pointer to the first source BINT.
 * @param ptrY   Pointer to the second source BINT.
 * @param pptrZ  Pointer to the destination BINT pointer where the result will be stored.
 */
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
void OOR_BINT(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
/**
 * Computes the bitwise XOR of two given BINTs and stores the result in another BINT.
 * @param ptrX   Pointer to the first source BINT.
 * @param ptrY   Pointer to the second source BINT.
 * @param pptrZ  Pointer to the destination BINT pointer where the result will be stored.
 */
void XOR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);

// Function prototypes for BINT arithmetic operations.

/**
 * Adds two WORD values along with a carry and stores the result and carry-over.
 * x + y + k = Q * 2^w + R
 * @param x      First WORD value.
 * @param y      Second WORD value.
 * @param k      Initial carry value.
 * @param ptrQ   Pointer where the resulting carry will be stored.
 * @param ptrR   Pointer where the result will be stored.
 */
void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR);

/**
 * Core logic to add two BINT values. wordlen(X) >= wordlen(Y)
 * @param pptrX  Pointer to the first source BINT pointer.
 * @param pptrY  Pointer to the second source BINT pointer.
 * @param pptrZ  Pointer to the destination BINT pointer where the result will be stored.
 */
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);


// Integrate ADD
void ADD_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/**
 * Adds two BINT values and stores the result in another BINT.
 * @param pptrX  Pointer to the first source BINT pointer.
 * @param pptrY  Pointer to the second source BINT pointer.
 * @param pptrZ  Pointer to the destination BINT pointer where the result will be stored.
 */
void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ);


/************************************ 
* W := 2^{sizeof(WORD)*8}
* x - b - y = - brrow * W + res
************************************/
void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR); 

// Z = X - Y with X >= Y >= 0.
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

// Integrate SUB
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

// Author: Moon Ye-chan
// void mult_xyc(WORD x, WORD y , WORD* C);

/** Single-Word Multiplication
 * Input: X, Y in [0,W)
 * Output: Z = XY in [0,W^2)
 * 
 * X = X_1 || X_0
 * Y = Y_1 || Y_0
 * 
 * X_i, Y_i in [0,W^{1/2}) for i in {0,1}
 * 
 * XY = (X_1Y_1)W + X_0Y_0 + (X_1Y_0+X_0Y_1)W^{1/2}
 *    = (X_1Y_1 << w) + (X_0Y_0) + ((X_1Y_0+X_0Y_1) << w/2 )
 * 
*/
void mul_xyz(WORD valX, WORD valY, BINT** pptrZ);

/** Textbook Multiplication
 * Input: X is n-word integer, x_{n-1} || ... || x_0 with x_i in [0,W)
 *        Y is m-word integer, y_{m-1} || ... || x_0 with y_i in [0,W)
 * Output: Z = XY in [0,W^{n+m})
*/
void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

/** Improved Textbook Multiplication
 * Input: X is n-word integer s.t. n=2p
 *        Y is m-word integer s.t. n=2q
 * Output: Z = XY in [0,W^{n+m}) = [0,W^{2(p+q)})
*/
// void mul_core_ImpTxtBk_xyz(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
void mul_core_ImpTxtBk_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void mul_core_Krtsb_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void Krtsb_FLAG_Test(BINT** pptrX, BINT** pptrY, BINT** pptrZ, int flag);
void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

void squ_core(WORD valX,BINT** pptrZ);
void SQU_Txtbk_xz(BINT** pptrX, BINT** pptrZ);

void DIV_Bianry_Long_Test(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);
void DIV_Bianry_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

void mul_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void add_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void mul_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void add_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void exp_Mongomery(BINT** ptrX, BINT** ptrY,BINT** ptrZ);


// void sqrt(BINT** pptrBint_dst, BINT** pptrBint_src);

#endif // OPERATION_H