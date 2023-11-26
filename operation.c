#include <stdio.h>
#include <stdlib.h>
// #include <stdbool.h>
#include <string.h>
#include <math.h>

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
void OOR_BINT(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    int max_len = MAXIMUM((*pptrX)->wordlen, (*pptrY)->wordlen);    
    matchSize(*pptrX, *pptrY);
    for (int i = 0; i < max_len; i++) {
        (*pptrZ)->val[i] = (*pptrX)->val[i] | (*pptrY)->val[i];
    }
    (*pptrZ)->wordlen = max_len; // The result size will be size of the smaller operand
    refine_BINT(*pptrX);
    refine_BINT(*pptrY);
    refine_BINT(*pptrZ);
    // ptrZ->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = MINIMUM(ptrX->wordlen, ptrY->wordlen);
    int max_len = MAXIMUM(ptrX->wordlen, ptrY->wordlen);
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] | ptrY->val[i];
    }
    for (int i = min_len; i < max_len; i++) {
        if(ptrX->wordlen > ptrY->wordlen)
            (*pptrZ)->val[i] = ptrX->val[i];
        else
            (*pptrZ)->val[i] = ptrY->val[i];
    }
    (*pptrZ)->wordlen = max_len; // The result size will be size of the smaller operand
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
}
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "add_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "add_core_xyz");
    if (!compare_abs_bint(pptrX,pptrY)) {
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
    refine_BINT(*pptrZ);

    // int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;

    // // if (!pptrZ || !*pptrZ || !(*pptrZ)->val)
    // init_bint(pptrZ, n+1);
    // CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "add_core_xyz");

    // WORD res = 0x00;
    // WORD carry = 0x00;
    // WORD k = 0x00;

    // for (int i = 0; i < m; i++) {
    //     // printf("Before: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res);
    //     add_carry((*pptrX)->val[i], (*pptrY)->val[i], k, &carry, &res);
    //     (*pptrZ)->val[i] = res;
    //     // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res,i,ptrZ->val[i]);
    //     k = carry;
    // }
    // for (int i = m; i < n; i++) {
    //     add_carry((*pptrX)->val[i],(WORD)0, k, &carry, &res);
    //     (*pptrZ)->val[i] = res;
    //     k = carry;
    // }
    // if(k == 1) {
    //     (*pptrZ)->val[n] = k;
    // } else {
    //     (*pptrZ)->wordlen = n;
    //     WORD* tmp = (*pptrZ)->val;
    //     tmp = (WORD*)realloc((*pptrZ)->val, n*sizeof(WORD));
    //     (*pptrZ)->val = tmp;
    // }
    // refine_BINT((*pptrZ));
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
    bool xGeqy = compare_abs_bint(pptrX, pptrY);
    if(!xGeqy) swapBINT(pptrX,pptrY);

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
    refine_BINT(*pptrX); refine_BINT(*pptrY);
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
    delete_bint(&ptrTemp);
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
    custom_printHex_xy(*pptrX, *pptrY, MAXIMUM(n,m));

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
    // if(!compare_abs_bint(pptrX,pptrY)) swapBINT(pptrX,pptrY);
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

        ADD(&ptrT1, &ptrT0, &ptrT);
        
        left_shift_word(&ptrT, j);

        copyBINT(&ptrTmpZ, pptrZ);
        ADD(&ptrTmpZ, &ptrT, pptrZ);
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
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_Krtsb_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_Krtsb_xyz");

    print_bint_hex_split(*pptrX);
    print_bint_hex_split(*pptrY);

    int n = (*pptrX)->wordlen;
    int m = (*pptrY)->wordlen;
    printf("n, m = 0x%0x 0x%0x\n", n, m);
    static int lenZ = -1; // Declare lenZ as a static variable
    if (lenZ == -1) {
        lenZ = n + m; // Calculate lenZ only if it hasn't been initialized yet
        init_bint(pptrZ, lenZ);
        CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    }
    if (FLAG >= MINIMUM(n,m)) {
        BINT* tmpTxtBk_X = NULL;
        BINT* tmpTxtBk_Y = NULL;
        printf("=========================================\n");
        printf("FLAG ON!!\n");
        print_bint_hex_split(*pptrX);
        print_bint_hex_split(*pptrY);
        copyBINT(&tmpTxtBk_X, pptrX);
        copyBINT(&tmpTxtBk_Y, pptrY);
        MUL_Core_ImpTxtBk_xyz(&tmpTxtBk_X,&tmpTxtBk_Y,pptrZ);
        delete_bint(&tmpTxtBk_X);
        delete_bint(&tmpTxtBk_Y);
        print_bint_hex_split(*pptrZ);
        printf("=========================================\n");
        return;
    }
    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    // matchSize(*pptrX,*pptrY);
    int l = (MAXIMUM(n,m) + 1) >> 1;
    printf("l = 0x%x\n\n", l);

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrShiftT1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);

    BINT* ptrR = NULL;
    BINT* ptrTmpR = NULL;
    init_bint(&ptrR, (*pptrZ)->wordlen);
    BINT* ptrTmpST1 = NULL;
    BINT* ptrTmpST0 = NULL;

    matchSize(*pptrX, *pptrY);
    copyBINT(&ptrX1, pptrX); right_shift_word(&ptrX1, l);
    copyBINT(&ptrX0, pptrX); reduction(&ptrX0, l * WORD_BITLEN);
    printf("Split X to X1: ");print_bint_hex_python(&ptrX1);printf("\n");
    printf("Split X to X0: ");print_bint_hex_python(&ptrX0);printf("\n");

    copyBINT(&ptrY1, pptrY); right_shift_word(&ptrY1, l);
    copyBINT(&ptrY0, pptrY); reduction(&ptrY0, l * WORD_BITLEN);
    printf("Split Y to Y1: ");print_bint_hex_python(&ptrY1);printf("\n");
    printf("Split Y to Y0: ");print_bint_hex_python(&ptrY0);printf("\n");

    mul_core_Krtsb_test(&ptrX1, &ptrY1, &ptrT1);
    mul_core_Krtsb_test(&ptrX0, &ptrY0, &ptrT0);
    printf("T1: ");print_bint_hex_python(&ptrT1);printf("\n");
    printf("T0: ");print_bint_hex_python(&ptrT0);printf("\n");

    copyBINT(&ptrShiftT1, &ptrT1);
    left_shift_word(&ptrShiftT1, 2*l);
    ADD(&ptrShiftT1, &ptrT0, &ptrR);
    printf("R: ");print_bint_hex_python(&ptrR);printf("\n");

    SUB(&ptrX0, &ptrX1, &ptrS1);
    SUB(&ptrY1, &ptrY0, &ptrS0);
    printf("S1: ");print_bint_hex_python(&ptrS1);printf("\n");
    printf("S0: ");print_bint_hex_python(&ptrS0);printf("\n");

    bool sgn_S = ((ptrS0)->sign) ^ ((ptrS1)->sign);
    ptrS0->sign = false;
    ptrS1->sign = false;
    mul_core_Krtsb_test(&ptrS1, &ptrS0, &ptrS);
    ptrS->sign = sgn_S;
    printf("S: ");print_bint_hex_python(&ptrS);printf("\n");

    // copyBINT(&ptrTmpST1, &ptrS);
    ADD(&ptrS,&ptrT1,&ptrTmpST1);
    copyBINT(&ptrS,&ptrTmpST1);
    printf("S + t1: ");print_bint_hex_python(&ptrS);printf("\n");

    copyBINT(&ptrTmpST0, &ptrS);
    ADD(&ptrTmpST0,&ptrT0,&ptrS);
    printf("S + t0: ");print_bint_hex_python(&ptrS);printf("\n");

    left_shift_word(&ptrS, l);
    printf("S << lw: ");print_bint_hex_python(&ptrS);printf("\n");

    ADD(&ptrR, &ptrS, pptrZ);
    printf("Result: ");print_bint_hex_python(pptrZ);printf("\n");

    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrShiftT1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS);
    delete_bint(&ptrR); delete_bint(&ptrTmpR);
    delete_bint(&ptrTmpST0); delete_bint(&ptrTmpST1);
}
void Krtsb_FLAG_Test(BINT** pptrX, BINT** pptrY, BINT** pptrZ, int flag) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_Krtsb_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_Krtsb_xyz");

    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;
    static int lenZ = -1; // Declare lenZ as a static variable
    if (lenZ == -1) {
        lenZ = n + m; // Calculate lenZ only if it hasn't been initialized yet
        init_bint(pptrZ, lenZ);
        CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    }
    if (FLAG >= MINIMUM(n,m)) {
        BINT* tmpTxtBk_X = NULL; BINT* tmpTxtBk_Y = NULL;
        copyBINT(&tmpTxtBk_X, pptrX); copyBINT(&tmpTxtBk_Y, pptrY);
        MUL_Core_ImpTxtBk_xyz(&tmpTxtBk_X,&tmpTxtBk_Y,pptrZ);
        delete_bint(&tmpTxtBk_X); delete_bint(&tmpTxtBk_Y);
        return;
    }
    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    int l = (MAXIMUM(n,m) + 1) >> 1;

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrShiftT1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);

    BINT* ptrR = NULL;
    BINT* ptrTmpR = NULL;
    init_bint(&ptrR, (*pptrZ)->wordlen);
    BINT* ptrTmpST1 = NULL;
    BINT* ptrTmpST0 = NULL;

    matchSize(*pptrX, *pptrY);
    copyBINT(&ptrX1, pptrX); right_shift_word(&ptrX1, l);
    copyBINT(&ptrX0, pptrX); reduction(&ptrX0, l * WORD_BITLEN);
    
    copyBINT(&ptrY1, pptrY); right_shift_word(&ptrY1, l);
    copyBINT(&ptrY0, pptrY); reduction(&ptrY0, l * WORD_BITLEN);
    
    Krtsb_FLAG_Test(&ptrX1, &ptrY1, &ptrT1, flag);
    Krtsb_FLAG_Test(&ptrX0, &ptrY0, &ptrT0, flag);
    
    copyBINT(&ptrShiftT1, &ptrT1);
    left_shift_word(&ptrShiftT1, 2*l);
    ADD(&ptrShiftT1, &ptrT0, &ptrR);
    
    SUB(&ptrX0, &ptrX1, &ptrS1);
    SUB(&ptrY1, &ptrY0, &ptrS0);
    
    bool sgn_S = ((ptrS0)->sign) ^ ((ptrS1)->sign);
    ptrS0->sign = false; ptrS1->sign = false;
    Krtsb_FLAG_Test(&ptrS1, &ptrS0, &ptrS, flag);
    ptrS->sign = sgn_S;
    
    ADD(&ptrS,&ptrT1,&ptrTmpST1);
    copyBINT(&ptrS,&ptrTmpST1);
    
    copyBINT(&ptrTmpST0, &ptrS);
    ADD(&ptrTmpST0,&ptrT0,&ptrS);
    
    left_shift_word(&ptrS, l);
    
    ADD(&ptrR, &ptrS, pptrZ);
    
    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrShiftT1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS);
    delete_bint(&ptrR); delete_bint(&ptrTmpR);
    delete_bint(&ptrTmpST0); delete_bint(&ptrTmpST1);
}
void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_Krtsb_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_Krtsb_xyz");

    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;
    static int lenZ = -1; // Declare lenZ as a static variable
    if (lenZ == -1) {
        lenZ = n + m; // Calculate lenZ only if it hasn't been initialized yet
        init_bint(pptrZ, lenZ);
        CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    }
    if (FLAG >= MINIMUM(n,m)) {
        BINT* tmpTxtBk_X = NULL; BINT* tmpTxtBk_Y = NULL;
        copyBINT(&tmpTxtBk_X, pptrX); copyBINT(&tmpTxtBk_Y, pptrY);
        MUL_Core_ImpTxtBk_xyz(&tmpTxtBk_X,&tmpTxtBk_Y,pptrZ);
        delete_bint(&tmpTxtBk_X); delete_bint(&tmpTxtBk_Y);
        return;
    }
    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    int l = (MAXIMUM(n,m) + 1) >> 1;

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrShiftT1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);

    BINT* ptrR = NULL;
    BINT* ptrTmpR = NULL;
    init_bint(&ptrR, (*pptrZ)->wordlen);
    BINT* ptrTmpST1 = NULL;
    BINT* ptrTmpST0 = NULL;

    matchSize(*pptrX, *pptrY);
    copyBINT(&ptrX1, pptrX); right_shift_word(&ptrX1, l);
    copyBINT(&ptrX0, pptrX); reduction(&ptrX0, l * WORD_BITLEN);
    
    copyBINT(&ptrY1, pptrY); right_shift_word(&ptrY1, l);
    copyBINT(&ptrY0, pptrY); reduction(&ptrY0, l * WORD_BITLEN);
    
    MUL_Core_Krtsb_xyz(&ptrX1, &ptrY1, &ptrT1);
    MUL_Core_Krtsb_xyz(&ptrX0, &ptrY0, &ptrT0);
    
    copyBINT(&ptrShiftT1, &ptrT1);
    left_shift_word(&ptrShiftT1, 2*l);
    ADD(&ptrShiftT1, &ptrT0, &ptrR);
    
    SUB(&ptrX0, &ptrX1, &ptrS1);
    SUB(&ptrY1, &ptrY0, &ptrS0);
    
    bool sgn_S = ((ptrS0)->sign) ^ ((ptrS1)->sign);
    ptrS0->sign = false; ptrS1->sign = false;
    MUL_Core_Krtsb_xyz(&ptrS1, &ptrS0, &ptrS);
    ptrS->sign = sgn_S;
    
    ADD(&ptrS,&ptrT1,&ptrTmpST1);
    copyBINT(&ptrS,&ptrTmpST1);
    
    copyBINT(&ptrTmpST0, &ptrS);
    ADD(&ptrTmpST0,&ptrT0,&ptrS);
    
    left_shift_word(&ptrS, l);
    
    ADD(&ptrR, &ptrS, pptrZ);
    
    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrShiftT1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS);
    delete_bint(&ptrR); delete_bint(&ptrTmpR);
    delete_bint(&ptrTmpST0); delete_bint(&ptrTmpST1);
}

