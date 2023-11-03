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
    int min_len = MIN(ptrX->wordlen, ptrY->wordlen);
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
    WORD sum = x + y;
	*ptrQ = 0x00;
	if(sum < x)
		*ptrQ = 0x01;
	sum += k;
	*ptrR = sum;
	if(sum < k)
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
    if (!compare_abs_bint(pptrX,pptrY)) {
        add_core_xyz(pptrY, pptrX, pptrZ);
        return;
    }  
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;

    init_bint(pptrZ, n+1);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "add_core_xyz");

    // init_bint(pptrZ, n+1);
    // CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "add_core_xyz");

    WORD res = 0x00;
    WORD carry = 0x00;
    WORD k = 0x00;

    for (int i = 0; i < m; i++) {
        // printf("Before: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res);
        add_carry((*pptrX)->val[i], (*pptrY)->val[i], k, &carry, &res);
        (*pptrZ)->val[i] = res;
        // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res,i,ptrZ->val[i]);
        k = carry;
    }
    for (int i = m; i < n; i++) {
        add_carry((*pptrX)->val[i],(WORD)0, k, &carry, &res);
        (*pptrZ)->val[i] = res;
        k = carry;
    }
    if(k) {
        (*pptrZ)->val[n] = k;
    } else {
        (*pptrZ)->wordlen = n;
    }
    refine_BINT((*pptrZ));
}
void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "ADD");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "ADD");
    // bool xGeqy = compare_abs_bint(pptrX, pptrY);
    if (!compare_abs_bint(pptrX, pptrY)) {
        ADD(pptrY, pptrX, pptrZ);
        return;
        // swapBINT(pptrX,pptrY);
    }
    refine_BINT(*pptrX); refine_BINT(*pptrY);
    
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

    // //Optimize
    // *res = x-b;
    // *borrow = (x < b);
    // *borrow += (*res < y);
    // *res -= y;
}
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "sub_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "sub_core_xyz");
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;
    bool xGeqy = compare_abs_bint(pptrX, pptrY);
    if(!xGeqy) swapBINT(pptrX,pptrY);

    init_bint(pptrZ, MAX(n,m));
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
    refine_BINT(*pptrX); refine_BINT(*pptrY);
    if(!xGeqy) swapBINT(pptrX,pptrY);
}
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "SUB");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "SUB");
    // Swap the order if abs(X) < abs(Y) to ensure X >= Y
    // bool xGeqy = compare_abs_bint(pptrX, pptrY);
    int n = (*pptrX)->wordlen;
    int m = (*pptrY)->wordlen;
    bool sgnX = (*pptrX)->sign;
    bool sgnY = (*pptrY)->sign;
    // if (!xGeqy) { swapBINT(pptrX,pptrY); }
    
    init_bint(pptrZ, MAX(n,m));
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SUB");
    
    // if (!(*pptrY)->sign && compare_bint(pptrX,pptrY)) {
    //     sub_core_xyz(pptrX,pptrY,pptrZ);
    // } else if (!(*pptrX)->sign && !compare_bint(pptrX,pptrY)) {
    //     sub_core_xyz(pptrY,pptrX,pptrZ);
    //     (*pptrZ)->sign = true;
    // } else if ((*pptrX)->sign && compare_bint(pptrX,pptrY)) {
    //     (*pptrX)->sign = false;
    //     (*pptrY)->sign = false;
    //     sub_core_xyz(pptrY,pptrX,pptrZ);
    // } else if ((*pptrY)->sign && !compare_bint(pptrX,pptrY)) {
    //     (*pptrX)->sign = false;
    //     (*pptrY)->sign = false;
    //     sub_core_xyz(pptrX,pptrY,pptrZ);
    //     (*pptrZ)->sign = true;
    // } else if (!(*pptrX)->sign && (*pptrY)->sign == true) {
    //     (*pptrY)->sign = false;
    //     ADD(pptrX,pptrY,pptrZ);
    // } else {
    //     (*pptrX)->sign = false;
    //     ADD(pptrX,pptrY,pptrZ);
    //     (*pptrZ)->sign = true;
    // }

    if ((*pptrY)->sign == false && compare_bint(pptrX,pptrY)) {
        sub_core_xyz(pptrX,pptrY,pptrZ);
    } else if ((*pptrX)->sign == false && !compare_bint(pptrX,pptrY)) {
        sub_core_xyz(pptrY,pptrX,pptrZ);
        (*pptrZ)->sign = true;
    } else if ((*pptrX)->sign == true && compare_bint(pptrX,pptrY)) {
        (*pptrX)->sign = false;
        (*pptrY)->sign = false;
        sub_core_xyz(pptrY,pptrX,pptrZ);
    } else if ((*pptrY)->sign == true && !compare_bint(pptrX,pptrY)) {
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
    // if (!xGeqy) { swapBINT(pptrX,pptrY); }
}

