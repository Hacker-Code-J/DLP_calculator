#include "config.h"
#include "bigint_utils.h"

#ifndef BIGINT_ARITHMETIC_H
#define BIGINT_ARITHMETIC_H

void NOT_BINT(BINT** ptrbint_dst, BINT** pptrBint_src);
void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);
void XOR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ);

void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR);
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR); 
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

void mul_xyz(WORD valX, WORD valY, BINT** pptrZ);
void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

void mul_core_ImpTxtBk_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

void mul_core_Krtsb_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void Krtsb_FLAG_Test(BINT** pptrX, BINT** pptrY, BINT** pptrZ, int flag);
void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ);

void squ_core(WORD valX,BINT** pptrZ);
void SQU_Txtbk_xz(BINT** pptrX, BINT** pptrZ);
void SQU_Krtsb_xz(BINT** pptrX, BINT** pptrZ);

void DIV_Bianry_Long_Test(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);
void DIV_Bianry_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR);

void mul_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void add_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void mul_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void add_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ);
void exp_Mongomery(BINT** ptrX, BINT** ptrY,BINT** ptrZ);
void Mod_Exp_Mongo(BINT** ptrX,BINT** ptrY,BINT** ptrM,BINT** ptrZ);

#endif // BIGINT_ARITHMETIC_H