void squ_core(WORD valX,BINT** pptrZ){//테스트 완료
    //CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_xyz");

    const int half_w = WORD_BITLEN / 2; // if w=32, half_w = 16 = 2^4
	const WORD MASK = (1 << half_w) - 1;

	BINT* C=NULL;
    init_bint(&C,2);
    BINT* T=NULL;
    init_bint(&T ,2);
    BINT* Temp=NULL;
    init_bint(&Temp ,2);
    // Split the WORDs into halves
	WORD X0 = valX & MASK;
	WORD X1 = valX >> half_w;
	

    // Cross multiplication
	WORD C1 = X1 * X1;
	WORD C0 = X0 * X0;
    C->val[0] = C0;
    C->val[1] = C1;
	T->val[0] = X0 * X1;
    left_shift_bit(T,half_w + 1); 
    ADD(&C,&T,&Temp);
    
	
    copyBINT(pptrZ,&Temp);
    delete_bint(&C);
    delete_bint(&T);
    delete_bint(&Temp);
}
void SQU_Txtbk_xz(BINT** pptrX,BINT** pptrZ){
    //init_bint(pptrZ,2*((*pptrX)->wordlen));
    BINT* C1=NULL;
    init_bint(&C1,1);
    BINT* C2=NULL;
    init_bint(&C2,1);
    BINT* T1=NULL;
    init_bint(&T1,2);
    BINT* T2=NULL;
    init_bint(&T2,2);
    BINT* Temp=NULL;
    BINT* Temp2=NULL;
    for(int j=0; j<(*pptrX)->wordlen; j++){
        squ_core((*pptrX)->val[j],&T1);
        left_shift_word(&T1,(2*j));
        ADD(&T1,&C1,&Temp);
        copyBINT(&C1,&Temp);
        init_bint(&T1,2);
        for ( int i = j+1; i< (*pptrX)->wordlen;i++){
            mul_xyz((*pptrX)->val[j],(*pptrX)->val[i],&T2);
            left_shift_word(&T2,(i+j));
            ADD(&C2,&T2,&Temp2);
            copyBINT(&C2,&Temp2);
            init_bint(&T2,2);
        }
    }
    left_shift_bit(C2,1);
    ADD(&C1,&C2,pptrZ);
    (*pptrZ)->sign = false;
    delete_bint(&C1);
    delete_bint(&C2);
    delete_bint(&T1);
    delete_bint(&T2);
    delete_bint(&Temp);
    delete_bint(&Temp2);
}
void SQU_Krtsb_xz(BINT** pptrX, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "SQU_Krtsb_xz");
    int n = (*pptrX)->wordlen;
    static int lenZ = -1; // Declare lenZ as a static variable
    if (lenZ == -1) {
        lenZ = 2 * n;
        init_bint(pptrZ, lenZ);
        CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SQU_Krtsb_xz");
    }
    if (FLAG >= n) {
        BINT* tmpTxtBk_X = NULL; copyBINT(&tmpTxtBk_X, pptrX);
        SQU_Txtbk_xz(&tmpTxtBk_X, pptrZ);
        delete_bint(&tmpTxtBk_X);
        return;
    }
    init_bint(pptrZ, 2*n);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SQU_Krtsb_xz");
    int l = (n + 1) >> 1;

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrShiftT1 = NULL;
    BINT* ptrR = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);

    copyBINT(&ptrX1, pptrX); right_shift_word(&ptrX1, l);
    copyBINT(&ptrX0, pptrX); reduction(&ptrX0, l * WORD_BITLEN);

    SQU_Krtsb_xz(&ptrX1, &ptrT1);
    SQU_Krtsb_xz(&ptrX0, &ptrT0);

    copyBINT(&ptrShiftT1, &ptrT1);
    left_shift_word(&ptrShiftT1, 2*l);
    ADD(&ptrShiftT1, &ptrT0, &ptrR);

    MUL_Core_Krtsb_xyz(&ptrX1, &ptrX0, &ptrS);
    left_shift_word(&ptrS, l);
    left_shift_bit(ptrS, 1);

    ADD(&ptrR, &ptrS, pptrZ);

    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrShiftT1);
    delete_bint(&ptrS);
    delete_bint(&ptrR);
}

