#include <stdarg.h>
#include <stdbool.h>

#include "setup.h"
#include "BigInteger.h"

#ifndef OPERATION_H
#define OPERATION_H

extern const BINT BINT_ZERO;    //zero integer
extern const BINT BINT_ONE;     //one integer
extern const BINT BINT_NEG_ONE; //negative one integer

void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);


// Function prototypes for BINT arithmetic operations.

/** Add with Carry
 * Input: Single-Word X, Y in [0,W) and carry k in {0,1}
 * Output: Q in {0,1} and R in [0,W) s.t. X + Y + k = Q * W + R
 * 
 * W = 2^w
*/
void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR);

// Z = X + Y with wordlen(X) >= wordlen(Y)
void add_xyz(BINT* X, BINT* Y, BINT* Z);
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);


// Integrate ADD
void ADD_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ);


/************************************ 
* W := 2^{sizeof(WORD)*8}
* x - b - y = - brrow * W + res
************************************/
void sub_xby(WORD x, WORD b, WORD y, WORD* res, WORD* borrow); 

// Z = X - Y with X >= Y >= 0.
void sub_xyz(BINT* X, BINT* Y, BINT* Z);

// Integrate SUB
void SUB_xyz(BINT* X, BINT* Y, BINT* Z);

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
void mul_core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

void MUL_Core_ImpTxtBk(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

#endif // OPERATION_H