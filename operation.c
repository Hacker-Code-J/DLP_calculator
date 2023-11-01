#include <stdio.h>
#include <stdlib.h>

#include "setup.h"
#include "BigInteger.h"
#include "operation.h"

void NOT_BINT(BINT** pptrbint_dst, BINT** pptrbint_src) {
    BINT* ptrbint_dst = *pptrbint_dst;
    BINT* ptrbint_src = *pptrbint_src;
    for (int i = 0; i < ptrbint_src->wordlen; i++) {
        ptrbint_dst->val[i] = ~(ptrbint_src->val[i]);
    }
    ptrbint_dst->wordlen = ptrbint_src->wordlen; // The result size will be the same as the input
    // (*pptrZ)->sign = !(ptrX->sign);    // Negative if input is positive and vice versa
}
void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] & ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; // The result size will be size of the smaller operand
    // ptrZ->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] | ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; // The result size will be size of the smaller operand
    // ptrZ->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}
void XOR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] ^ ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; 
    // Resulting sign depends on the signs of operands. You can decide on a rule, 
    // but for now, let's say it's negative if either of the operands is negative.
    // (*pptrZ)->sign = ptrX->sign || ptrY->sign; 
}

void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR) {
    WORD tmp = x + y;
	*ptrQ = 0x00;
	if(tmp < x)
		*ptrQ = 0x01;
	tmp += k;
	*ptrR = tmp;
	if(tmp < k)
		*ptrQ += 0x01;
    // if (WORD_BITLEN == 8 || WORD_BITLEN == 32) {
    //     u64 result = (u64)x + (u64)y + (u64)k;
    //     *ptrR = (WORD)result;
    //     *ptrQ = result >> WORD_BITLEN;
    // } else if (WORD_BITLEN == 64) {
    //     const WORD HALF_MASK = (1ULL << 32) - 1;
    //     const u64 LOW_X = x & HALF_MASK;
    //     const u64 HIGH_X = x >> 32;
    //     const u64 LOW_Y = y & HALF_MASK;
    //     const u64 HIGH_Y = y >> 32;

    //     // Add the lower halves
    //     u64 low_result = LOW_X + LOW_Y + (k & HALF_MASK);

    //     // Check if there was a carry from the lower half
    //     u64 low_carry = (low_result > HALF_MASK) ? 1 : 0;

    //     // Add the upper halves
    //     u64 high_result = HIGH_X + HIGH_Y + (k >> 32) + low_carry;

    //     // Return results
    //     *ptrR = (high_result << 32) | (low_result & HALF_MASK);
    //     *ptrQ = (high_result >> 32);  // Carry from the higher half
    // } else {
    //     fprintf(stderr, "Unsupported WORD size in 'add_carry'\n");
    //     exit(1);
    // }
}
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "add_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "add_core_xyz");
    BINT* ptrX = *pptrX; BINT* ptrY = *pptrY;
    int n = ptrX->wordlen; int m = ptrY->wordlen;

    init_bint(pptrZ, n+1);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "add_core_xyz");
    BINT* ptrZ = *pptrZ;

    WORD res = 0x00;
    WORD carry = 0x00;
    WORD k = 0x00;
    
    int i;
    for (i = 0; i < m; i++) {
        // printf("Before: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res);
        add_carry(ptrX->val[i], ptrY->val[i], k, &carry, &res);
        ptrZ->val[i] = res;
        // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res,i,ptrZ->val[i]);
        k = carry;
    }
    for (i = m; i < n; i++) {
        // printf("Before: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res);    
        add_carry(ptrX->val[i], 0, k, &carry, &res);
        // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res, i, ptrZ->val[i]);
        ptrZ->val[i] = res;
        k = carry;
    }
    if(k) {
        ptrZ->val[n] = k;
    } else {
        ptrZ->wordlen = n;
    }
    refine_BINT(ptrX);
    refine_BINT(ptrY);
    refine_BINT(ptrZ);
}
void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "ADD");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "ADD");
    bool xGeqy = compare_abs_bint(pptrX, pptrY);
    if (!xGeqy) {
        swapBINT(pptrX,pptrY);
        // ADD(pptrY, pptrX, pptrZ);
        // return;
    }
    BINT* ptrX = *pptrX; BINT* ptrY = *pptrY;
    refine_BINT(ptrX); refine_BINT(ptrY);
    
    int n = ptrX->wordlen;

    init_bint(pptrZ, n+1);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "ADD");
    
    if (ptrX->sign == ptrY->sign) {
        // If signs are the same, add the numbers
        add_core_xyz(pptrX, pptrY, pptrZ);
        if (ptrX->sign) {
            // If both numbers are negative, then result is negative
            FLIP_SIGN(pptrZ);
        }
    } else {
        // If signs are different, subtract the numbers
        sub_core_xyz(pptrX, pptrY, pptrZ);
        if (ptrX->sign) {
            // If X is negative and Y is positive, then result is negative
            FLIP_SIGN(pptrZ);
        }
    }
}