void mul_xyz(WORD valX, WORD valY, BINT** pptrZ) {
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
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;

    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_TxtBk_xyz");

    BINT* ptrWordMul = NULL;
    BINT* ptrTemp = NULL;
    init_bint(&ptrTemp, m+n);
    //SET_BINT_CUSTOM_ZERO(&ptrWordMul, 2);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            init_bint(&ptrWordMul, 2);
            //printf("\nBefore: x[%d]*y[%d] = 0x%x * 0x%x = ",i,j,ptrX->val[i], ptrY->val[j]);
            mul_xyz((*pptrX)->val[i], (*pptrY)->val[j], &ptrWordMul);
            //printf("\n-After: x[%d]*y[%d] = 0x%x * 0x%x = ",i,j,ptrX->val[i], ptrY->val[j]);
            //printHex2(ptrWordMul);printf("\n");
            left_shift_word(&ptrWordMul, (i+j));
            //printf("After shift: ");printHex2(ptrWordMul);printf("\n");
            add_core_xyz(pptrZ, &ptrWordMul , &ptrTemp);
            //printf("\n Result: ");printHex2(ptrZ);printf("\n");
            // ptrTmp->val = ptrZ->val;
            // ADD(&ptrTmp, &ptrWordMul, pptrZ);
            copyBINT(pptrZ,&ptrTemp);
        }
    }
    delete_bint(&ptrWordMul);
    if((*pptrX)->sign != (*pptrY)->sign)
        (*pptrZ)->sign = true;
}

