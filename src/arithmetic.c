#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arithmetic.h"

void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR) {
    WORD sum = x + y;
	*ptrQ = 0x00;
	if(sum < x)
		*ptrQ = 0x01;
	sum += k;
	*ptrR = sum;
	if(sum < k)
		*ptrQ += 0x01;
}
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "add_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "add_core_xyz");
    if (!compare_abs_bint(*pptrX,*pptrY)) {
        add_core_xyz(pptrY, pptrX, pptrZ);
        return;
    }  

    init_bint(pptrZ, (*pptrX)->wordlen + 1);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "add_core_xyz");

    WORD res = 0x00;
    WORD carry = 0x00;
    int k = 0x00;

    for(int i = 0; i < (*pptrY)->wordlen; i++) {
        add_carry((*pptrX)->val[i], (*pptrY)->val[i], k, &carry, &res);
        (*pptrZ)->val[i] = res;
        // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res,i,ptrZ->val[i]);
        k = carry;
    }
    for (int i = (*pptrY)->wordlen; i < (*pptrX)->wordlen; i++) {
        add_carry((*pptrX)->val[i],(WORD)0, k, &carry, &res);
        (*pptrZ)->val[i] = res;
        k = carry;
    }
    (*pptrZ)->val[(*pptrX)->wordlen] = k;
    refineBINT(*pptrZ);
}

void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "ADD");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "ADD");
    if (!compare_abs_bint(*pptrX, *pptrY)) {
        ADD(pptrY, pptrX, pptrZ);
        swapBINT(pptrX,pptrY);
        return;
    }    
    int n = (*pptrX)->wordlen;

    init_bint(pptrZ, n+1);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "ADD");
    
    if ((*pptrX)->sign == (*pptrY)->sign) {
        // If signs are the same, add the numbers
        add_core_xyz(pptrX, pptrY, pptrZ);
        if ((*pptrX)->sign) {
            // If both numbers are negative, then result is negative
            (*pptrZ)->sign = true;
        }
    } else {
        // If signs are different, subtract the numbers
        sub_core_xyz(pptrX, pptrY, pptrZ);
        if ((*pptrX)->sign) {
            // If X is negative and Y is positive, then result is negative
            (*pptrZ)->sign = true;
        }
    }
}

void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR) {
    WORD tmp = (WORD)x - *ptrQ;
    *ptrQ = 0x00;
    if (x < tmp)
        *ptrQ = 0x01;
    if (tmp < y)
        *ptrQ += 0x01;
    tmp = (WORD)tmp-y;
    *ptrR = tmp;
}
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "sub_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "sub_core_xyz");
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;
    bool xGeqy = compare_abs_bint(*pptrX, *pptrY);
    if (!xGeqy) {
        sub_core_xyz(pptrY, pptrX, pptrZ);
        swapBINT(pptrX,pptrY);
        return;
    }  

    init_bint(pptrZ, MAXIMUM(n,m));
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "sub_core_xyz");
    
    WORD res = 0x00;
    WORD borrow = 0x00;

    matchSize(*pptrX,*pptrY);
    for(int i = 0; i < m; i++) {
        sub_borrow((*pptrX)->val[i], (*pptrY)->val[i], &borrow, &res);
        (*pptrZ)->val[i] = res;
    }
    for(int i = m; i < n; i++) {
        sub_borrow((*pptrX)->val[i], (WORD)0, &borrow, &res);
        (*pptrZ)->val[i] = res;
    }
    refineBINT(*pptrX); refineBINT(*pptrY);
    if(!xGeqy) swapBINT(pptrX,pptrY);
}
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "SUB");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "SUB");
    int n = (*pptrX)->wordlen;
    int m = (*pptrY)->wordlen;
    bool sgnX = (*pptrX)->sign;
    bool sgnY = (*pptrY)->sign;
    
    init_bint(pptrZ, MAXIMUM(n,m));
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SUB");


    bool xGeqy = compare_bint(*pptrX, *pptrY);
    if ((*pptrY)->sign == false && xGeqy) {
        sub_core_xyz(pptrX,pptrY,pptrZ);
    } else if ((*pptrX)->sign == false && !xGeqy) {
        sub_core_xyz(pptrY,pptrX,pptrZ);
        (*pptrZ)->sign = true;
    } else if ((*pptrX)->sign == true && xGeqy) {
        (*pptrX)->sign = false;
        (*pptrY)->sign = false;
        sub_core_xyz(pptrY,pptrX,pptrZ);
    } else if ((*pptrY)->sign == true && !xGeqy) {
        (*pptrX)->sign = false;
        (*pptrY)->sign = false;
        sub_core_xyz(pptrX,pptrY,pptrZ);
        (*pptrZ)->sign = true;
    } else if ((*pptrX)->sign == false && (*pptrY)->sign == true) {
        (*pptrY)->sign = false;
        ADD(pptrX,pptrY,pptrZ);
    } else {
        (*pptrX)->sign = false;
        ADD(pptrX,pptrY,pptrZ);
        (*pptrZ)->sign = true;
    }

    (*pptrX)->sign = sgnX;
    (*pptrY)->sign = sgnY;
}