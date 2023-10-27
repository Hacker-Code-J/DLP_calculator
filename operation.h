#include <stdarg.h>
#include <stdbool.h>

#include "setup.h"
#include "BigInteger.h"

#ifndef OPERATION_H
#define OPERATION_H

extern const BINT BINT_ZERO;    //zero integer
extern const BINT BINT_ONE;     //one integer
extern const BINT BINT_NEG_ONE; //negative one integer

// Function prototypes for BINT arithmetic operations.

/************************************
* W := 2^{sizeof(WORD)*8}
* x + y + k = carry * W + res
************************************/
void add_xyk(WORD x, WORD y, WORD k, WORD* res, WORD* carry);

// Z = X + Y with wordlen(X) >= wordlen(Y)
void add_xyz(BINT* X, BINT* Y, BINT* Z);


// Integrate ADD
void ADD_xyz(BINT* X, BINT* Y, BINT* Z);

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

/**
 * W = 2^w  
 * X, Y in [0,W) and Z in [0, W^2)
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
void mul_xyz2(WORD valX, WORD valY, BINT** pptrZ);
void mul_xyz(WORD X, WORD Y, WORD* ptrZ); 
void mul_core_xyz(BINT* ptrX, BINT* ptrY, BINT* ptrZ);

#endif // OPERATION_H