void mul_core_ImpTxtBk_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_ImpTxtBk_test");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_ImpTxtBk_test");
    if(!compare_abs_bint(pptrX,pptrY)) swapBINT(pptrX,pptrY);
    matchSize(*pptrX, *pptrY);
    makeEven(*pptrX); makeEven(*pptrY);
    int n = (*pptrX)->wordlen; int m = (*pptrX)->wordlen;
    custom_printHex_xy(*pptrX, *pptrY, MAX(n,m));

    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_ImpTxtBk_test");

    int p = n / 2; int q = n / 2;

    BINT* ptrT = NULL;
    init_bint(&ptrT, n+m);
    BINT* ptrT0 = NULL;
    init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = NULL;
    init_bint(&ptrT1, 2*p+1);
    BINT* ptrTmp0 = NULL;
    init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = NULL;
    init_bint(&ptrTmp1, 2*p+1);
    BINT* ptrTmpZ = NULL;
    init_bint(&ptrTmpZ, (*pptrZ)->wordlen);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            reset_bint(ptrTmp0); reset_bint(ptrTmp1);
            mul_xyz((*pptrX)->val[2*k], (*pptrY)->val[j], &ptrTmp0);
            mul_xyz((*pptrX)->val[2*k+1], (*pptrY)->val[j], &ptrTmp1);
            printf("\nx[%d]*y[%d]=", 2*k, j);
            printf("%08x * %08x = Tmp0: \n",(*pptrX)->val[2*k],(*pptrY)->val[j]);
            printHex2(ptrTmp0);printf("\n");
            printf("\nx[%d]*y[%d]=", 2*k+1, j);
            printf("%08x * %08x = Tmp1: \n",(*pptrX)->val[2*k+1],(*pptrY)->val[j]);
            printHex2(ptrTmp1);printf("\n");
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

        add_core_xyz(&ptrT1, &ptrT0, &ptrT);
        
        printf("\n\nprint(hex(");
        printHex2(ptrT1);printf(" + ");printHex2(ptrT0);
        printf(") == hex(");
        printHex2(ptrT);printf("))\n\n");
        
        left_shift_word(&ptrT, j);
        printf("T: ");printHex2(ptrT);

        printf("\n\nprint(hex(");
        printHex2(ptrT);printf(" + ");printHex2(*pptrZ);
        printf(") == hex(");
        // BINT* ptrTmpZ = *pptrZ;
        copyBINT(&ptrTmpZ, pptrZ);
        add_core_xyz(&ptrTmpZ, &ptrT, pptrZ);
        // if ((*pptrZ)->wordlen > ptrT->wordlen) // wordlen Z >= wordlen T
        //     add_core_xyz(pptrZ, &ptrT, pptrZ);
        // else 
        //     add_core_xyz(&ptrT, pptrZ, pptrZ);
        printHex2(*pptrZ);printf("))\n\n");
        printf("%d-th Z: ", j);printHex2(*pptrZ);printf("\n\n\n");
    }
    // Cleanup
    delete_bint(&ptrT);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrTmp0); delete_bint(&ptrTmp1);
    delete_bint(&ptrTmpZ);
    refine_BINT(*pptrX); refine_BINT(*pptrY);
    if((*pptrX)->sign != (*pptrY)->sign)
        (*pptrZ)->sign = true;
}
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_ImpTxtBk_test");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_ImpTxtBk_test");
    if(!compare_abs_bint(pptrX,pptrY)) swapBINT(pptrX,pptrY);
    matchSize(*pptrX, *pptrY);
    makeEven(*pptrX); makeEven(*pptrY);
    int n = (*pptrX)->wordlen; int m = (*pptrX)->wordlen;

    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_ImpTxtBk_test");

    int p = n / 2; int q = n / 2;

    BINT* ptrT = NULL;
    init_bint(&ptrT, n+m);
    BINT* ptrT0 = NULL;
    init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = NULL;
    init_bint(&ptrT1, 2*p+1);
    BINT* ptrTmp0 = NULL;
    init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = NULL;
    init_bint(&ptrTmp1, 2*p+1);
    BINT* ptrTmpZ = NULL;
    init_bint(&ptrTmpZ, (*pptrZ)->wordlen);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            reset_bint(ptrTmp0); reset_bint(ptrTmp1);
            mul_xyz((*pptrX)->val[2*k], (*pptrY)->val[j], &ptrTmp0);
            mul_xyz((*pptrX)->val[2*k+1], (*pptrY)->val[j], &ptrTmp1);
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

        add_core_xyz(&ptrT1, &ptrT0, &ptrT);
        
        left_shift_word(&ptrT, j);

        copyBINT(&ptrTmpZ, pptrZ);
        add_core_xyz(&ptrTmpZ, &ptrT, pptrZ);
    }
    // Cleanup
    delete_bint(&ptrT);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrTmp0); delete_bint(&ptrTmp1);
    delete_bint(&ptrTmpZ);
    refine_BINT(*pptrX); refine_BINT(*pptrY);
    if((*pptrX)->sign != (*pptrY)->sign)
        (*pptrZ)->sign = true;
}