void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR) {
    WORD tmp = x - *ptrQ;
    *ptrQ = 0x00;
    if (x < tmp)
        *ptrQ = 0x01;
    if (tmp < y)
        *ptrQ += 0x01;
    tmp -= y;
    *ptrR = tmp;

    // //Optimize
    // *res = x-b;
    // *borrow = (x < b);
    // *borrow += (*res < y);
    // *res -= y;
}
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "sub_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "sub_core_xyz");
    BINT* ptrX = *pptrX; BINT* ptrY = *pptrY;
    int n = ptrX->wordlen; int m = ptrY->wordlen;

    init_bint(pptrZ, n);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "sub_core_xyz");
    BINT* ptrZ = *pptrZ;

    bool xGeqy = compare_abs_bint(pptrX, pptrY);
    if(!xGeqy) swapBINT(pptrX,pptrY);

    WORD res = 0x00;
    WORD borrow = 0x00;

    matchSize(ptrX,ptrY);
    // printf("Z: ");printHex2(ptrZ);printf("\n");
    for(int i = 0; i < m; i++) {
        // printf("Before: X[%d] - Y[%d] - k = %x  - %x - %x = - %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], b, borrow, res,i,ptrZ->val[i]);
        sub_borrow(ptrX->val[i], ptrY->val[i], &borrow, &res);
        ptrZ->val[i] = res;
        // printf("-After: X[%d] - Y[%d] - k = %x  - %x - %x = - %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], b, borrow, res,i,ptrZ->val[i]);
    }
    // printf("Z*: ");printHex2(ptrZ);printf("\n");
    for(int i = m; i < n; i++) {
        // printf("Before: X[%d] - Y[%d] - k = %x  - %x - %x = - %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], b, borrow, res,i,ptrZ->val[i]);
        sub_borrow(ptrX->val[i], 0, &borrow, &res);
        ptrZ->val[i] = res;
        // printf("-After: X[%d] - Y[%d] - k = %x  - %x - %x = - %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], b, borrow, res,i,ptrZ->val[i]);
    }
    // printf("Z**: ");printHex2(ptrZ);printf("\n");
    refine_BINT(ptrX); refine_BINT(ptrY);
}
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "SUB");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "SUB");
    // Swap the order if abs(X) < abs(Y) to ensure X >= Y
    bool xGeqy = compare_abs_bint(pptrX, pptrY);
    if (!xGeqy) {
        swapBINT(pptrX,pptrY);
    }
    BINT* ptrX = *pptrX; BINT* ptrY = *pptrY;
    refine_BINT(ptrX); refine_BINT(ptrY);
    
    int n = ptrX->wordlen;

    init_bint(pptrZ, n);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SUB");
    
    if (ptrX->sign == ptrY->sign) {
        // If signs are the same, subtract the numbers
        sub_core_xyz(pptrX, pptrY, pptrZ);
        if (ptrX->sign) {
            // If both numbers are negative, flip the sign of the result
            FLIP_SIGN(pptrZ);
        }
    } else {
        // If signs are different, add the numbers
        add_core_xyz(pptrX, pptrY, pptrZ);
        if (ptrX->sign) {
            // If X is positive and Y is negative, then result is positive
            FLIP_SIGN(pptrZ);
        }
    }
}