// Only 0 <= X < YW
void DIV_Bianry_Long_Test(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR) {
    if((*pptrDivisor)->wordlen == 0 || ((*pptrDivisor)->wordlen == 1 && (*pptrDivisor)->val[0] == 0) ) {
        fprintf(stderr, "Division by zero error.\n");
        exit(1);
    }
    if (!(*pptrDividend)->sign && compare_bint(pptrDivisor, pptrDividend)) {
        SET_BINT_ZERO(pptrQ);
        copyBINT(pptrR, pptrDividend);
        // pptrR = pptrDividend; // Correctly assign the value of pptrDividend to what pptrR is pointing to.
        return;
    }
    int n = (*pptrDividend)->wordlen;
    // int m = (*pptrDivisor)->wordlen;
    // init_bint(pptrQ, n);
    SET_BINT_ZERO(pptrQ);
    (*pptrQ)-> sign = (*pptrDividend)->sign ^ (*pptrDivisor)->sign;
    // init_bint(pptrR, m);
    SET_BINT_ZERO(pptrR);
    
    BINT* ptrTmpSub = NULL;
    SET_BINT_ZERO(&ptrTmpSub);
    BINT* ptrTmpAdd = NULL;
    SET_BINT_ZERO(&ptrTmpAdd);

    // bool* dividend = HexToBinary(*pptrDividend);
    // bool* divisor = HexToBinary(*pptrDivisor);

    // Initialize quotient and remainder.
    // bool* quotient = (bool*)calloc(n, sizeof(bool));
    // int quotient_len = n * WORD_BITLEN;
    // bool* remainder = (bool*)calloc(m, sizeof(bool));
    // int remainder_len = m * WORD_BITLEN;
    // bool* tmp_remainder = (bool*)calloc(m,sizeof(bool));    

    // printf("\nHexX: ");print_bint_hex_split(*pptrDividend);
    // printf("BinX: ");
    // bool* binary = HexToBinary(*pptrDividend);
    // PrintBinary(binary, n*WORD_BITLEN);
    // printf("\nHexY: ");print_bint_hex_split(*pptrDivisor);
    // printf("BinY: ");
    // bool* binary2 = HexToBinary(*pptrDivisor);
    // PrintBinary(binary2, m*WORD_BITLEN);
    
    matchSize(*pptrDividend,*pptrDivisor);
    for(int i = n * WORD_BITLEN - 1; i >= 0 ; i--) {
        printf("\nn: %02d |",i);
        // printf("Q: ");print_bint_hex_python(*pptrQ);printf(" ");
        // printf("R: ");print_bint_hex_python(*pptrR);printf(" \n");
        left_shift_bit(*pptrR, 1);  // R <- 2R
        printf("Get %d-th bit: %d\n", i, GET_BIT(pptrDividend, i));
        // printf("-After R: ");print_bint_hex_split(*pptrR);
        (*pptrR)->val[0] ^= GET_BIT(pptrDividend, i); // R <- R + x_i
        // printf("Y: ");print_bint_hex_python(*pptrDivisor);printf(" \n");
        
        printf("R(%d)>=Y(%d)? : %d\n", (*pptrR)->wordlen,(*pptrDivisor)->wordlen,compare_bint(pptrR,pptrDivisor));

        matchSize(*pptrR,*pptrDivisor);
        if(compare_bint(pptrR,pptrDivisor)) {   // R >= Y
            printf("\nR:");print_bint_hex(*pptrR);
            // printf("Before Q: ");print_bint_hex_split(*pptrQ);
            printf("Y:");print_bint_hex(*pptrDivisor);
            SUB(pptrR,pptrDivisor,&ptrTmpSub);
            printf("R-Y:");print_bint_hex(ptrTmpSub);
            copyBINT(pptrR,&ptrTmpSub);
            printf("R-Y:");print_bint_hex(*pptrR);

            SET_BINT_ONE(&ptrTmpAdd);
            left_shift_bit(ptrTmpAdd, i);
            matchSize(*pptrQ, ptrTmpAdd);
            for(int j = 0; j < ptrTmpAdd->wordlen; j++) {
                (*pptrQ)->val[j] ^= ptrTmpAdd->val[j];
            }
            
            // printf("-After Q: ");print_bint_hex_split(*pptrQ);
        }
    }
    

    // free(binary);
    // *pptrQ = BinaryToHex(quotient, n * WORD_BITLEN);
    // *pptrR = BinaryToHex(remainder, m * WORD_BITLEN);

    refine_BINT(*pptrQ);
    refine_BINT(*pptrR);

    delete_bint(&ptrTmpAdd);
    delete_bint(&ptrTmpSub);
}
void DIV_Bianry_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR) {
    if((*pptrDivisor)->wordlen == 0 || ((*pptrDivisor)->wordlen == 1 && (*pptrDivisor)->val[0] == 0) ) {
        fprintf(stderr, "Division by zero error.\n");
        exit(1);
    }
    if (!(*pptrDividend)->sign && compare_bint(pptrDivisor, pptrDividend)) {
        SET_BINT_ZERO(pptrQ);
        copyBINT(pptrR, pptrDividend);
        return;
    }
    int n = (*pptrDividend)->wordlen;
    SET_BINT_ZERO(pptrQ);
    (*pptrQ)-> sign = (*pptrDividend)->sign ^ (*pptrDivisor)->sign;
    SET_BINT_ZERO(pptrR);
    
    BINT* ptrTmpSub = NULL;
    SET_BINT_ZERO(&ptrTmpSub);
    BINT* ptrTmpAdd = NULL;
    SET_BINT_ZERO(&ptrTmpAdd);

    matchSize(*pptrDividend,*pptrDivisor);
    for(int i = n * WORD_BITLEN - 1; i >= 0 ; i--) {
        left_shift_bit(*pptrR, 1);  // R <- 2R
        (*pptrR)->val[0] ^= GET_BIT(pptrDividend, i); // R <- R + x_i
        
        matchSize(*pptrR,*pptrDivisor);
        if(compare_bint(pptrR,pptrDivisor)) {   // R >= Y
            SUB(pptrR,pptrDivisor,&ptrTmpSub);
            copyBINT(pptrR,&ptrTmpSub);

            SET_BINT_ONE(&ptrTmpAdd);
            left_shift_bit(ptrTmpAdd, i);
            matchSize(*pptrQ, ptrTmpAdd);
            for(int j = 0; j < ptrTmpAdd->wordlen; j++) {
                (*pptrQ)->val[j] ^= ptrTmpAdd->val[j];
            }
        }
    }
    refine_BINT(*pptrQ);
    refine_BINT(*pptrR);

    delete_bint(&ptrTmpAdd);
    delete_bint(&ptrTmpSub);
}