void mul_core_Krtsb_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_Krtsb_test");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_Krtsb_test");
    int n = (*pptrX)->wordlen;
    int m = (*pptrX)->wordlen;
    static int lenZ = -1; // Declare lenZ as a static variable
    if (lenZ == -1) {
        lenZ = n + m; // Calculate lenZ only if it hasn't been initialized yet
    }
    init_bint(pptrZ, lenZ);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_Krtsb_test");
    if (FLAG >= MIN(n,m)) {
        BINT* tmpTxtBk_X = NULL;
        BINT* tmpTxtBk_Y = NULL;
        copyBINT(&tmpTxtBk_X, pptrX);
        copyBINT(&tmpTxtBk_Y, pptrY);
        printf("\nFLAG on!\n");
        printf("Before ImpTxtBk:\n");
        printHex2(tmpTxtBk_X);printf(" * ");
        printHex2(tmpTxtBk_Y);
        printf("\n");
        MUL_Core_ImpTxtBk_xyz(&tmpTxtBk_X,&tmpTxtBk_Y,pptrZ);
        printf("-After ImpTxtBk:\n");
        printHex2(tmpTxtBk_X);printf(" * ");
        printHex2(tmpTxtBk_Y);printf("=\n");
        printHex2(*pptrZ);
        printf("\n");
        printf("FLAG END\n\n");
        delete_bint(&tmpTxtBk_X);
        delete_bint(&tmpTxtBk_Y);
        return;  // This will immediately terminate the function.
    }
    printf("\nMain-----------------------------------------------------------\n");
    printf("Main Krtsb Start!\n");
    printf("FLAG and MAX: %d, %d\n", FLAG, MAX(n,m));
    custom_printHex_xy(*pptrX, *pptrY, MAX(n,m));

    int l = (MAX(n,m) + 1) >> 1;
    printf("l= MAX(%d, %d) + 1 >> 1 = %d\n\n", n, m, l);

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    // BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);
    BINT* ptrR = NULL;
    BINT* ptrTmpR = NULL;
    init_bint(&ptrR, (*pptrZ)->wordlen);
    BINT* ptrTmpST1 = NULL;
    BINT* ptrTmpST0 = NULL;

    copyBINT(&ptrX1, pptrX);
    printf("X1: ");printHex2(ptrX1);printf("\n");
    right_shift_word(&ptrX1, l);
    printf("X1 >> %d: ", l);printHex2(ptrX1);printf("\n");

    copyBINT(&ptrX0, pptrX);
    printf("X0: ");printHex2(ptrX0);printf("\n");
    reduction(&ptrX0, l * WORD_BITLEN);
    printf("X0 mod 2^{w*%d}: ", l);printHex2(ptrX0);printf("\n");

    copyBINT(&ptrY1, pptrY);
    printf("Y1: ");printHex2(ptrY1);printf("\n");
    right_shift_word(&ptrY1, l);
    printf("Y1 >> %d: ", l);printHex2(ptrY1);printf("\n");
    copyBINT(&ptrY0, pptrY);
    reduction(&ptrY0, l * WORD_BITLEN);
    printf("Y0 mod 2^{w*%d}: ", l);printHex2(ptrY0);printf("\n");

    printf("\nKarT1***************************************************************\n");
    printf("\nKrtsb - T1 start!\n");
    printf("X1 at Krtsb - T1: ");printHex2(ptrX1);
    printf(",\n Y1 at Krtsb - T1: ");printHex2(ptrY1);printf("\n");
    mul_core_Krtsb_test(&ptrX1, &ptrY1, &ptrT1);
    printf("X1*Y1 at Krtsb - T1: ");printHex2(ptrT1);printf("\n\n");
    printf("\nEndKarT1*************************************************************\n");
    
    printf("\nKarT0***************************************************************\n");
    printf("\nKrtsb - T0 start!\n");
    printf("X0 at Krtsb - T0: ");printHex2(ptrX0);
    printf(",\n Y0 at Krtsb - T0 ");printHex2(ptrY0);printf("\n");
    mul_core_Krtsb_test(&ptrX0, &ptrY0, &ptrT0);
    printf("X0*Y0: ");printHex2(ptrT0);printf("\n\n");
    printf("\nEndKarT0*************************************************************\n");
    
    left_shift_word(&ptrT1, 2*l);
    printf("*W^{2*%d}: ", l);printHex2(ptrT1);printf("\n");

    matchSize(ptrT0, ptrT1);
    OR_BINT(ptrT0,ptrT1,&ptrR);
    refine_BINT(ptrT0);
    refine_BINT(ptrT1);
    printf("X1Y1||X0Y0: ");printHex2(ptrR);printf("\n");

    // ADD(&ptrT0, &ptrT1, &ptrTmpR);
    // printf("X1Y1||X0Y0: ");printHex2(ptrTmpR);printf("\n");

    printf("X0: ");printHex2(ptrX0);printf(", X1 ");printHex2(ptrX1);printf("\n");
    bool signS1 = compare_abs_bint(&ptrX1,&ptrX0);
    SUB(&ptrX0, &ptrX1, &ptrS1);
    ptrS1->sign = signS1;
    printf("S1 := X0-X1 = ");printHex2(ptrS1);printf("\n");
    printf("sgn_S1 = %d", (ptrS1)->sign);
    printf("\n\n");
    printf("Y1: ");printHex2(ptrY1);printf(", Y0 ");printHex2(ptrY0);printf("\n");
    bool signS0 = compare_abs_bint(&ptrY0,&ptrY1);
    SUB(&ptrY1, &ptrY0, &ptrS0);
    ptrS0->sign = signS0;
    printf("S0 := Y1-Y0 = ");printHex2(ptrS0);printf("\n");
    printf("sgn_S0 = %d", (ptrS0)->sign);
    printf("\n\n");

    printf("\nKarS***************************************************************\n");
    printf("\nKrtsb - S start!\n");
    printf("sgn_S0: %d, sgn_S1: %d\n", (ptrS0)->sign, (ptrS1)->sign);
    bool sgn_S = ((ptrS0)->sign) ^ ((ptrS1)->sign);
    printf("X0-X1: ");printHex2(ptrS1);printf(", Y1-Y0 ");printHex2(ptrS0);printf("\n");
    mul_core_Krtsb_test(&ptrS1, &ptrS0, &ptrS);
    ptrS->sign = sgn_S;
    printf("sgn_S: %d\n", ptrS->sign);
    printf("S_1*S_0 = (X0-X1)*(Y1-Y0): ");printHex2(ptrS);
    printf("\nsgn_{S1*S0} = %d", ptrS->sign);
    printf("\n\n");
    printf("\nEndKarS***************************************************************\n");
   

    right_shift_word(&ptrT1, 2*l);
    printf("(X0-X1)*(Y1-Y0): ");printHex2(ptrS);printf(", X1Y1: ");printHex2(ptrT1);printf("\n");
    copyBINT(&ptrTmpST1, &ptrS);
    ADD(&ptrTmpST1,&ptrT1,&ptrS);
    printf("(X0-X1)*(Y1-Y0) + X1Y1: ");printHex2(ptrS);
    printf("\nsgn_{(X0-X1)*(Y1-Y0) + X1Y1}: %d", ptrS->sign);
    printf("\n\n");
    
    
    printf("(X0-X1)*(Y1-Y0) + X1Y1: ");printHex2(ptrS);printf(", X0Y0: ");printHex2(ptrT0);printf("\n");
    copyBINT(&ptrTmpST0, &ptrS);
    ADD(&ptrTmpST0,&ptrT0,&ptrS);
    printf("(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0: ");printHex2(ptrS);
    printf("\nsgn_{(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0}: %d", ptrS->sign);
    printf("\n\n");
    

    printf("Before: W^%d: ", l);printHex2(ptrS);printf("\n");
    left_shift_word(&ptrS, l);
    printf("-After: W^%d: ", l);printHex2(ptrS);printf("\n");
    

    printf("X1Y1||X0Y0: ");printHex2(ptrR);printf(", [(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0]w^l: ");printHex2(ptrS);printf("\n");
    copyBINT(&ptrTmpR, &ptrR);
    printf("\nZ length: %d\n",(*pptrZ)->wordlen);
    ADD(&ptrTmpR, &ptrS, pptrZ);
    printf("*Result: ");printHex2(*pptrZ);printf("\n");

    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    // delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS);
    delete_bint(&ptrR);
    delete_bint(&ptrTmpR);
    delete_bint(&ptrTmpST0); delete_bint(&ptrTmpST1);
    printf("\nEndKarMain-----------------------------------------------------------\n");
}