void mul_xyz(WORD valX, WORD valY, BINT** pptrZ) {
	// if (!pptrZ || !*pptrZ || !(*pptrZ)->val) {
	// 	return;
	// }
    // If *pptrZ is NULL, allocate memory for it
    if (!*pptrZ) {
        init_bint(pptrZ, 2);
        if (!*pptrZ) {
            fprintf(stderr, "Error: Memory allocation failed in 'ADD'\n");
            exit(1);
        }
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_xyz");

    const int half_w = WORD_BITLEN / 2; // if w=32, half_w = 16 = 2^4
	const WORD MASK = (1 << half_w) - 1;
	
    // Split the WORDs into halves
	WORD X0 = valX & MASK;
	WORD X1 = valX >> half_w;
	WORD Y0 = valY & MASK;
	WORD Y1 = valY >> half_w;
	
    // Cross multiplication
	WORD T0 = X0 * Y1;
	WORD T1 = X1 * Y0;
	T0 = T0 + T1;
	T1 = T0 < T1; // overflow
	
    // Direct multiplication
	WORD Z0 = X0 * Y0;
	WORD Z1 = X1 * Y1;
	
    // Adjust for overflows
	WORD T = Z0;
	Z0 += (T0 << half_w);
	Z1 += (T1 << half_w) + (T0 >> half_w) + (Z0 < T);
	
    // Set results
	(*pptrZ)->val[0] = Z0;
	(*pptrZ)->val[1] = Z1;
}

void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_TxtBk_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_TxtBk_xyz");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;

    if (!*pptrZ) {
        init_bint(pptrZ, ptrX->wordlen + ptrY->wordlen);
        if (!*pptrZ) {
            fprintf(stderr, "Error: Memory allocation failed in 'mul_core_TxtBk_xyz'\n");
            exit(1);
        }
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_TxtBk_xyz");


    // if(ptrX->sign != ptrY->sign)
    //     (*pptrZ)->sign = true;
    BINT* ptrTmp = NULL;
    init_bint(&ptrTmp, ptrX->wordlen + ptrY->wordlen);
    BINT* ptrWordMUl = NULL;

    for(int i = 0; i < ptrX->wordlen; i++) {
        for(int j = 0; j < ptrY->wordlen; j++) {
            init_bint(&ptrWordMUl,2);
            mul_xyz(ptrX->val[i], ptrY->val[j], &ptrWordMUl);
            left_shift_word(&ptrWordMUl, (i+j));
            if ((*pptrZ)->wordlen < (ptrWordMUl->wordlen)){
                add_core_xyz(&ptrWordMUl,pptrZ, &ptrTmp);
            }
            else{ 
                add_core_xyz(pptrZ, &ptrWordMUl, &ptrTmp);
            }
            copyBINT(pptrZ, &ptrTmp);
            delete_bint(&ptrWordMUl);
        }
    }
    delete_bint(&ptrTmp);
    refine_BINT(*pptrZ);
}

void mul_core_ImpTxtBk_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_ImpTxtBk_test");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_ImpTxtBk_test");
    BINT* ptrX = *pptrX; BINT* ptrY = *pptrY;
    int n = ptrX->wordlen; int m = ptrX->wordlen;
    matchSize(ptrX, ptrY); makeEven(ptrX); makeEven(ptrY);

    init_bint(pptrZ, n+m);
    if (!*pptrZ) {
        fprintf(stderr, "Error: Memory allocation failed in 'mul_core_ImpTxtBk_test'\n");
        exit(1);
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_ImpTxtBk_test");
    BINT* ptrZ = *pptrZ;

    int p = ptrX->wordlen / 2; int q = ptrY->wordlen / 2;

    BINT* ptrT = NULL;
    init_bint(&ptrT, n+m);
    BINT* ptrT0 = NULL;
    init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = NULL;
    init_bint(&ptrT1, 2*p);
    BINT* ptrTmp0 = NULL;
    init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = NULL;
    init_bint(&ptrTmp1, 2*p);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            reset_bint(ptrTmp0); reset_bint(ptrTmp1);
            mul_xyz(ptrX->val[2*k], ptrY->val[j], &ptrTmp0);
            mul_xyz(ptrX->val[2*k+1], ptrY->val[j], &ptrTmp1);
            printf("\nx[%d]*y[%d]=Tmp0: ",2*k,j);printHex2(ptrTmp0);printf("\n");
            printf("x[%d]*y[%d]=Tmp1: ",2*k+1,j);printHex2(ptrTmp1);printf("\n");
            if (!k) {
                copyBINT(&ptrT0, &ptrTmp0);
                copyBINT(&ptrT1, &ptrTmp1);
                printf("--T0: ");printHex2(ptrT0);printf("\n");
                printf("--T1: ");printHex2(ptrT1);printf("\n");
            } else {
                left_shift_word(&ptrTmp0, 2*k);
                refine_BINT_word(ptrTmp0, 2*k);
                OR_BINT(ptrTmp0, ptrT0, &ptrT0);
                left_shift_word(&ptrTmp1, 2*k);
                refine_BINT_word(ptrTmp1, 2*k);
                OR_BINT(ptrTmp1, ptrT1, &ptrT1);
                printf("--T0: ");printHex2(ptrT0);printf("\n");
                printf("--T1: ");printHex2(ptrT1);printf("\n");
            }
        }
        left_shift_word(&ptrT1, 1);
        printf("\nResult:-------------------------------------------------\n");
        printf("--T0: ");printHex2(ptrT0);printf("\n");
        printf("--T1: ");printHex2(ptrT1);printf("\n\n");

        if (ptrT1->wordlen > ptrT0->wordlen)
            add_core_xyz(&ptrT1, &ptrT0, &ptrT);
        else
            add_core_xyz(&ptrT0, &ptrT1, &ptrT);

        printf("\n\nprint(int(hex(");
        printHex2(ptrT1);printf(" + ");printHex2(ptrT0);
        printf("), 16) == int(\"");
        printHex2(ptrT);printf("\", 16))\n\n");
        
        left_shift_word(&ptrT, j);
        printHex2(ptrT);

        printf("\n\nprint(int(hex(");
        printHex2(ptrT);printf(" + ");printHex2(*pptrZ);
        printf("), 16) == int(\"");
        if (ptrZ->wordlen > ptrT->wordlen) // wordlen Z >= wordlen T
            add_core_xyz(pptrZ, &ptrT, pptrZ);
        else 
            add_core_xyz(&ptrT, pptrZ, pptrZ);
        printHex2(*pptrZ);printf("\", 16))\n\n");

    }
    // Cleanup
    delete_bint(&ptrT);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrTmp0); delete_bint(&ptrTmp1);
}
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_ImpTxtBk_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_ImpTxtBk_xyz");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    
    int n = ptrX->wordlen;
    int m = ptrX->wordlen;

    matchSize(ptrX, ptrY);
    makeEven(ptrX); makeEven(ptrY);

    delete_bint(pptrZ);
    init_bint(pptrZ, n+m);
    if (!*pptrZ) {
        fprintf(stderr, "Error: Memory allocation failed in 'MUL_Core_ImpTxtBk_xyz'\n");
        exit(1);
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_ImpTxtBk_xyz");
    BINT* ptrZ = *pptrZ;

    int p = ptrX->wordlen / 2;
    int q = ptrY->wordlen / 2;

    BINT* ptrT = NULL;
    init_bint(&ptrT, n+m);
    BINT* ptrT0 = NULL;
    init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = NULL;
    init_bint(&ptrT1, 2*p);
    BINT* ptrTmp0 = NULL;
    init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = NULL;
    init_bint(&ptrTmp1, 2*p);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            reset_bint(ptrTmp0);
            reset_bint(ptrTmp1);
            mul_xyz(ptrX->val[2*k], ptrY->val[j], &ptrTmp0);
            mul_xyz(ptrX->val[2*k+1], ptrY->val[j], &ptrTmp1);
            if (!k) {
                copyBINT(&ptrT0, &ptrTmp0);
                copyBINT(&ptrT1, &ptrTmp1);
            } else {
                left_shift_word(&ptrTmp0, 2*k);
                refine_BINT_word(ptrTmp0, 2*k);
                OR_BINT(ptrTmp0, ptrT0, &ptrT0);

                left_shift_word(&ptrTmp1, 2*k);
                refine_BINT_word(ptrTmp1, 2*k);
                OR_BINT(ptrTmp1, ptrT1, &ptrT1);
            }
        }
        left_shift_word(&ptrT1, 1);
        if (ptrT1->wordlen > ptrT0->wordlen)
            add_core_xyz(&ptrT1, &ptrT0, &ptrT);
        else
            add_core_xyz(&ptrT0, &ptrT1, &ptrT);
        left_shift_word(&ptrT, j);
        if (ptrZ->wordlen > ptrT->wordlen) // wordlen Z >= wordlen T
            add_core_xyz(pptrZ, &ptrT, pptrZ);
        else 
            add_core_xyz(&ptrT, pptrZ, pptrZ);
    }
    delete_bint(&ptrT);
    delete_bint(&ptrT0);
    delete_bint(&ptrT1);
    delete_bint(&ptrTmp0);
    delete_bint(&ptrTmp1);
}