// Function to perform division as per the provided algorithm
void DIV_long(BINT** X, BINT** Y, BINT** Q, BINT** R) {
    // Ensure proper initialization of Q and R
    init_bint(Q, 1);  // Assume Q is no longer than 1 word.
    init_bint(R, (*X)->wordlen); // R has the same word length as X for safety.

    // Fetch bit lengths
    int n = (*X)->wordlen;  // Pass the address of the pointer
    int m = (*Y)->wordlen;  // Pass the address of the pointer
    
    // Calculate W as 2^word size
    WORD W = ((WORD)1) << (WORD_BITLEN - 1); // Use one less bit to avoid overflow

    // Get the most significant word from X and Y
    WORD x_m = get_mth_word(*X, m);     // assuming get_mth_word fetches the m-th WORD from BINT
    WORD x_m_1 = get_mth_word(*X, m - 1);
    WORD y_m_1 = get_mth_word(*Y, m - 1);

    if(y_m_1 == 0)
        printf("!!!!!!!!!!!!\n");

    // Main algorithm as per the pseudocode
    if (n == m) {
        (*Q)->val[0] = x_m_1 / y_m_1;
    }
    if (n == m + 1) {
        if (x_m == y_m_1) {
            (*Q)->val[0] = W - 1;
        } else {
            // Cast to a larger integer type if necessary to prevent overflow
            unsigned long long dividend = ((unsigned long long)x_m << (sizeof(WORD) * 8)) | x_m_1;
            (*Q)->val[0] = (WORD)(dividend / y_m_1);
        }
    }

    // Calculate R = X - Y * Q
    BINT* YQ = NULL;
    init_bint(&YQ, (*Y)->wordlen);
    MUL_Core_Krtsb_xyz(Y, Q, &YQ);  // Pass the addresses of the pointers
    SUB(X, &YQ, R);                 // Pass the addresses of the pointers

    // Correct R if it is negative
    while ((*R)->sign) {
        BINT* ONE;
        BINT* tmpQ;
        BINT* tmpR;
        // SET_BINT_ONE(&ONE);         // Set ONE to 1
        SET_BINT_CUSTOM_ONE(&ONE, (*Q)->wordlen);
        copyBINT(&tmpQ, Q);
        SUB(&tmpQ, &ONE, Q);            // Q = Q - 1
        copyBINT(&tmpR, R);
        ADD(&tmpR, Y, R);              // R = R + Y
        delete_bint(&ONE);          // Clean up ONE
        delete_bint(&tmpQ);delete_bint(&tmpR);
    }

    // Clean up
    delete_bint(&YQ);
}