void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_Krtsb_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_Krtsb_xyz");
    if (FLAG >= MIN((*pptrX)->wordlen, (*pptrY)->wordlen)) {
        BINT* tmpTxtBk_X = NULL;
        BINT* tmpTxtBk_Y = NULL;
        copyBINT(&tmpTxtBk_X, pptrX);
        copyBINT(&tmpTxtBk_Y, pptrY);
        MUL_Core_ImpTxtBk_xyz(&tmpTxtBk_X,&tmpTxtBk_Y,pptrZ);
        // MUL_Core_ImpTxtBk_xyz(pptrX,pptrY,pptrZ);
        delete_bint(&tmpTxtBk_X);
        delete_bint(&tmpTxtBk_Y);
        return;  // This will immediately terminate the function.
    }
    matchSize(*pptrX,*pptrY);
    int n = (*pptrX)->wordlen;
    int m = (*pptrX)->wordlen;
    init_bint(pptrZ, n+m);

    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    

    const int l = (MAX(n,m) + 1) >> 1;

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);
    BINT* ptrTmpZ = NULL;
    BINT* ptrTmpST1 = NULL;
    BINT* ptrTmpST0 = NULL;

    // copyBINT(&ptrX1, pptrX);
    // right_shift_word(&ptrX1, l);
    // copyBINT(&ptrX0, pptrX);
    // reduction(&ptrX0, l * WORD_BITLEN);

    // copyBINT(&ptrY1, pptrY);
    // right_shift_word(&ptrY1, l);
    // copyBINT(&ptrY0, pptrY);
    // reduction(&ptrY0, l * WORD_BITLEN);
   
    MUL_Core_Krtsb_xyz(&ptrX1, &ptrY1, &ptrT1);
    MUL_Core_Krtsb_xyz(&ptrX0, &ptrY0, &ptrT0);
    
    left_shift_word(&ptrT1, 2*l);
    
    matchSize(ptrT0, ptrT1);
    OR_BINT(ptrT0,ptrT1,pptrZ);
    refine_BINT(ptrT0);
    refine_BINT(ptrT1);
    
    bool signS1 = compare_abs_bint(&ptrX1,&ptrX0);
    SUB(&ptrX0, &ptrX1, &ptrS1);
    ptrS1->sign = signS1;
    
    bool signS0 = compare_abs_bint(&ptrY0,&ptrY1);
    SUB(&ptrY1, &ptrY0, &ptrS0);
    ptrS0->sign = signS0;
    
    bool sgn_S = ((ptrS0)->sign) ^ ((ptrS1)->sign);
    MUL_Core_Krtsb_xyz(&ptrS1, &ptrS0, &ptrS);
    ptrS->sign = sgn_S;

    right_shift_word(&ptrT1, 2*l);
    copyBINT(&ptrTmpST1, &ptrS);
    ADD(&ptrTmpST1,&ptrT1,&ptrS);
    copyBINT(&ptrTmpST0, &ptrS);
    ADD(&ptrTmpST0,&ptrT0,&ptrS);
    left_shift_word(&ptrS, l);
    copyBINT(&ptrTmpZ, pptrZ);
    ADD(&ptrTmpZ, &ptrS, pptrZ);
    
    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS);
    delete_bint(&ptrTmpZ);
    delete_bint(&ptrTmpST0); delete_bint(&ptrTmpST1);
}