void mul_core_Krtsb_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_Krtsb_test");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_Krtsb_test");

    int n = (*pptrX)->wordlen;
    int m = (*pptrX)->wordlen;

    init_bint(pptrZ, n+m);
    if (!*pptrZ) {
        fprintf(stderr, "Error: Memory allocation failed in 'mul_core_Krtsb_test'\n");
        exit(1);
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_Krtsb_test");
    BINT* ptrZ = *pptrZ;

    if (FLAG >= MIN(n,m)) {
        mul_core_ImpTxtBk_test(pptrX,pptrY,pptrZ);
        return;  // This will immediately terminate the function.
    }

    int l = (MAX(n,m) + 1) >> 1;
    printf("l= MAX(%d, %d) + 1 >> 1 : %d\n\n", n, m, l);

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL; BINT* ptrR = NULL; 

    copyBINT(&ptrX1, pptrX);
    // printf("X1: ");printHex2(ptrX1);printf("\n");
    right_shift_word(&ptrX1, l);
    // printf("X1: ");printHex2(ptrX1);printf("\n");

    copyBINT(&ptrX0, pptrX);
    // printf("X0: ");printHex2(ptrX0);printf("\n");
    reduction(&ptrX0, l * WORD_BITLEN);
    // printf("X0: ");printHex2(ptrX0);printf("\n");

    copyBINT(&ptrY1, pptrY);
    right_shift_word(&ptrY1, l);
    copyBINT(&ptrY0, pptrY);
    reduction(&ptrY0, l * WORD_BITLEN);

    mul_core_Krtsb_test(&ptrX1, &ptrY1, &ptrT1);
    printf("X1: ");printHex2(ptrX1);printf(", Y1 ");printHex2(ptrY1);printf("\n");
    printf("X1*Y1: ");printHex2(ptrT1);printf("\n\n");

    mul_core_Krtsb_test(&ptrX0, &ptrY0, &ptrT0);
    printf("X0: ");printHex2(ptrX0);printf(", Y0 ");printHex2(ptrY0);printf("\n");
    printf("X0*Y0: ");printHex2(ptrT0);printf("\n\n");

    left_shift_word(&ptrT1, 2*l);
    printf("*W^{2l}: ");printHex2(ptrT1);printf("\n");

    matchSize(ptrT0, ptrT1);
    init_bint(&ptrR, 2 * (n+m+l));
    OR_BINT(ptrT0,ptrT1,&ptrR);
    refine_BINT(ptrT0);
    printf("X1Y1||X0Y0: ");printHex2(ptrR);printf("\n");

    SUB(&ptrX0, &ptrX1, &ptrS1);
    printf("X0: ");printHex2(ptrX0);printf(", X1 ");printHex2(ptrX1);printf("\n");
    printf("X0-X1: ");printHex2(ptrS1);printf("\n\n");
    SUB(&ptrY1, &ptrY0, &ptrS0);
    printf("Y1: ");printHex2(ptrY1);printf(", Y0 ");printHex2(ptrY0);printf("\n");
    printf("Y1-Y0: ");printHex2(ptrS0);printf("\n\n");
    
    mul_core_Krtsb_test(&ptrS1, &ptrS0, &ptrS);
    printf("X0-X1: ");printHex2(ptrS1);printf(", Y1-Y0 ");printHex2(ptrS0);printf("\n");
    printf("(X0-X1)*(Y1-Y0): ");printHex2(ptrS);printf("\n\n");

    right_shift_word(&ptrT1, 2*l);
    printf("(X0-X1)*(Y1-Y0): ");printHex2(ptrS);printf(", X1Y1: ");printHex2(ptrT1);printf("\n");
    add_core_xyz(&ptrS,&ptrT1,&ptrS);
    // ADD(&ptrS,&ptrT1,&ptrS);
    printf("(X0-X1)*(Y1-Y0) + X1Y1: ");printHex2(ptrS);printf("\n\n");
    
    
    printf("(X0-X1)*(Y1-Y0) + X1Y1: ");printHex2(ptrS);printf(", X0Y0: ");printHex2(ptrT0);printf("\n");
    add_core_xyz(&ptrS,&ptrT0,&ptrS);
    // ADD(&ptrS,&ptrT0,&ptrS);
    printf("(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0: ");printHex2(ptrS);printf("\n\n");
    
    left_shift_word(&ptrS, l);
    printf("*W^l: ");printHex2(ptrS);printf("\n");

    printf("X1Y1||X0Y0: ");printHex2(ptrZ);printf(", [(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0]w^l: ");printHex2(ptrS);printf("\n");
    add_core_xyz(&ptrR, &ptrS, pptrZ);
    printf("*Result: ");printHex2(ptrZ);printf("\n");

    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS); delete_bint(&ptrR); 
}