void mul_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ) {
int bit_len = BIT_LENGTH(ptrY);

    BINT* t0=NULL;
    BINT* temp=NULL;
    SET_BINT_ONE(&t0);

    for (int i= bit_len-1 ; i >= 0 ;i--){
        init_bint(&temp,1);

        if (GET_BIT(ptrY,i) == 1){
            
            SQU_Txtbk_xz(&t0,&temp);
            mul_core_TxtBk_xyz(&temp,ptrX,&t0);
        }
        else{
            SQU_Txtbk_xz(&t0,&temp);
            copyBINT(&t0,&temp);
        }
    }       
    copyBINT(ptrZ,&t0);
    refine_BINT(*ptrZ);
    delete_bint(&t0);
    delete_bint(&temp);
    
}
void add_LeftToRight(BINT** ptrX, BINT** ptrY, BINT** ptrZ) {
    int bit_len = BIT_LENGTH(ptrY);

    BINT* t0=NULL;
    BINT* temp=NULL;
    SET_BINT_ZERO(&t0);

    for (int i= bit_len-1 ; i >= 0 ;i--){
        init_bint(&temp,1);

        if (GET_BIT(ptrY,i) == 1){
            left_shift_bit(t0,1);
            copyBINT(&temp, &t0);
            ADD(&temp,ptrX,&t0);
        }
        else{
            left_shift_bit(t0,1);
        }
    }       
    copyBINT(ptrZ,&t0);
    refine_BINT(*ptrZ);
    delete_bint(&t0);
    delete_bint(&temp);
    
}
void mul_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ) {
    int bit_len = BIT_LENGTH(ptrY);

    BINT* t0 = NULL;
    BINT* t1 = NULL;
    BINT* temp = NULL;
    SET_BINT_ONE(&t0);
    copyBINT(&t1, ptrX);

    for (int i = 0; i <= bit_len-1; i++) {
        init_bint(&temp, 1);

        if (GET_BIT(ptrY, i) == 1) {
            
            mul_core_TxtBk_xyz(&t0, &t1, &temp);
            copyBINT(&t0, &temp);

            SQU_Txtbk_xz(&t1, &temp);
            copyBINT(&t1, &temp);


        } else {
            SQU_Txtbk_xz(&t1, &temp);
            copyBINT(&t1, &temp);
        }
    }

    copyBINT(ptrZ, &t0);
    refine_BINT(*ptrZ);
    delete_bint(&t0);
    delete_bint(&t1);
    delete_bint(&temp);
}
void add_RightToLeft(BINT** ptrX, BINT** ptrY, BINT** ptrZ) {
    int bit_len = BIT_LENGTH(ptrY);

    BINT* t0 = NULL;
    BINT* t1 = NULL;
    BINT* temp = NULL;
    SET_BINT_ZERO(&t0);
    copyBINT(&t1, ptrX);

    for (int i = 0; i <= bit_len - 1; i++) {
        init_bint(&temp, 1);

        if (GET_BIT(ptrY, i) == 1) {
            ADD(&t0, &t1, &temp);
            copyBINT(&t0, &temp);

            left_shift_bit(t1, 1);
   
        } else {
            left_shift_bit(t1, 1);
 
        }
    }
    copyBINT(ptrZ, &t0);
    refine_BINT(*ptrZ);
    delete_bint(&t0);
    delete_bint(&t1);
    delete_bint(&temp);
}
void exp_Mongomery(BINT** ptrX,BINT** ptrY,BINT** ptrZ){
    int bit_len = BIT_LENGTH_NONZERO(ptrY);
    BINT* t0=NULL;
    BINT* t1=NULL;
    BINT* temp=NULL;
    BINT* temp2=NULL;
    SET_BINT_ONE(&t0);
    copyBINT(&t1,ptrX);
    
    for (int i= bit_len-1 ; i >= 0 ;i--){
        init_bint(&temp,1);
        init_bint(&temp2,1);

        if (GET_BIT(ptrY,i) == 0){
            mul_core_TxtBk_xyz(&t0,&t1,&temp);
            copyBINT(&t1,&temp);
            copyBINT(&temp2,&t0);
            mul_core_TxtBk_xyz(&t0,&temp2,&temp);
            copyBINT(&t0,&temp);
        }
        else{
            mul_core_TxtBk_xyz(&t0,&t1,&temp);
            copyBINT(&t0,&temp);
            copyBINT(&temp2,&t1);
            mul_core_TxtBk_xyz(&t1,&temp2,&temp);
            copyBINT(&t1,&temp);
        }

    }
    copyBINT(ptrZ,&t0);
    refine_BINT(*ptrZ);
    delete_bint(&t0);
    delete_bint(&t1);
    delete_bint(&temp);
    delete_bint(&temp2);
}