void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_Krtsb_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_Krtsb_xyz");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    
    int n = ptrX->wordlen;
    int m = ptrX->wordlen;

    init_bint(pptrZ, n+m);
    if (!*pptrZ) {
        fprintf(stderr, "Error: Memory allocation failed in 'MUL_Core_Krtsb_xyz'\n");
        exit(1);
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");

    if (FLAG >= MIN(n,m)) {
        MUL_Core_ImpTxtBk_xyz(pptrX,pptrY,pptrZ);
        return;  // This will immediately terminate the function.
    }

    int l = MAX(n,m+1) >> 1;

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL;

    copyBINT(&ptrX1, pptrX); right_shift_word(&ptrX1, l);
    copyBINT(&ptrX0, pptrX); reduction(&ptrX0, l * WORD_BITLEN);
    copyBINT(&ptrY1, pptrY); right_shift_word(&ptrY1, l);
    copyBINT(&ptrY0, pptrY); reduction(&ptrY0, l * WORD_BITLEN);

    MUL_Core_Krtsb_xyz(&ptrX1, &ptrY1, &ptrT1);
    MUL_Core_Krtsb_xyz(&ptrX0, &ptrY0, &ptrT0);

    left_shift_word(&ptrT1, 2*l);

    matchSize(ptrT0, ptrT1);
    OR_BINT(ptrT0,ptrT1,pptrZ);
    refine_BINT(ptrT0);

    SUB(&ptrX0, &ptrX1, &ptrS1);
    SUB(&ptrY1, &ptrY0, &ptrS0);
  
    MUL_Core_Krtsb_xyz(&ptrS1, &ptrS0, &ptrS);
   
    right_shift_word(&ptrT1, 2*l);
    add_core_xyz(&ptrS,&ptrT1,&ptrS);
    add_core_xyz(&ptrS,&ptrT0,&ptrS);
    left_shift_word(&ptrS, 2*l);
    add_core_xyz(pptrZ, &ptrS, pptrZ);

    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS);
}