void Mod_Exp_Mongo(BINT** ptrX,BINT** ptrY,BINT** ptrM,BINT** ptrZ) {
    //일단 비트수가 필요하다 그만큼 연산을 해야하니 
    int bit_len = BIT_LENGTH_NONZERO(ptrY);
    BINT* t0=NULL;
    BINT* t1=NULL;
    BINT* temp=NULL;
    BINT* temp2=NULL;
    BINT* Q1=NULL;
    BINT* Q2=NULL;
    SET_BINT_ONE(&t0);
    copyBINT(&t1,ptrX);
    

    for (int i= bit_len-1 ; i >= 0 ;i--){
        init_bint(&temp,1);
        init_bint(&temp2,1);
        init_bint(&Q1,1);
        init_bint(&Q2,1);

        if (GET_BIT(ptrY,i) == 0){
            mul_core_TxtBk_xyz(&t0,&t1,&temp);
            DIV_Bianry_Long(&temp,ptrM,&Q1,&t1);
            SQU_Txtbk_xz(&t0,&temp2);
            DIV_Bianry_Long(&temp2,ptrM,&Q2,&t0);
        }
        else{
            mul_core_TxtBk_xyz(&t0,&t1,&temp);
            DIV_Bianry_Long(&temp,ptrM,&Q1,&t0);
            SQU_Txtbk_xz(&t1,&temp2);
            DIV_Bianry_Long(&temp2,ptrM,&Q2,&t1);
        }

    }
    copyBINT(ptrZ,&t0);
    refine_BINT(*ptrZ);
    delete_bint(&t0);
    delete_bint(&t1);
    delete_bint(&temp);
    delete_bint(&temp2);
    delete_bint(&Q1);
    delete_bint(&Q2);
}